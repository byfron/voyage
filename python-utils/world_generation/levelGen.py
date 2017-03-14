from __future__ import division 
import random
import numpy as np
import pdb
from PIL import Image, ImageDraw
import FbxCommon
from fbx import *
from collections import OrderedDict
    
 
def saveScene(pFilename, pFbxManager, pFbxScene, pAsASCII=False ):
    ''' Save the scene using the Python FBX API '''
    exporter = FbxExporter.Create( pFbxManager, '' )

    if pAsASCII:
        #DEBUG: Initialize the FbxExporter object to export in ASCII.
        asciiFormatIndex = getASCIIFormatIndex( pFbxManager )
        isInitialized = exporter.Initialize( pFilename, asciiFormatIndex )
    else:
        isInitialized = exporter.Initialize( pFilename )

    if( isInitialized == False ):
        raise Exception( 'Exporter failed to initialize. Error returned: ' + str( exporter.GetLastErrorString() ) )

    exporter.Export( pFbxScene )

    exporter.Destroy()
    
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

#find length of the ridge between two regions (0 if not adjacent)
def findRidgeLength(graph, r1, r2):

    vidx1 = graph.polygons[r1];
    vidx2 = graph.polygons[r2];
    
    intersect = np.intersect1d(vidx1, vidx2)
    return np.linalg.norm(graph.vertices[intersect[0],:] - graph.vertices[intersect[1],:])


def computeRoomCentroid(graph, room):
    for reg in room.regions:
        pdb.set_trace()

class TriangleMesh:
    def __init__(self):
        self.vertices = []
        self.indices = []
        self.normals = []
        self.texuv = []
        
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


class Corridor:

    def __init__(self, points, width):
        self.points = points;         
        self.width = width;

class Room:

    def __init__(self, regions, walls):
        self.centroid = []
        self.regions = regions;
        self.walls = walls; #pairs of vertices for each wall
        
