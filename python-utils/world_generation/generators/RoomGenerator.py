import numpy as np

#find length of the ridge between two regions (0 if not adjacent)
def findRidgeLength(graph, r1, r2):

    vidx1 = graph.polygons[r1];
    vidx2 = graph.polygons[r2];

    intersect = np.intersect1d(vidx1, vidx2)
    return np.linalg.norm(graph.vertices[intersect[0],:] - graph.vertices[intersect[1],:])


class Room:

    def __init__(self, room_id):
        self.room_id = room_id
        self.centroid = []
        self.regions = [];
        self.walls = []; #pairs of vertices for each wall
        self.floor_vertices = [];
        self.floor_indices = [];
        self.uv_coords = []

    def getRandomRoomSize(self):
        return 3;

    def create(self, region_idx, regions, graph):

        room_size = self.getRandomRoomSize();
        room_polygon = graph.polygons[region_idx];

        lengths = []
        adjacent_regions = regions[region_idx].adjacent_regions

        for adj in adjacent_regions:
            length = findRidgeLength(graph, adj, region_idx)
            lengths.append(length)

        room_size = min((room_size, len(adjacent_regions)))
        idx_largest_edges = np.argsort(lengths)[-room_size:]

        room_regions = [adjacent_regions[i] for i in idx_largest_edges if not regions[adjacent_regions[i]].isEdgeRegion]
        room_regions.append(region_idx)
        room_walls = []

        for reg in room_regions:
            regions[reg].color = 'blue'
        regions[region_idx].color = 'green'

        vertices3d = np.hstack((graph.vertices, np.zeros((graph.vertices.shape[0],1))))

        # find walls of the room. Every ridge between room regions
        # and non-room regions
        for reg in room_regions:
            vreg = graph.polygons[reg]
            cv = np.mean(graph.vertices[vreg], axis=0)

            adjacent_regions = regions[reg].adjacent_regions
            for adj_reg in adjacent_regions:
                if (adj_reg not in room_regions):
                    vadj_reg = graph.polygons[adj_reg]
                    cva = np.mean(graph.vertices[vadj_reg], axis=0);
                    intersect = np.intersect1d(vreg, vadj_reg)
                    if intersect.size:
                        iv0 = vertices3d[intersect[0]]
                        iv1 = vertices3d[intersect[1]]
                        #check if we have the region to the left or to the right
                        #to oder the wall indices properly
                        vdir = iv1 - iv0
                        vcenter = cv - cva
                        if np.cross(vdir, vcenter)[2] < 0:
                            room_walls.append(intersect)
                        else:
                            room_walls.append(np.array([intersect[1], intersect[0]]))

        self.regions = room_regions;
        self.walls = room_walls;

        # create doors in random walls
        # num_doors = 1
        # door_wall_idx = random.randint(0, len(room.walls)-1)
        # door_wall = room.walls[door_wall_idx]

        # dv1 = vertices3d[door_wall[0]]
        # dv2 = vertices3d[door_wall[1]]
        # doorv = dv2-dv1
        # middle_door = (dv1 + dv2)/2
        # door_width = 0.25
        # door_edge1 = middle_door - doorv*door_width/2
        # door_edge2 = middle_door + doorv*door_width/2
        # exit_vector = np.array([door_edge1[1], -door_edge1[0]])

        # idx_new_vertex = graph.vertices.shape[0]
        # graph.vertices = np.vstack([graph.vertices, door_edge1[0:2]])
        # graph.vertices = np.vstack([graph.vertices, door_edge2[0:2]])

        # room.walls.pop(door_wall_idx)
        # room.walls.append((door_wall[0], idx_new_vertex))
        # room.walls.append((idx_new_vertex+1, door_wall[1]))

        # self.rooms.append(room)
        # return room;
