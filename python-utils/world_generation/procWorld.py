import numpy as np
import pdb
import random
from noise import pnoise1, pnoise2, snoise2
from PIL import Image, ImageDraw
from scipy.spatial.distance import cdist
import networkx as nx

class Region:
    def __init__(self, polygon):
        self.moist = []
        self.height = 0
        self.biome = []
        self.polygon = polygon
        self.edge_list = []
        self.isWater = False
        self.isLake = False
        self.distToCenter = -1

class World:

    def __init__(self, graph, properties):
        self.graph = graph
        self.regions = []
        self.createShape()
        self.createLakes()
        self.createHeight()
        self.createRiver()


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
        base = 0.07

        #generate a perlin noise vectors with the same elements as points
        noise_vec = np.zeros(self.graph.N)
        for i in range(self.graph.N):
                noise_vec[i] = pnoise1(float(i)/self.graph.N, 8, base=int(seed*100))

        #rescale noise
        noise_vec = np.abs(noise_vec) + base;
        noise_vec = noise_vec/noise_vec.max()*0.7

        #compute distances to the centroid of all points
        self.distances_to_center = np.sqrt(np.sum((self.graph.points - np.array([0.5, 0.5]))**2,1))
        max_dist = self.distances_to_center.max()        

        self.regions = [None]*self.graph.N
        self.land_points = []
        self.water_points = []
        for i in range(self.graph.N):
                reg = Region(self.graph.polygons[idx[i]])
                reg.distToCenter = self.distances_to_center[idx[i]]
                
                if self.distances_to_center[idx[i]] > max_dist * noise_vec[i]:
                    reg.isWater = True
                else:
                    reg.isWater = False

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
        land_points = []
        water_points = []
        self.height_vector = np.zeros(len(self.regions))
        for reg in self.regions:
            if reg.isWater == False and reg.isLake == False:
                land_points.append(self.graph.points[idx,:])
                land_regions_idx.append(idx)                
            else:
                water_points.append(self.graph.points[idx,:])
                water_regions_idx.append(idx)
            idx = idx + 1

        #measure distances from land to water
        distLandWater = cdist(np.array(land_points), np.array(water_points)).min(1)

        #compute height of each region acoording to the distance
        distLandWater = distLandWater/distLandWater.max()
        distLandWater = distLandWater/2.;

        idx = 0
        for reg_idx in land_regions_idx:
            self.regions[reg_idx].height = distLandWater[idx]
            self.height_vector[reg_idx] = distLandWater[idx]
            idx = idx + 1

        #anotate the graph nodes with the height (of the lower region)
        for node in self.graph.nxgraph:
            if self.graph.nxgraph.node[node] != {}:
                adj_idx = self.graph.nxgraph.node[node]['adj_reg'];
                node_height = self.height_vector[adj_idx.astype(int)]
                self.graph.nxgraph.node[node]['height'] = node_height.min()
                
        #anotate the graph edges as the height difference between its connecting nodes
        #going_up = positive weight
        for edge in self.graph.nxgraph.edges():
            h1 = self.graph.nxgraph.node[edge[0]]['height']
            h2 = self.graph.nxgraph.node[edge[1]]['height']
            self.graph.nxgraph[edge[0]][edge[1]]['weight'] = h2 - h1
                 
    def createRiver(self):

        #choose a high region
        centric_regions = np.argsort(self.height_vector)

        water_vertices = []        
        #find a vertex in the water
        for node in self.graph.nxgraph:
            if self.graph.nxgraph.node[node] != {}:
                adj_idx = self.graph.nxgraph.node[node]['adj_reg'];
                adj_regions = np.array(self.regions)[adj_idx.astype(int)]
                waterRegion = True
                for r in adj_regions:
                    if r.isWater == False:
                        waterRegion = False
                if waterRegion == True:
                    water_vertices.append(node)
                            

        #find the source vectex of the river
        dist_vert_center = np.sqrt(np.sum((self.graph.vor.vertices - np.array([.5,.5]))**2,1))
        center_node = np.argmin(dist_vert_center)

        self.path = nx.dijkstra_path(self.graph.nxgraph,center_node, water_vertices[0], weight='weight')
                    
        self.wv = water_vertices
        
                 
    def show(self, ntiles):

            all_polys = np.vstack(self.graph.polygons);
            bottom_right = all_polys.max(0)
            top_left = all_polys.min(0)
            max_range_w = bottom_right[1] - top_left[1];
            max_range_h = bottom_right[0] - top_left[0];
            scale = [ntiles/max_range_h, ntiles/max_range_w]
            img = Image.new('RGB', (ntiles, ntiles), 0)
            idx = 0
            
            for reg in self.regions:
                    po = (reg.polygon - top_left)*scale
                    
                    if reg.isWater:
                            ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline=100, fill='blue')
                    else:
                        if reg.isLake:
                           ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline=100, fill='cyan')
                        else:
                            color = np.array([reg.height*255,reg.height*255,reg.height*255]).astype(int)
                            ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline=100, fill=tuple(color))


            for idx in range(1,len(self.path)):
                v1 = (self.graph.vor.vertices[self.path[idx-1],:]  - top_left) * scale
                v2 = (self.graph.vor.vertices[self.path[idx],:]  - top_left) * scale
                ImageDraw.Draw(img).line(np.hstack((v1,v2)).tolist(), fill='cyan', width=3)
                            
            # for v in self.wv:
            #     v = (v - top_left)*scale
            #     ImageDraw.Draw(img).ellipse((v[0],v[1],v[0]+10,v[1]+10), outline=100, fill='black')
              
            img.show()
                    
                 