class GameLevel:

    def __init__(self, graph):
        self.graph = graph;
        self.regions = []
        self.walls =[]
        self.rooms = []
        self.corridors = [];

        self.create();

    def create(self):
        
        #choose the starting region randomly

        starting_region_idx = 20;

        #choose it somehwre in the center
        
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

        
        starting_region_idx = self.valid_regions[random.randint(0, len(self.valid_regions)-1)]
        room = self.createRoom(starting_region_idx);
        corridor = self.createCorridor(room)        

                
    def createCorridor(self, room):
        corridor_w = self.getRandomCorridorWidth();

        candidate_walls = []

        #find wall with enough size for the corridor
        for wall in room.walls:
            vwall = self.graph.vertices[wall,:]
            length = np.linalg.norm(self.graph.vertices[wall[0],:] - self.graph.vertices[wall[1],:])
            if length > corridor_w:
                candidate_walls.append(wall)

                
        #choose wall randomly
        wall = candidate_walls[random.randint(0, len(candidate_walls)-1)]

        #cast corridor from the middle of the wall
        p1 = self.graph.vertices[wall[0],:];
        p2 = self.graph.vertices[wall[1],:];

        edge_vec = p2 - p1
        middle_point = p1 + edge_vec/2

        edge_vec = edge_vec/np.linalg.norm(edge_vec)

        corridor_p1 = middle_point - edge_vec*corridor_w/2;
        corridor_p2 = middle_point + edge_vec*corridor_w/2;
        corridor_dir = np.array([edge_vec[1], -edge_vec[0]]);

        #compute room centroid
        roomv = np.array([]);
        for reg in room.regions:
            poly = self.regions[reg].polygon
            regv = self.graph.vertices[poly,:]
            roomv = np.concatenate((roomv, regv), axis=0) if roomv.size else regv;

        room_centroid = np.mean(roomv, axis=0)
        room.centroid = np.array((room_centroid[0], room_centroid[1], 0))
        
        #make sure that the direction is "outwards"
        if np.linalg.norm(middle_point + corridor_dir - room_centroid) < np.linalg.norm(middle_point - corridor_dir - room_centroid):
            corridor_dir = corridor_dir * -1;
    
        
        room_neighbors = []
        
        #Find all neighbor regions of the room
        for reg in room.regions:
            adj_reg = findAdjacentRegions(self.graph, reg)
            #remove regions in the room
            adj_reg_clean = [x for x in adj_reg if (x not in room.regions and not self.regions[x].isEdgeRegion)]
            
            room_neighbors = room_neighbors + adj_reg_clean

        # for reg in room_neighbors:
        #     self.regions[reg].color = 'yellow'
            
        #Find intersecting points between corridor and neighbor regions
        corridor_points = []
        corridor_points.append(middle_point);
        middle_point1 = self.computeCorridorSegment(middle_point, corridor_dir, room_neighbors, roomv)
        corridor_points.append(middle_point1)


        self.corridors.append( Corridor(corridor_points, 2.0))
        
        #randomly generate a small room here
        

        #if we are at the edge of the world, make small prefab room and stop here
        

        #decide of we keep with the corridor or we go back to room

        



            
        
        
    def computeCorridorSegment(self, middle_point, corridor_dir, room_neighbors, roomv):

        #CAST ONLY ONE RAY WHILE COMPUTING THE CORRIDOR
        
        l1a = middle_point
        l1b = (middle_point + corridor_dir).tolist();
        R1l = []
        for reg in room_neighbors:
            vv = self.graph.vertices[self.regions[reg].polygon,:];
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
                               

    def getRandomCorridorWidth(self):
        return 0.025;
        
    def getRandomRoomSize(self):
        return 3;
    

    def createPrefabRoom(self):
        pass
    
    def createRoom(self, region_idx):

        room_size = self.getRandomRoomSize();
        room_polygon = self.graph.polygons[region_idx];

        lengths = []
        adjacent_regions = self.regions[region_idx].adjacent_regions        
        
        for adj in adjacent_regions:
            length = findRidgeLength(self.graph, adj, region_idx)
            lengths.append(length)

        room_size = min((room_size, len(adjacent_regions)))
        idx_largest_edges = np.argsort(lengths)[-room_size:]


        room_regions = [adjacent_regions[i] for i in idx_largest_edges if not self.regions[adjacent_regions[i]].isEdgeRegion]        
        room_regions.append(region_idx)

        for idx in room_regions:
            self.regions[idx].color = 'blue'

        vertices3d = np.hstack((self.graph.vertices, np.zeros((self.graph.vertices.shape[0],1))))
            
        # find walls of the room. Every ridge between room regions
        # and non-room regions
        self.walls = []
        for reg in room_regions:
            vreg = self.graph.polygons[reg]
            cv = np.mean(self.graph.vertices[vreg], axis=0)

            adjacent_regions = self.regions[reg].adjacent_regions
            for adj_reg in adjacent_regions:
                if (adj_reg not in room_regions):
                    vadj_reg = self.graph.polygons[adj_reg]
                    cva = np.mean(self.graph.vertices[vadj_reg], axis=0);                    
                    intersect = np.intersect1d(vreg, vadj_reg)                                      
                    if intersect.size:
                        iv0 = vertices3d[intersect[0]]
                        iv1 = vertices3d[intersect[1]]
                        #check if we have the region to the left or to the right
                        #to oder the wall indices properly
                        vdir = iv1 - iv0
                        vcenter = cv - cva                        
                        if np.cross(vdir, vcenter)[2] > 0:
                            self.walls.append(intersect)
                        else:
                            self.walls.append(np.array([intersect[1], intersect[0]]))
                            
        room = Room(room_regions, self.walls)

        # create doors in random walls
        num_doors = 1
        door_wall_idx = random.randint(0, len(room.walls)-1)
        door_wall = room.walls[door_wall_idx]

        dv1 = vertices3d[door_wall[0]]
        dv2 = vertices3d[door_wall[1]]
        doorv = dv2-dv1
        middle_door = (dv1 + dv2)/2
        door_width = 0.25
        door_edge1 = middle_door - doorv*door_width/2
        door_edge2 = middle_door + doorv*door_width/2
        exit_vector = np.array([door_edge1[1], -door_edge1[0]])
        
        idx_new_vertex = self.graph.vertices.shape[0]
        self.graph.vertices = np.vstack([self.graph.vertices, door_edge1[0:2]])
        self.graph.vertices = np.vstack([self.graph.vertices, door_edge2[0:2]])

        room.walls.pop(door_wall_idx)
        room.walls.append((door_wall[0], idx_new_vertex))
        room.walls.append((idx_new_vertex+1, door_wall[1]))

        self.rooms.append(room)
        return room;

    def exportToFbx(self):
        # Prepare the FBX SDK.
        (lSdkManager, lScene) = FbxCommon.InitializeSdkObjects()
        self.createFbxScene(lSdkManager, lScene)

        lResult = FbxCommon.SaveScene(lSdkManager, lScene, 'test2')
