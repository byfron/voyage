from scipy.spatial import Voronoi
import random
import pdb
import numpy as np
from PIL import Image, ImageDraw
import networkx as nx

class RegionGraph:

    def __init__(self, N):
        self.N = N        
        self.createPolygons()

    def createPolygons(self):

        points = np.random.rand(self.N,2)
        vor = Voronoi(points)
        
        tmp_list = []
        tmp_region_idx = []
        tmp_point_idx = []
        for i in range(len(vor.point_region)):
            #re-order regions so that they're in the same order of the points
            idx = vor.point_region[i]            
            if vor.regions[idx] != []:                
                tmp_list.append(vor.vertices[vor.regions[idx],:])
                tmp_region_idx.append(vor.regions[idx])
                tmp_point_idx.append(i)

        self.polygons = []
        self.voronoi_regions = []
        points_idx = []
        tmp_list_v_idx2 = []
        tmp_voronoi_regions_rep = []
        idx = 0
        for i in range(len(tmp_list)):
            p = tmp_list[i]            
            v_idx = tmp_region_idx[i]
            
            if np.bitwise_and(np.bitwise_and(p[:,0] > points.min(0)[0]+0.05,
                                             p[:,0] < points.max(0)[0]-0.05),
                              np.bitwise_and(p[:,1] > points.min(0)[1]+0.05,
                                             p[:,1] < points.max(0)[1]-0.05)).all():
                self.polygons.append(p)
                self.voronoi_regions.append(v_idx)
                tmp_voronoi_regions_rep.append(idx*np.ones(len(v_idx)))  
                points_idx.append(tmp_point_idx[i])
                idx = idx + 1

        #also filter points that are outside of the crop
        self.points = points[points_idx,:]
        self.N = len(self.polygons)
        
        #indices of voronoi vertices selected
        self.vor_vertex_idx = np.unique(np.hstack(np.array(self.voronoi_regions)))
        self.nvertices = self.vor_vertex_idx.shape[0]

        #Create a graph of vertices
        ridge_matrix = np.array(vor.ridge_vertices)

        G = nx.Graph()

        tmp_voronoi_regions_rep = np.hstack(tmp_voronoi_regions_rep)
        
        #make a graph with the valid vertices and ridges
        for i in range(len(self.vor_vertex_idx)):
            v_idx = self.vor_vertex_idx[i]

            #look for the ridge
            A = ridge_matrix[np.where(ridge_matrix[:,0] == v_idx)[0],:]
            B = ridge_matrix[np.where(ridge_matrix[:,1] == v_idx)[0],:]
            edges = np.vstack((A,B))

            #anotate the node with the index of the adjacent regions/points
            adj_regions = tmp_voronoi_regions_rep[np.where(np.hstack(np.array(self.voronoi_regions)) == v_idx)[0]]
            
            G.add_node(v_idx, adj_reg=adj_regions)            
            G.add_edges_from(edges, weight=0)                           

        self.nxgraph = G
        self.vor = vor
        

    def show(self, ntiles):
        all_polys = np.vstack(self.polygons);
        bottom_right = all_polys.max(0)
        top_left = all_polys.min(0)
        max_range_w = bottom_right[1] - top_left[1];
        max_range_h = bottom_right[0] - top_left[0];
        scale = [ntiles/max_range_h, ntiles/max_range_w]
        img = Image.new('L', (ntiles, ntiles), 0)
        idx = 0
        for polygon in self.polygons:
            po = (polygon - top_left) * scale
            ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline=100, fill=0)
    
        img.show()
