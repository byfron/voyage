import numpy as np
import pdb
import random
from noise import pnoise1, pnoise2, snoise2
from PIL import Image, ImageDraw
from scipy.spatial.distance import cdist
import networkx as nx
import math

class Region:
    def __init__(self, polygon):
        self.moist = []
        self.height = 0
        self.biome = []
        self.polygon = polygon
        self.edge_list = []
        self.isWater = False
        self.isLake = False
        self.isRiverSide = False
        self.distToCenter = -1

class World:

    def __init__(self, graph, properties):
        self.graph = graph
        self.regions = []
        self.createShape()
        self.createLakes()
        self.createHeight()
        self.createRiver()
        self.createBiomes()

    def createShape(self):
                
        Y_vector = np.array([0,1]);
        vectors = self.graph.points - np.array([0.5,0.5]);
        norm_vectors = (vectors.T/np.sqrt(np.sum(vectors**2,1))).T
        dotynv = np.dot(Y_vector, norm_vectors.T)
        detynv = Y_vector[0]*norm_vectors[:,1] - norm_vectors[:,0]*Y_vector[1]
        angles = np.arctan2(detynv, dotynv)
        seed = random.random()
        
        #sort in increasing angle
        idx = np.argsort(angles)
        base = 0.1

        #generate a perlin noise vectors with the same elements as points
        noise_vec = np.zeros(self.graph.N)
        for i in range(self.graph.N):
                noise_vec[i] = pnoise1(float(i)/self.graph.N, 8, base=int(seed*100))

        #rescale noise
        noise_vec = np.abs(noise_vec) + base;
        noise_vec = noise_vec/noise_vec.max()*0.8

        #compute distances to the centroid of all points
        self.distances_to_center = np.sqrt(np.sum((self.graph.points - np.array([0.5, 0.5]))**2,1))
        max_dist = self.distances_to_center.max()        

        self.regions = [None]*self.graph.N
        self.land_idx = []
        for i in range(self.graph.N):
                reg = Region(self.graph.polygons[idx[i]])
                reg.distToCenter = self.distances_to_center[idx[i]]
                
                if self.distances_to_center[idx[i]] > max_dist * noise_vec[i]:
                    reg.isWater = True
                else:
                    reg.isWater = False
                    self.land_idx.append(i)

                self.regions[idx[i]] = reg
 
    def createLakes(self):

        #find lake candidates as a random region in land not very close to the sea
        idx = 0
        idx_lake_candidates = []
        for reg in self.regions:
                if reg.isWater == False & (reg.distToCenter/self.distances_to_center.max() < 1.0/4):
                    idx_lake_candidates.append(idx)
                idx = idx + 1
        lake_src = random.randint(0, len(idx_lake_candidates))
        lake_index = idx_lake_candidates[lake_src]
        closest_lake_regions = np.argsort(cdist([self.graph.points[lake_index,:]], self.graph.points))[0]
        lake_size = random.randint(3, 7)
        for i in range(lake_size):
                 self.regions[closest_lake_regions[i]].isLake = True


    def createHeight(self):

        #compute distance from land regions to the water regions
        idx = 0
        land_regions_idx = []
        water_regions_idx = []
        self.land_points = []
        self.water_points = []
        self.height_vector = np.zeros(len(self.regions))        
        for reg in self.regions:
            if reg.isWater == False and reg.isLake == False:
                self.land_points.append(self.graph.points[idx,:])
                land_regions_idx.append(idx)                
            else:
                self.water_points.append(self.graph.points[idx,:])
                water_regions_idx.append(idx)
            idx = idx + 1

        #measure distances from land to water
        distLandWater = cdist(np.array(self.land_points), np.array(self.water_points)).min(1)

        #compute height of each region acoording to the distance
        distLandWater = distLandWater/distLandWater.max()

        #create a 2D noise map
        nres = 100
        seed = random.random()
        #generate perlin height map
        hmap = np.zeros((nres, nres))
        freq = nres * .2
        turbulence = 0.5
        for i in range(nres):
            for j in range(nres):
                hmap[i,j] = pnoise2(float(i)/freq, float(j)/freq, 5, base=int(seed*100))

        hmap = (hmap - hmap.min())# - .5

        #distort the coerresponding height with the map
        idx = 0
        alpha = 0.35
        for reg_idx in land_regions_idx:

            hmap_coords = (self.graph.points[reg_idx,:]*(nres-1)).astype(int);
            self.regions[reg_idx].height = max(0,distLandWater[idx] + alpha*hmap[hmap_coords[0],hmap_coords[1]])
            self.height_vector[reg_idx] = self.regions[reg_idx].height
            idx = idx + 1

        #anotate the graph nodes with the height (of the lower region)
        for node in self.graph.nxgraph:
            if self.graph.nxgraph.node[node] != {}:
                adj_idx = self.graph.nxgraph.node[node]['adj_reg'];
                node_height = self.height_vector[adj_idx.astype(int)]
                self.graph.nxgraph.node[node]['height'] = node_height.min()
                
        #anotate the graph edges as the height difference between its connecting nodes
        for edge in self.graph.nxgraph.edges():
            if self.graph.nxgraph.node[edge[0]] != {} and self.graph.nxgraph.node[edge[1]] != {}:
                h1 = self.graph.nxgraph.node[edge[0]]['height']
                h2 = self.graph.nxgraph.node[edge[1]]['height']
                self.graph.nxgraph[edge[0]][edge[1]]['weight'] = np.max(0,h2 - h1)
                 
    def createRiver(self):

        max_height = self.height_vector.max()
        highest_vertex = 0
        
        water_vertices = []        
        #find a vertex in the water
        for node in self.graph.nxgraph:
            if self.graph.nxgraph.node[node] != {}:
                adj_idx = self.graph.nxgraph.node[node]['adj_reg'];
                adj_regions = np.array(self.regions)[adj_idx.astype(int)]
                waterRegion = True
                for r in adj_regions:
                    if r.isWater == False:
                        if r.height == max_height:
                            highest_vertex = node
                        waterRegion = False
                if waterRegion == True:
                    water_vertices.append(node)
                            

        self.path = nx.dijkstra_path(self.graph.nxgraph, highest_vertex, water_vertices[0], weight='weight')
        
        #mark regions which are neighbor with the river (moist biomes)
        self.riverside_regions_idx = []
        for i in self.path:
            if self.graph.nxgraph.node[i] != {}:
                 adj_idx = self.graph.nxgraph.node[i]['adj_reg'];
                 self.riverside_regions_idx.append(adj_idx)
                 for j in adj_idx:
                     self.regions[j.astype(int)].isRiverSide = True
        
        
        self.riverside_regions_idx = np.unique(np.hstack(np.array(self.riverside_regions_idx)))
            
        self.wv = water_vertices

    def createBiomes(self):

        snow = [248,248,248] #M: 6,5,4 H:4
        tundra = [221, 221, 187] #M:3 H: 4
        bare = [187, 187, 187] #M2: 
        scorched = [153, 153, 153] #M1
        taiga = [204, 212, 187] #M6,5 H3
        shrubland = [196, 204, 187] #M4,3 H3
        temperate_desert = [228, 232, 202] #M21 H3
        temperate_rainforest = [164, 196, 168] #M6 H2
        temperate_dec_forest = [180, 201, 169] #M4,5 H2
        grassland = [196,212,170] #M3,2 H2
        temperate_dessert = [228,232,202] #M1 H2
        tropical_rainforest = [156, 187, 169] #M6,5
        tropical_season_forest = [169,204,164] #M3,4
        subtropical_desert = [233,221,199] #M1

        self.biome_color_map = np.zeros((14,3))
        self.biome_color_map[0,:] = subtropical_desert
        self.biome_color_map[1,:] = tropical_season_forest
        self.biome_color_map[2,:] = tropical_rainforest
        self.biome_color_map[3,:] = temperate_dessert
        self.biome_color_map[4,:] = grassland
        self.biome_color_map[5,:] = temperate_dec_forest
        self.biome_color_map[6,:] = temperate_rainforest
        self.biome_color_map[7,:] = temperate_desert
        self.biome_color_map[8,:] = shrubland
        self.biome_color_map[9,:] = taiga
        self.biome_color_map[10,:] = scorched
        self.biome_color_map[11,:] = bare
        self.biome_color_map[12,:] = tundra
        self.biome_color_map[13,:] = snow

        height_bins = 4;
        moist_bins = 6;

        self.biome_map = np.ones((height_bins, moist_bins))*-1
        self.biome_map[0,0:2] = 0#'st_desert'
        self.biome_map[0,2:4] = 1#'trop_s_desert'
        self.biome_map[0,4:6] = 2#'trop_rainforest'
        self.biome_map[1,0] = 3#'temp_desert'
        self.biome_map[1,1:3] = 4#'grassland'
        self.biome_map[1,3:4] = 5#'temp_dec_forest'
        self.biome_map[1,3:6] = 6#'temp_rainforest'
        self.biome_map[2,0:2] = 7#'temp_desert'
        self.biome_map[2,2:4] = 8#'shrubland'
        self.biome_map[2,4:6] = 9#'taiga'
        self.biome_map[3,0] = 10#'scorched'
        self.biome_map[3,1] = 11#'bare'
        self.biome_map[3,2] = 12#'tundra'
        self.biome_map[3,3:6] = 13#'snow'


        #compute moist of regions as the distance to the nearest "wet area".
        #Add the riverside in the water points
        for i in self.riverside_regions_idx:
            self.water_points.append(self.graph.points[i.astype(int),:])

        distLandWater = cdist(np.array(self.graph.points), np.array(self.water_points)).min(1)
        distLandWater = distLandWater/distLandWater.max()

        h_bin_size =  self.height_vector.max()/(height_bins);
        m_bin_size = 1./(moist_bins);
        
        #assign biome to regions
        idx  = 0
        for r in self.regions:
            hbin = int(max(0,math.ceil((r.height)/h_bin_size)) - 1)
            mbin = int(math.ceil((1-distLandWater[idx])/m_bin_size) - 1)

            r.biome = self.biome_map[hbin, mbin]
            idx = idx + 1
                 
    def generateImage(self, ntiles):

            all_polys = np.vstack(self.graph.polygons);
            bottom_right = all_polys.max(0)
            top_left = all_polys.min(0)
            max_range_w = bottom_right[1] - top_left[1];
            max_range_h = bottom_right[0] - top_left[0];
            scale = [ntiles/max_range_h, ntiles/max_range_w]
            img = Image.new('RGB', (ntiles, ntiles), 0)
            idx = 0
            minpo  = np.array([ntiles,ntiles])
            maxpo = []
            
            for reg in self.regions:
                    po = (reg.polygon - top_left)*scale
                    
                    if reg.isWater:
                            ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline='blue', fill='blue')
                    elif reg.isLake:
                           ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline='blue', fill='blue')
                    # elif reg.isRiverSide:
                    #     ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline='green', fill='green')       
                    else:
                        color = self.biome_color_map[reg.biome ,:].astype(int)
                        ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline=tuple(color), fill=tuple(color))


            for idx in range(1,len(self.path)):
                v1 = (self.graph.vor.vertices[self.path[idx-1],:]  - top_left) * scale
                v2 = (self.graph.vor.vertices[self.path[idx],:]  - top_left) * scale
                            
                #add noise to the river to make it more fancy
                seed = random.random()
                river_partitions = int(np.sqrt(np.sum((v1 - v2)**2))/2)
                max_dev = 20;

                x = np.linspace(v1[0], v2[0], river_partitions)
                y = np.linspace(v1[1], v2[1], river_partitions)

                src = v1;
                
                noise = np.zeros(river_partitions)
                for i in range(1,river_partitions):
                    noise = pnoise1(float(i)/river_partitions, 5, base=int(seed*100))
                    #TODO Add noise only in the perpendicular component of the overall river direction
                    xr = int(x[i] + noise*max_dev)
                    yr = int(y[i] + noise*max_dev)
                    ImageDraw.Draw(img).line(np.hstack((src,xr,yr)).tolist(), fill='blue', width=3)
                    src = np.array([xr,yr])

                ImageDraw.Draw(img).line(np.hstack((src,v2)).tolist(), fill='blue', width=3)

            #crop image
            tl = ((self.graph.points[self.land_idx,:].min(0) - top_left)*scale).astype(int) - 20
            br = ((self.graph.points[self.land_idx,:].max(0) - top_left)*scale).astype(int) + 20
            h = br[0] - tl[0]
            w = br[1] - tl[1]

            croppedIm = img.crop((tl[0], tl[1], w, h));
            #crop image to surround the island:
                
            return croppedIm
                    
                 

            
