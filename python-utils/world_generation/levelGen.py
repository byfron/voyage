from __future__ import division
import random
import numpy as np
import pdb
from PIL import Image, ImageDraw
from generators.RoomGenerator import Room

def find_intersection( p0, p1, p2, p3 ) :

    s10_x = p1[0] - p0[0]
    s10_y = p1[1] - p0[1]
    s32_x = p3[0] - p2[0]
    s32_y = p3[1] - p2[1]

    denom = s10_x * s32_y - s32_x * s10_y

    if denom == 0 : return None # collinear

    denom_is_positive = denom > 0

    s02_x = p0[0] - p2[0]
    s02_y = p0[1] - p2[1]

    s_numer = s10_x * s02_y - s10_y * s02_x

    if (s_numer < 0) == denom_is_positive : return None # no collision

    t_numer = s32_x * s02_y - s32_y * s02_x

    if (t_numer < 0) == denom_is_positive : return None # no collision

    if (s_numer > denom) == denom_is_positive or (t_numer > denom) == denom_is_positive : return None # no collision


    # collision detected

    t = t_numer / denom

    intersection_point = [ p0[0] + (t * s10_x), p0[1] + (t * s10_y) ]


    return intersection_point


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

class Region:
    def __init__(self, polygon):
        self.moist = []
        self.height = 0
        self.biome = []
        self.color = (20,20,20,255)
        self.polygon = polygon
        self.edge_list = []
        self.distToCenter = -1
        self.adjacent_regions = []
        self.isEdgeRegion = False

class GameLevel:

    def __init__(self, graph):
        self.graph = graph;
        self.regions = []
        self.walls =[]
        self.rooms = []
        self.corridors = [];
        self.createWorld();

    def createWorld(self):

        for i in range(self.graph.N):
                reg = Region(self.graph.polygons[i])
                reg.adjacent_regions = findAdjacentRegions(self.graph, i)
                self.regions.append(reg)

        # count how many regions per vertex. If some missing, is edgeRegion
        self.reg_vertex_counter = np.zeros(self.graph.vertices.shape[0])
        for reg in self.regions:
            self.reg_vertex_counter[reg.polygon] = self.reg_vertex_counter[reg.polygon] + 1;

        self.valid_regions = []
        idx = 0
        for reg in self.regions:
            reg.isEdgeRegion = np.any(self.reg_vertex_counter[reg.polygon] == 1);
            if (not reg.isEdgeRegion):
                self.valid_regions.append(idx)
            idx = idx + 1

        #choose the starting region randomly
        starting_region_idx = self.valid_regions[random.randint(0, len(self.valid_regions)-1)]

        room = Room()
        room.create(starting_region_idx, self.regions, self.graph)
        self.rooms.append(room)


    def show(self, ntiles):

        all_polys = np.vstack(self.graph.vertices[np.hstack(self.graph.polygons),:]);
        bottom_right = all_polys.max(0)
        top_left = all_polys.min(0)
        max_range_w = bottom_right[1] - top_left[1];
        max_range_h = bottom_right[0] - top_left[0];
        scale = [ntiles/max_range_h, ntiles/max_range_w]
        img = Image.new('RGB', (ntiles, ntiles), 0)
        idx = 0
        for region in self.regions:
            vertices = self.graph.vertices[region.polygon,:]
            po = (vertices - top_left) * scale
            ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline='white', fill=region.color)

        for wall in self.walls:
            vwall = self.graph.vertices[wall,:]
            po = (vwall - top_left) * scale
            p1 = po[0].astype(int)
            p2 = po[1].astype(int)
            ImageDraw.Draw(img).ellipse((p1 - 5).tolist() + (p1+5).tolist(), fill='green')
            ImageDraw.Draw(img).ellipse((p2 - 5).tolist() + (p2+5).tolist(), fill='red')

           # ImageDraw.Draw(img).line(np.hstack(po.astype(int)).tolist(), fill='red', width=5)

        # cline1 =  np.vstack((self.corridors[0].points[0], self.corridors[0].points[1]))
        # po1 = (cline1 - top_left) * scale
        # ImageDraw.Draw(img).line(np.hstack(po1.astype(int)).tolist(), fill='green', width=5)

        # cline2 =  np.vstack((self.corridor[1], self.corridor[3]))
        # po2 = (cline2 - top_left) * scale
        # ImageDraw.Draw(img).line(np.hstack(po2.astype(int)).tolist(), fill='green', width=5)

        img.show()