#        saveScene('test', lSdkManager, lScene)
    
        lSdkManager.Destroy()

    def createFbxScene(self, pSdkManager, pScene):
        lSceneInfo = FbxDocumentInfo.Create(pSdkManager, "SceneInfo")
        lSceneInfo.mTitle = "Example scene"
        lSceneInfo.mSubject = "Illustrates the creation and animation of a deformed cylinder."
        lSceneInfo.mAuthor = "ExportScene01.exe sample program."
        lSceneInfo.mRevision = "rev. 1.0"
        lSceneInfo.mKeywords = "deformed cylinder"
        lSceneInfo.mComment = "no particular comments required."
        pScene.SetSceneInfo(lSceneInfo)
        
        sceneControlPoints = []

        # add the polygon at the bottom. Each voronoi region is convex so is very easy to create
        # a triangle mesh
        
        # walls are created with two triangles per wall and two more triangles for top
        for room in self.rooms:
            tMesh = self.convertRoomToTriangleMesh(room);
            roomMesh = FbxMesh.Create( pSdkManager, 'Room')

            # Set the normals on Layer 0.
            # lLayer = roomMesh.GetLayer(0)
            # if lLayer == None:
            #     roomMesh.CreateLayer()
            #     lLayer = roomMesh.GetLayer(0)
                
            # lLayerNormal= FbxLayerElementNormal.Create(roomMesh, "")
            # lLayerNormal.SetMappingMode(FbxLayerElement.eByControlPoint)
            # lLayerNormal.SetReferenceMode(FbxLayerElement.eDirect)

            N = len(tMesh.vertices)
            
            roomMesh.InitControlPoints(N)

            for i in range(N):
                fbxv = FbxVector4(int(tMesh.vertices[i][0]*100),
                                  int(tMesh.vertices[i][1]*100),
                                  int(tMesh.vertices[i][2]*100))
                
                roomMesh.SetControlPointAt(fbxv, i)

            # for idx_triangle in tMesh.indices:
            #     for vtri in range(3):         
            #         lLayerNormal.GetDirectArray().Add(FbxVector4(0.0, 0.0, 1.0))
                    
            for idx_triangle in tMesh.indices:                
                roomMesh.BeginPolygon()
                for vtri in range(3):
                    roomMesh.AddPolygon(idx_triangle[vtri])
                roomMesh.EndPolygon();
                
            #lLayer.SetNormals(lLayerNormal)            
                
            roomNode = FbxNode.Create(pSdkManager, "Room")
            roomNode.SetNodeAttribute(roomMesh);
            roomNode.LclScaling.Set(FbxDouble3(1.0, 1.0, 1.0))
            roomNode.LclTranslation.Set(FbxDouble3(0.0, 0.0, 0.0))
            roomNode.LclRotation.Set(FbxDouble3(0.0, 0.0, 0.0))

        pScene.GetRootNode().AddChild(roomNode)

    def convertRoomToTriangleMesh(self, room):

        mesh = TriangleMesh();
        all_indices = []
        triangles = []

        vertices3d = np.hstack((self.graph.vertices, np.zeros((self.graph.vertices.shape[0],1))))

        #gather all the necessary indices
        for reg in room.regions:

            #create triangles from polygon
            i0 = self.regions[reg].polygon[0]

            all_indices = all_indices + self.regions[reg].polygon
            
            for ipol in range(1,len(self.regions[reg].polygon)-1):
                
                i1 = self.regions[reg].polygon[ipol]
                i2 = self.regions[reg].polygon[ipol+1]

                #get the right winding order in the triangle
                v0 = np.zeros(3)
                v0[0] = vertices3d[i0][0]
                v0[1] = vertices3d[i0][1]

                v1 = np.zeros(3)
                v1[0] = vertices3d[i1][0]
                v1[1] = vertices3d[i1][1]

                v2 = np.zeros(3)
                v2[0] = vertices3d[i2][0]
                v2[1] = vertices3d[i2][1]

                v01 = v1 - v0;
                v12 = v2 - v1;

                v01 = v01/np.linalg.norm(v01)
                v12 = v12/np.linalg.norm(v12)
                n = np.cross(v01,v12);

                if n[2] > 0:                                
                    triangles.append((i0,i1,i2))
                else:
                    triangles.append((i2,i1,i0))




        wall_vertices = []
        wall_indices = []
        wall_height = 0.05
        unique_indices = list(OrderedDict.fromkeys(all_indices))                    
        floor_vertices = vertices3d[unique_indices,:]
        index_dict = dict(zip(unique_indices, range(len(unique_indices))))
        
        mapped_triangles = []
        for tri in triangles:
            mapped_tri = [index_dict[i] for i in tri]
            mapped_triangles.append(mapped_tri)
        
        wall_idx = len(unique_indices)
        wall_vertices = np.zeros((len(room.walls)*4,3))
        idx = 0

        #add the walls
        for wall in room.walls:
            v1 = vertices3d[wall[0],:]
            v2 = vertices3d[wall[1],:]
            v3 = np.copy(v1)
            v3[2] = wall_height
            v4 = np.copy(v2)
            v4[2] = wall_height

            wall_vertices[idx+0] = v1
            wall_vertices[idx+1] = v2
            wall_vertices[idx+2] = v3
            wall_vertices[idx+3] = v4
            idx = idx + 4

            #invert order dependeing on centroid
            n = np.cross(v1 - v2, room.centroid - v2)
            mapped_triangles.append((wall_idx+2,wall_idx+1,wall_idx))
            mapped_triangles.append((wall_idx+2,wall_idx+3,wall_idx+1))
#            if n[2] < 0:                        


 #           else:
  #              mapped_triangles.append((wall_idx,wall_idx+1,wall_idx+2))
   #             mapped_triangles.append((wall_idx+1,wall_idx+3,wall_idx+2))
                
            wall_idx = wall_idx + 4
    

        mesh.vertices = np.vstack((floor_vertices, wall_vertices))
        mesh.indices = mapped_triangles

        
        return mesh

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
            
