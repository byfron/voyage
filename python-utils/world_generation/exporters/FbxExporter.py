import FbxCommon
from fbx import *
import numpy as np
from collections import OrderedDict
from PIL import Image, ImageDraw
from generators.TextureGenerator import Texture
from settings import Settings
import pdb

class RoomTriangleMesh:
    def __init__(self):
        self.floor_vertices = []
        self.floor_indices = []
        self.normals = []
        self.floor_indices = []
        self.wall_indices = []
        self.wall_vecs = []
        self.texuv = []
        self.collision_vertices = []
        self.collision_indices = []

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

class Exporter:

    def __init__(self, level):
        self.vertices = level.graph.vertices
        self.regions = level.regions
        self.rooms = level.rooms
        self.material_counter = 0
        self.tex_scale = 1000
        self.mesh_scale = 20

    def generateWallTextureName(self):
        return 'room_wall_tex' + str(self.material_counter)

    def generateFloorTextureName(self):
        return 'room_floor_tex' + str(self.material_counter)

    def exportToFbx(self, filename):
        (lSdkManager, lScene) = FbxCommon.InitializeSdkObjects()
        self.createFbxScene(lSdkManager, lScene)
        lResult = FbxCommon.SaveScene(lSdkManager, lScene, filename)
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
            floorMesh = FbxMesh.Create( pSdkManager, 'RoomFloor' + str(room.room_id))
            wallMesh = FbxMesh.Create( pSdkManager, 'RoomWalls' + str(room.room_id))

            #######################################################
            #center the room at the mean. Remove this later
            av = np.mean(tMesh.floor_vertices, axis=0)
            av[2] = 0
            tMesh.floor_vertices = tMesh.floor_vertices - av
            tMesh.wall_vertices = tMesh.wall_vertices - av
            tMesh.collision_vertices = tMesh.collision_vertices - av
            #######################################################

            self.createMesh(floorMesh, tMesh.floor_vertices, tMesh.floor_indices)
            self.createMesh(wallMesh, tMesh.wall_vertices, tMesh.wall_indices)

            all_vertices = np.vstack([tMesh.floor_vertices, tMesh.wall_vertices]);

            floorNode = FbxNode.Create(pSdkManager, "RoomFloor" + str(room.room_id))
            floorNode.SetNodeAttribute(floorMesh);
            floorNode.LclScaling.Set(FbxDouble3(1.0, 1.0, 1.0))
            floorNode.LclTranslation.Set(FbxDouble3(0.0, 0.0, 0.0))
            floorNode.LclRotation.Set(FbxDouble3(0.0, 0.0, 0.0))
            floorNode.SetShadingMode(FbxNode.eTextureShading)

            collisionNode = self.createCollisionNode(pSdkManager, tMesh)
            floorNode.AddChild(collisionNode)

            wallNode = FbxNode.Create(pSdkManager, "RoomWall" + str(room.room_id))
            wallNode.SetNodeAttribute(wallMesh);
            wallNode.LclScaling.Set(FbxDouble3(1.0, 1.0, 1.0))
            wallNode.LclTranslation.Set(FbxDouble3(0.0, 0.0, 0.0))
            wallNode.LclRotation.Set(FbxDouble3(0.0, 0.0, 0.0))
            wallNode.SetShadingMode(FbxNode.eTextureShading)

            pScene.GetRootNode().AddChild(floorNode)
            pScene.GetRootNode().AddChild(wallNode)

            self.createFloorMaterial(pSdkManager, floorMesh,
                                     tMesh.floor_vertices, tMesh.floor_indices)

            self.createWallMaterial(pSdkManager, wallMesh, tMesh.wall_vecs,
                                    tMesh.wall_vertices, tMesh.wall_indices)


            # TODO: add as polygons as well the convex polys of the room? much easier
            # to check locations!!



    def createCollisionNode(self, pSdkManager, tMesh):
        #create a collision node for each collision polygon
        collisionNode = FbxNode.Create(pSdkManager, "RoomCollision")
        collisionMesh = FbxMesh.Create(pSdkManager, 'Polygon')

        idx_col_vert = 0
        for col_vert in tMesh.collision_vertices:
            fbxv = FbxVector4(col_vert[0]*self.mesh_scale,
                              col_vert[1]*self.mesh_scale,
                              col_vert[2]*self.mesh_scale)
            collisionMesh.SetControlPointAt(fbxv, idx_col_vert)
            idx_col_vert = idx_col_vert + 1

        for col_indices in tMesh.collision_indices:
            collisionMesh.BeginPolygon()
            for idx in col_indices:
                collisionMesh.AddPolygon(idx)
                collisionMesh.EndPolygon()

        collisionNode.SetNodeAttribute(collisionMesh)
        return collisionNode



    def createMesh(self, mesh, vertices, indices):

        #Set the normals on Layer 0.
        lLayer = mesh.GetLayer(0)
        if lLayer == None:
            mesh.CreateLayer()
            lLayer = mesh.GetLayer(0)

        lLayerNormal= FbxLayerElementNormal.Create(mesh, "")
        lLayerNormal.SetMappingMode(FbxLayerElement.eByControlPoint)
        lLayerNormal.SetReferenceMode(FbxLayerElement.eDirect)

        N = len(vertices)
        mesh.InitControlPoints(N)

        for i in range(N):
            fbxv = FbxVector4(vertices[i][0]*self.mesh_scale,
                              vertices[i][1]*self.mesh_scale,
                              vertices[i][2]*self.mesh_scale)

            lLayerNormal.GetDirectArray().Add(FbxVector4(0.0, 0.0, -1.0))
            mesh.SetControlPointAt(fbxv, i)

        lLayer.SetNormals(lLayerNormal)

        # add floor polygon indices and material
        idx_vertex = 0
        for idx_triangle in indices:
            mesh.BeginPolygon(self.material_counter) #put here material ID
            for vtri in range(3):
                mesh.AddPolygon(idx_triangle[vtri])
                idx_vertex = idx_vertex + 1
            mesh.EndPolygon();

    def createFloorMaterial(self, pSdkManager, mesh, vertices, indices):

        lLayer = mesh.GetLayer(0)
        UVLayer = FbxLayerElementUV.Create(mesh, "DiffuseUV")
        UVLayer.SetMappingMode(FbxLayerElement.eByPolygonVertex)
        UVLayer.SetReferenceMode(FbxLayerElement.eIndexToDirect)
        lLayer.SetUVs(UVLayer, FbxLayerElement.eTextureDiffuse)

        texture_name = self.generateFloorTextureName()

        self.generateFloorTexture(texture_name, vertices, UVLayer)
        UVLayer.GetIndexArray().SetCount(vertices.shape[0]) #as many UV coordinate as vertices

        #add floor polygon indices and material
        idx_vertex = 0
        for idx_triangle in indices:
            for vtri in range(3):
                UVLayer.GetIndexArray().SetAt(idx_vertex, idx_triangle[vtri])
                idx_vertex = idx_vertex + 1

        self.createStandardMaterial(pSdkManager, mesh, self.material_counter,
                                    texture_name + '.ktx')

        self.material_counter = self.material_counter  + 1


    def createWallMaterial(self, pSdkManager, mesh, wall_vecs, vertices, indices):

        lLayer = mesh.GetLayer(0)
        UVLayer = FbxLayerElementUV.Create(mesh, "DiffuseUV")
        UVLayer.SetMappingMode(FbxLayerElement.eByPolygonVertex)
        UVLayer.SetReferenceMode(FbxLayerElement.eIndexToDirect)
        lLayer.SetUVs(UVLayer, FbxLayerElement.eTextureDiffuse)

        texture_name = self.generateWallTextureName()

        self.generateWallTexture(texture_name, wall_vecs, vertices, UVLayer)
        UVLayer.GetIndexArray().SetCount(vertices.shape[0]) #as many UV coordinate as vertices

        #add floor polygon indices and material
        idx_vertex = 0
        for idx_triangle in indices:
            for vtri in range(3):
                UVLayer.GetIndexArray().SetAt(idx_vertex, idx_triangle[vtri])
                idx_vertex = idx_vertex + 1

        self.createStandardMaterial(pSdkManager, mesh, self.material_counter,
                                    texture_name + '.ktx')

        self.material_counter = self.material_counter  + 1

    def createStandardMaterial(self, pSdkManager, mesh, material_id, texture_file):

        lLayer = mesh.GetLayer(0)
        if lLayer == None:
            mesh.CreateLayer()
        lLayer = mesh.GetLayer(0)

        lMaterial = None

        # get the node of mesh, add material for it.
        lNode = mesh.GetNode()
        if lNode:
            lMaterial = lNode.GetSrcObject(FbxCriteria.ObjectType(FbxSurfacePhong.ClassId), material_id)
            if not lMaterial:
                lMaterialName = "toto"
                lShadingName  = "Phong"
                lBlack = FbxDouble3(0.0, 0.0, 0.0)
                lRed = FbxDouble3(1.0, 0.0, 0.0)
                lDiffuseColor = FbxDouble3(0.75, 0.75, 0.0)
                lMaterial = FbxSurfacePhong.Create(pSdkManager, lMaterialName)

                # Generate primary and secondary colors.
                lMaterial.Emissive.Set(lBlack)
                lMaterial.Ambient.Set(lRed)
                lMaterial.AmbientFactor.Set(1.)
                # Add texture for diffuse channel
                lMaterial.Diffuse.Set(lDiffuseColor)
                lMaterial.DiffuseFactor.Set(1.)
                lMaterial.TransparencyFactor.Set(0.4)
                lMaterial.ShadingModel.Set(lShadingName)
                lMaterial.Shininess.Set(0.5)
                lMaterial.Specular.Set(lBlack)
                lMaterial.SpecularFactor.Set(0.3)

        lTexture = FbxFileTexture.Create(pSdkManager,"Diffuse Texture")
        # Set texture properties.
        lTexture.SetFileName(texture_file) # Resource file is in current directory.
        lTexture.SetTextureUse(FbxTexture.eStandard)
        lTexture.SetMappingType(FbxTexture.eUV)
        lTexture.SetMaterialUse(FbxFileTexture.eModelMaterial)
        lTexture.SetSwapUV(False)
        lTexture.SetTranslation(0.0, 0.0)
        lTexture.SetScale(1.0, 1.0)
        lTexture.SetRotation(0.0, 0.0)

        #connect tex. with material
        if lMaterial:
            lMaterial.Diffuse.ConnectSrcObject(lTexture)
            lNode.AddMaterial(lMaterial)


    def generateFloorTexture(self, texture_name, vertices, lUVDiffuseLayer):

        #get image
        im = Image.open("textures/floor.jpg")
        imsize = im.size;

        #get room square
        minrv = np.min(vertices, axis=0)
        maxrv = np.max(vertices, axis=0)
        top_left = minrv[0:2]*self.tex_scale
        bottom_right = maxrv[0:2]*self.tex_scale
        room_height = bottom_right[0] - top_left[0]
        room_width = bottom_right[1] - top_left[1]

        #re-self.tex_scale the image to match the room
        factor = 1
        if room_height > room_width:
            factor = room_height/imsize[0]
        else:
            factor = room_width/imsize[1]

        im = im.resize((int(imsize[0]*factor)+1,int(imsize[1]*factor)+1), Image.ANTIALIAS)
        imsize = im.size;

        #compute UV coordinates for each vertex
        for v in vertices:
            coords_world = (v - minrv)*self.tex_scale
            coords_world[0] = 1 - coords_world[0]/room_height
            coords_world[1] = 1 - coords_world[1]/room_width
            uvVec = FbxVector2(coords_world[1], coords_world[0])
            lUVDiffuseLayer.GetDirectArray().Add(uvVec)

    def generateWallTexture(self, texture_name, wall_vecs, vertices, lUVDiffuseLayer):

        #add wall vertices
        #create a plane texture of the right size
        wall_height = 0.025
        wall_width = 0.01
        minrv = np.min(vertices, axis=0)
        top_left = minrv[0:2]*self.tex_scale
        #measure length of wall
        total_wall_length = 0
        for wall in wall_vecs:
            total_wall_length = total_wall_length + np.linalg.norm(wall);

        texture_height = int((wall_height + wall_width)*self.tex_scale)
        texture_width = int(total_wall_length*self.tex_scale)

        #wall vertices are order as:
        #1-4 wall vertices (bottom1, bottom2, top1, top2)
        #5-6 top vertices
        #7... next wall
        coord_length = 0.0
        coord = np.array([0.,0.])

        tex_ratio_height = wall_height/(wall_height + wall_width)

        for wall in wall_vecs:

            wall_length = np.linalg.norm(wall)

            # botom1
            coord[0] = coord_length;
            coord[1] = 0.0;
            uvVec = FbxVector2(coord[1], coord[0])
            lUVDiffuseLayer.GetDirectArray().Add(uvVec)
            print(uvVec)

            #bottom 2
            coord[0] = coord_length + wall_length/total_wall_length;
            coord[1] = 0.0;
            uvVec = FbxVector2(coord[1], coord[0])
            lUVDiffuseLayer.GetDirectArray().Add(uvVec)
            print(uvVec)
            
            #top1
            coord[0] = coord_length;
            coord[1] = tex_ratio_height
            uvVec = FbxVector2(coord[1], coord[0])
            lUVDiffuseLayer.GetDirectArray().Add(uvVec)
            print(uvVec)
            
            #top2
            coord[0] = coord_length + wall_length/total_wall_length;
            coord[1] = tex_ratio_height
            uvVec = FbxVector2(coord[1], coord[0])
            lUVDiffuseLayer.GetDirectArray().Add(uvVec)
            print(uvVec)
            
            #top1 (ceiling)
            coord[0] = coord_length;
            coord[1] = 1.0
            uvVec = FbxVector2(coord[1], coord[0])
            lUVDiffuseLayer.GetDirectArray().Add(uvVec)
            print(uvVec)
            
            #top2 (ceiling)
            coord[0] = coord_length + wall_length/total_wall_length;
            coord[1] = 1.0
            uvVec = FbxVector2(coord[1], coord[0])
            lUVDiffuseLayer.GetDirectArray().Add(uvVec)
            print(uvVec)
            
            coord_length = coord_length + wall_length/total_wall_length;

        texture = Texture()
        texture.generate(texture_width, texture_height)

        #save in texture folder and transform to ktx
        #texture.
        texture.save_and_compile(texture_name);

    #TODO def createFloorMesh
    def convertRoomToTriangleMesh(self, room):

        mesh = RoomTriangleMesh();
        mesh.collision_polygons = []
        all_indices = []
        triangles = []

        vertices3d = np.hstack((self.vertices, np.zeros((self.vertices.shape[0],1))))

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
        wall_height = -0.025
        wall_width = 0.01
        unique_indices = list(OrderedDict.fromkeys(all_indices))
        floor_vertices = vertices3d[unique_indices,:]
        index_dict = dict(zip(unique_indices, range(len(unique_indices))))

        mapped_triangles = []
        for tri in triangles:
            mapped_tri = [index_dict[i] for i in tri]
            mapped_triangles.append(mapped_tri)

        room.floor_indices = mapped_triangles;

        wall_idx = 0#len(unique_indices)
        wall_vertices = np.zeros((len(room.walls)*6,3))
        idx = 0
        col_idx = 0

        wall_idx_matrix = np.vstack([room.walls])

        mesh.floor_indices = mapped_triangles
        mesh.wall_vecs = []

        mapped_triangles = []

        #add the walls
        for wall in room.walls:
            v1 = vertices3d[wall[0],:]
            v2 = vertices3d[wall[1],:]

            mesh.wall_vecs.append(v2 - v1)

            v3 = np.copy(v1)
            v3[2] = wall_height
            v4 = np.copy(v2)
            v4[2] = wall_height

            wall_vertices[idx+0] = v1
            wall_vertices[idx+1] = v2
            wall_vertices[idx+2] = v3
            wall_vertices[idx+3] = v4

            mapped_triangles.append((wall_idx, wall_idx+1, wall_idx+2))
            mapped_triangles.append((wall_idx+1, wall_idx+3, wall_idx+2))

            #add vertices of the up outer-wall
            wall_vec = v2 - v1;
            wall_adj_to_start = np.where(wall_idx_matrix[:,1] == wall[0])
            wall_adj_to_end = np.where(wall_idx_matrix[:,0] == wall[1])
            wall_start = room.walls[wall_adj_to_start[0][0]]
            wall_end = room.walls[wall_adj_to_end[0][0]]
            wall_adj_vec0 = vertices3d[wall_start[1]] - vertices3d[wall_start[0]]
            wall_adj_vec1 = vertices3d[wall_end[1]] - vertices3d[wall_end[0]]
            edge_vec_start = (wall_adj_vec0 + wall_vec)/2.0
            edge_vec_end = (wall_adj_vec1 + wall_vec)/2.0
            normal_start =  np.array([edge_vec_start[1], -edge_vec_start[0]])
            normal_start = normal_start/np.linalg.norm(normal_start)
            normal_end =  np.array([edge_vec_end[1], -edge_vec_end[0]])
            normal_end = normal_end/np.linalg.norm(normal_end)

            wall_vertices[idx+4] = v3 + np.hstack((normal_start*wall_width, 0))
            wall_vertices[idx+5] = v4 + np.hstack((normal_end*wall_width, 0))

            mapped_triangles.append((wall_idx+4, wall_idx+3, wall_idx+2))
            mapped_triangles.append((wall_idx+4, wall_idx+5, wall_idx+3))

            #add the wall polygon as metadata into the FBX file
            mesh.collision_vertices.append(wall_vertices[idx+2])
            mesh.collision_vertices.append(wall_vertices[idx+3])
            mesh.collision_vertices.append(wall_vertices[idx+4])
            mesh.collision_vertices.append(wall_vertices[idx+5])
            mesh.collision_indices.append((col_idx, col_idx+1, col_idx+2, col_idx+3))

            col_idx = col_idx + 4
            wall_idx = wall_idx + 6
            idx = idx + 6

        mesh.wall_indices = mapped_triangles
        mesh.floor_vertices = floor_vertices
        mesh.wall_vertices = wall_vertices

        return mesh
