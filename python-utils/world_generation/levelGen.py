from __future__ import division
import random
import numpy as np
import pdb
from PIL import Image, ImageDraw
from generators.RoomGenerator import Room
from generators.CorridorGenerator import Corridor
from utils.graphUtils import findAdjacentRegions
from collections import OrderedDict

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

        num_rooms = 1
        for room_counter in range(num_rooms):
            region_idx = self.valid_regions[random.randint(0, len(self.valid_regions)-1)]
            room = Room(room_counter)
            room.create(region_idx, self.regions, self.graph, self.valid_regions)

            #remove room regions (and adjacent regions from valid vector)
            invalid_regions = room.regions
            for reg in room.regions:
                invalid_regions = invalid_regions + self.regions[reg].adjacent_regions
            invalid_regions = list(OrderedDict.fromkeys(invalid_regions))
            for inv in invalid_regions:
                if inv in self.valid_regions:
                    self.valid_regions.remove(inv)

            self.rooms.append(room)



        # corridor = Corridor()
        # corridor.create(room, self.regions, self.graph)
        # self.corridors.append(corridor)
        #self.rooms.append(corridor)


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

        # for r_idx in self.corridors[0].intersecting_regions:
        #     vertices = self.graph.vertices[self.regions[r_idx].polygon,:]
        #     po = (vertices - top_left) * scale
        #     ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline='white', fill='pink')

    # cline2 =  np.vstack((self.corridors[1], self.corridors[3]))
    #     po2 = (cline2 - top_left) * scale
    #     ImageDraw.Draw(img).line(np.hstack(po2.astype(int)).tolist(), fill='green', width=5)

        img.show()
