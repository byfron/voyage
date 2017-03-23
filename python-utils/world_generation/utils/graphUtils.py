from scipy.spatial import Voronoi
import random
import pdb
import numpy as np
from PIL import Image, ImageDraw
import networkx as nx

#find set of adjacent indices of a region
def findAdjacentRegions(graph, reg):

    reg_vertices = graph.voronoi_regions[reg]

    neighbors = []

    #find regions with shared vertices that are NOT at the border
    for idx in range(len(graph.voronoi_regions)):
        region = graph.voronoi_regions[idx]
        intersect = np.intersect1d(region, reg_vertices)
        if (intersect.size != 0 and idx != reg):
            neighbors.append(idx)

    return neighbors

class RegionGraph:

    def __init__(self, N):
        self.N = N
        self.vertices = np.array([])
        self.createPolygons()

    def createPolygons(self):

        points = np.random.rand(self.N,2)
        vor = Voronoi(points)

        self.vertices = vor.vertices;

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



                # vlist = np.vstack(p)
                # vidx = range(self.vertices.shape[0],self.vertices.shape[0]+vlist.shape[0])
                # self.vertices = np.concatenate((self.vertices, vlist), axis=0) if self.vertices.size else vlist

                self.polygons.append(v_idx)
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

            G.add_node(v_idx, adj_reg=adj_regions.astype(int))
            G.add_edges_from(edges)

        self.nxgraph = G
        self.vor = vor


    def show(self, ntiles):
        all_vertices = np.vstack(self.vertices);
        bottom_right = all_polys.max(0)
        top_left = all_polys.min(0)
        max_range_w = bottom_right[1] - top_left[1];
        max_range_h = bottom_right[0] - top_left[0];
        scale = [ntiles/max_range_h, ntiles/max_range_w]
        img = Image.new('L', (ntiles, ntiles), 0)
        idx = 0
        for polygon in self.polygons:

            pdb.set_trace()

            po = (polygon - top_left) * scale
            ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline=100, fill=0)

        img.show()
