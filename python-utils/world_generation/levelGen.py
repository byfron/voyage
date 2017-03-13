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

    #find regions with shared vertices
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


class Corridor:

    def __init__(self, points, width):
        self.points = points;         
        self.width = width;

class Room:

    def __init__(self, regions, walls):
        self.regions = regions;
        self.walls = walls; #pairs of vertices for each wall
        
class GameLevel:

    def __init__(self, graph):
        self.graph = graph;
        self.regions = []
        self.walls =[]
        self.rooms = []
        self.available_regions = []
        self.corridors = [];

        self.create();

    def create(self):
        
        #choose the starting region randomly
        starting_region_idx = int(random.random() * self.graph.N);

        #choose it somehwre in the center
        
        for i in range(self.graph.N):
                reg = Region(self.graph.polygons[i])                
                reg.adjacent_regions = findAdjacentRegions(self.graph, i)
                self.regions.append(reg)
        
        self.available_regions = range(self.graph.N)

        room = self.createRoom(starting_region_idx);
        corridor = self.createCorridor(room)        

                
    def createCorridor(self, room):
        corridor_w = self.getRandomCorridorWidth();

        candidate_walls = []
        
        #find wall with enough size for the corridor
        for wall in room.walls:
            vwall = self.graph.vertices[wall]
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
        
        #make sure that the direction is "outwards"
        if np.linalg.norm(middle_point + corridor_dir - room_centroid) < np.linalg.norm(middle_point - corridor_dir - room_centroid):
            corridor_dir = corridor_dir * -1;
    
        
        room_neighbors = []
        #Find all neighbor regions of the room
        for reg in room.regions:
            adj_reg = findAdjacentRegions(self.graph, reg)
            #remove regions in the room
            adj_reg_clean = [x for x in adj_reg if x not in room.regions]
            room_neighbors = room_neighbors + adj_reg_clean

        for reg in room_neighbors:
            self.regions[reg].color = 'yellow'
            
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
        room_regions = [adjacent_regions[i] for i in idx_largest_edges]
        room_regions.append(region_idx)
        
        for idx in room_regions:
            self.regions[idx].color = 'blue'


        # find walls of the room. Every ridge between room regions
        # and non-room regions
        self.walls = []
        for reg in room_regions:
            vreg = self.graph.polygons[reg]
            adjacent_regions = self.regions[reg].adjacent_regions
            for adj_reg in adjacent_regions:
                if (adj_reg not in room_regions):
                    vadj_reg = self.graph.polygons[adj_reg];
                    intersect = np.intersect1d(vreg, vadj_reg)
                    if intersect.size:
                        self.walls.append(intersect)
                    
        room = Room(room_regions, self.walls)
        self.rooms.append(room)
        return room;

    def exportToFbx(self):
        # Prepare the FBX SDK.
        (lSdkManager, lScene) = FbxCommon.InitializeSdkObjects()
        self.createFbxScene(lSdkManager, lScene)

        saveScene('test', lSdkManager, lScene)
    
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
            roomMesh = FbxMesh.Create( pScene, 'Room')

            # Set the normals on Layer 0.
            lLayer = roomMesh.GetLayer(0)
            if lLayer == None:
                roomMesh.CreateLayer()
                lLayer = roomMesh.GetLayer(0)
                
            lLayerNormal= FbxLayerElementNormal.Create(roomMesh, "")
            lLayerNormal.SetMappingMode(FbxLayerElement.eByControlPoint)
            lLayerNormal.SetReferenceMode(FbxLayerElement.eDirect)

            roomMesh.InitControlPoints(len(tMesh.indices))

            idx = 0
            for idx_triangle in tMesh.indices:
                for vtri in range(3):                
                    fbxv = FbxVector4(tMesh.vertices[idx_triangle[vtri]][0],
                                      tMesh.vertices[idx_triangle[vtri]][1], 0.0)
                    roomMesh.SetControlPointAt(fbxv, idx)
                    idx = idx + 1

            for idx_triangle in tMesh.indices:
                for vtri in range(3):         
                    lLayerNormal.GetDirectArray().Add(FbxVector4(0.0, 0.0, 1.0))
                    
            idx = 0                    
            for idx_triangle in tMesh.indices:                
                roomMesh.BeginPolygon(idx)
                for vtri in range(3):
                    roomMesh.AddPolygon(idx_triangle[vtri])
                roomMesh.EndPolygon();
                idx = idx + 1
                
            lLayer.SetNormals(lLayerNormal)            
                
            roomNode = FbxNode.Create(pSdkManager, "roomNode")
            roomNode.SetNodeAttribute(roomMesh);

        pScene.GetRootNode().AddChild(roomNode)

    def convertRoomToTriangleMesh(self, room):

        mesh = TriangleMesh();
        all_indices = []
        triangles = []


        vertices = []
        idx = 0
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
                v0[0] = self.graph.vertices[i0][0]
                v0[1] = self.graph.vertices[i0][1]

                v1 = np.zeros(3)
                v1[0] = self.graph.vertices[i1][0]
                v1[1] = self.graph.vertices[i1][1]

                v2 = np.zeros(3)
                v2[0] = self.graph.vertices[i2][0]
                v2[1] = self.graph.vertices[i2][1]

                v01 = v1 - v0;
                v12 = v2 - v1;

                v01 = v01/np.linalg.norm(v01)
                v12 = v12/np.linalg.norm(v12)
                n = np.cross(v01,v12);

                if n[2] > 0:                                
                    triangles.append((idx,idx+1,idx+2))
                    vertices.append(self.graph.vertices[i0])
                    vertices.append(self.graph.vertices[i1])
                    vertices.append(self.graph.vertices[i2])
                else:
                    triangles.append((idx+2,idx+1,idx))
                    vertices.append(self.graph.vertices[i2])
                    vertices.append(self.graph.vertices[i1])
                    vertices.append(self.graph.vertices[i0])
                idx = idx + 3

        # unique_indices = list(OrderedDict.fromkeys(all_indices))
        # index_dict = dict(zip(unique_indices, range(len(unique_indices))))
        
        # mapped_triangles = []
        # for tri in triangles:        
        #     mapped_tri = [index_dict[i] for i in tri]
        #     mapped_triangles.append(mapped_tri)

        mesh.vertices = vertices #self.graph.vertices[unique_indices,:]
        mesh.indices = triangles#mapped_triangles

        
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
            vwall = self.graph.vertices[wall]
            po = (vwall - top_left) * scale
            ImageDraw.Draw(img).line(np.hstack(po.astype(int)).tolist(), fill='red', width=5)    


        cline1 =  np.vstack((self.corridors[0].points[0], self.corridors[0].points[1]))
        po1 = (cline1 - top_left) * scale
        ImageDraw.Draw(img).line(np.hstack(po1.astype(int)).tolist(), fill='green', width=5)

        # cline2 =  np.vstack((self.corridor[1], self.corridor[3]))
        # po2 = (cline2 - top_left) * scale
        # ImageDraw.Draw(img).line(np.hstack(po2.astype(int)).tolist(), fill='green', width=5)    
            
        img.show()        
            
