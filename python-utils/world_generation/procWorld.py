import numpy as np
import pdb
import random
from noise import pnoise1, pnoise2, snoise2
from PIL import Image, ImageDraw
from scipy.spatial.distance import cdist

class Region:
    def __init__(self, polygon):
        self.moist = []
        self.height = []
        self.biome = []
        self.polygon = polygon
        self.edge_list = []
        self.isWater = False
        self.distToCenter = -1

class World:

    def __init__(self, graph, properties):
        self.graph = graph
        self.regions = []
        self.createShape()
        self.createLakes()

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
        base = 0.05

        #generate a perlin noise vectors with the same elements as points
        noise_vec = np.zeros(self.graph.N)
        for i in range(self.graph.N):
                noise_vec[i] = pnoise1(float(i)/self.graph.N, 6, base=int(seed*100))

        #rescale noise
        noise_vec = np.abs(noise_vec) + base;
        noise_vec = noise_vec/noise_vec.max()*0.6

        #compute distances to the centroid of all points
        self.distances_to_center = np.sqrt(np.sum((self.graph.points - np.array([0.5, 0.5]))**2,1))
        max_dist = self.distances_to_center.max()        

        for i in range(self.graph.N):
                reg = Region(self.graph.polygons[idx[i]])
                reg.distToCenter = self.distances_to_center[idx[i]]
                
                if self.distances_to_center[idx[i]] > max_dist * noise_vec[i]:
                        reg.isWater = True
                else:
                        reg.isWater = False

                self.regions.append(reg)
 
    def createLakes(self):

        #find lake candidates as a random region in land not very close to the sea
        idx = 0
        idx_lake_candidates = []
        for reg in self.regions:
                if reg.isWater == False & (reg.distToCenter/self.distances_to_center.max() < 1/1.2):
                    idx_lake_candidates.append(idx)
                idx = idx + 1

        lake_src = random.randint(0, len(idx_lake_candidates))
        lake_index = idx_lake_candidates[lake_src]
        closest_lake_regions = np.argsort(cdist([self.graph.points[lake_index,:]], self.graph.points))[0]
        lake_size = random.randint(3, 10)
        for i in range(lake_size):
                self.regions[closest_lake_regions[i]].isWater = True

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
                            ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline=100, fill='brown')

            img.show()
                    
                 
