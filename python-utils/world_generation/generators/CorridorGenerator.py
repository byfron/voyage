import numpy as np
from generators.RoomGenerator import Room
from utils.graphUtils import findAdjacentRegions
from utils.geometryUtils import *
import random
import pdb

class Corridor(Room):

    def __init__(self):
        self.width = self.getRandomCorridorWidth()
        self.points = []
        self.intersecting_regions = []

    def getRandomCorridorWidth(self):
        return 0.025;

    def create(self, room, regions, graph):
        corridor_w = self.getRandomCorridorWidth();

        candidate_walls = []

        #find wall with enough size for the corridor
        for wall in room.walls:
            vwall = graph.vertices[wall,:]
            length = np.linalg.norm(graph.vertices[wall[0],:] - graph.vertices[wall[1],:])
            if length > corridor_w:
                candidate_walls.append(wall)

        #choose wall randomly
        wall = candidate_walls[random.randint(0, len(candidate_walls)-1)]

        #cast corridor from the middle of the wall, outwards
        p1 = graph.vertices[wall[0],:];
        p2 = graph.vertices[wall[1],:];
        edge_vec = p1 - p2
        middle_point = p2 + edge_vec/2
        edge_vec = edge_vec/np.linalg.norm(edge_vec)
        corridor_p1 = middle_point - edge_vec*corridor_w/2;
        corridor_p2 = middle_point + edge_vec*corridor_w/2;
        corridor_dir = np.array([edge_vec[1], -edge_vec[0]]);

        self.points.append(middle_point)
        self.points.append(middle_point + corridor_dir*2)

        corridor_segment = np.array([middle_point, middle_point + corridor_dir*2])

        #mark regions intersecting the corridor as "used"

        for p_idx in range(len(graph.polygons)):
            pvertices = graph.vertices[graph.polygons[p_idx],:]
            if is_intersecting_segment_polygon(corridor_segment, pvertices):
                self.intersecting_regions.append(p_idx)

        pdb.set_trace()



        #choose the first available region of the corridor




    def computeCorridorSegment(self, middle_point, corridor_dir,
                               room_neighbors, roomv, graph, regions):

        #CAST ONLY ONE RAY WHILE COMPUTING THE CORRIDOR

        l1a = middle_point
        l1b = (middle_point + corridor_dir)
        R1l = []
        for reg in room_neighbors:
            vv = graph.vertices[regions[reg].polygon,:];
            for idx in range(vv.shape[0]-1):

                #TODO: if corridor intersects with the same room cancel it
                # skip walls that belong to the room itself
                if not np.any(np.where((roomv == vv[idx+1,:]).all(axis=1))[0]):
                    l2a = vv[idx,:]
                    l2b = vv[idx+1,:]
                    R1 = find_intersection(l1a,l1b,l2a,l2b)
                    if R1.any(): R1l.append(R1)

        #find closest intersection
        bestd = 10000;
        bestr1 = [];
        for r1 in R1l:
            d = np.linalg.norm(r1 - middle_point)
            if d < bestd:
                bestd = d;
                bestr1 = r1;
        return bestr1
