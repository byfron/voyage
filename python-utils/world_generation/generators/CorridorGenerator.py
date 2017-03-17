import numpy as np

class Corridor:

    def __init__(self):
        self.points = points;
        self.width = width;

    def getRandomCorridorWidth(self):
        return 0.025;

    def createCorridor(self, room, regions, graph):
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

        #cast corridor from the middle of the wall
        p1 = graph.vertices[wall[0],:];
        p2 = graph.vertices[wall[1],:];

        edge_vec = p2 - p1
        middle_point = p1 + edge_vec/2

        edge_vec = edge_vec/np.linalg.norm(edge_vec)

        corridor_p1 = middle_point - edge_vec*corridor_w/2;
        corridor_p2 = middle_point + edge_vec*corridor_w/2;
        corridor_dir = np.array([edge_vec[1], -edge_vec[0]]);

        #compute room centroid
        roomv = np.array([]);
        for reg in room.regions:
            poly = regions[reg].polygon
            regv = graph.vertices[poly,:]
            roomv = np.concatenate((roomv, regv), axis=0) if roomv.size else regv;

        room_centroid = np.mean(roomv, axis=0)
        room.centroid = np.array((room_centroid[0], room_centroid[1], 0))

        #make sure that the direction is "outwards"
        if np.linalg.norm(middle_point + corridor_dir - room_centroid) < np.linalg.norm(middle_point - corridor_dir - room_centroid):
            corridor_dir = corridor_dir * -1;


        room_neighbors = []

        #Find all neighbor regions of the room
        for reg in room.regions:
            adj_reg = findAdjacentRegions(graph, reg)
            #remove regions in the room
            adj_reg_clean = [x for x in adj_reg if (x not in room.regions and not regions[x].isEdgeRegion)]

            room_neighbors = room_neighbors + adj_reg_clean

        # for reg in room_neighbors:
        #     regions[reg].color = 'yellow'

        #Find intersecting points between corridor and neighbor regions
        corridor_points = []
        corridor_points.append(middle_point);
        middle_point1 = self.computeCorridorSegment(middle_point, corridor_dir,
                                                    room_neighbors, roomv, graph, regions)
        corridor_points.append(middle_point1)


        #self.corridors.append( Corridor(corridor_points, 2.0))

        #randomly generate a small room here


        #if we are at the edge of the world, make small prefab room and stop here


        #decide of we keep with the corridor or we go back to room

        def computeCorridorSegment(self, middle_point, corridor_dir,
                                   room_neighbors, roomv, graph, regions):

            #CAST ONLY ONE RAY WHILE COMPUTING THE CORRIDOR

            l1a = middle_point
            l1b = (middle_point + corridor_dir).tolist();
            R1l = []
            for reg in room_neighbors:
            vv = graph.vertices[regions[reg].polygon,:];
                for idx in range(vv.shape[0]-1):

                    #TODO: if corridor intersects with the same room cancel it
                    # skip walls that belong to the room itself
                    if not np.any(np.where((roomv == vv[idx+1,:]).all(axis=1))[0]):
                        l2a = vv[idx,:].tolist()
                        l2b = vv[idx+1,:].tolist()
                        R1 = find_intersection(l1a,l1b,l2a,l2b)
                        if R1: R1l.append(R1)

            #find closest intersection
            bestd = 10000;
            bestr1 = [];
            for r1 in R1l:
                d = np.linalg.norm(r1 - middle_point)
                if d < bestd:
                    bestd = d;
                    bestr1 = r1;
            return bestr1
