#1. create a polygonal graph structure (voronoi)

#2. Define elevation map (acoording to elevation settings)

#2. Generate coast line (noisy radial sin waves) around the lower elevation areas

#3. Generate (if needed) internal water sources (lakes)

#4. Assign first biome proposals depending on elevation (high:rocky snowy, low: forest, coastline: beach, rocks)

#5. Refine assignment acoording to user settings (humidity, desertical etc...)

#6. Smooth transition of biomes as well as elevation

#7 Create rivers from mountain to coast through the borders of the diagram

#8. Make the polygon borders noisy

#9. Create a squared tilemap that covers the whole polygonal map

#10. Within each tile assign the biome of the intersecting polygon

#11. Add certain variability within that biome's characteristics with additional smooth perlin nois.e

import pdb
import numpy as np
import matplotlib.pyplot as plt
from noise import pnoise1, pnoise2, snoise2
import random
from scipy.spatial import Voronoi, voronoi_plot_2d
from scipy.spatial.distance import squareform, pdist, cdist
import networkx as nx
from PIL import Image, ImageDraw, ImageFilter
import math

def voronoi_finite_polygons_2d(vor, radius=None):
    """
    Reconstruct infinite voronoi regions in a 2D diagram to finite
    regions.

    Parameters
    ----------
    vor : Voronoi
        Input diagram
    radius : float, optional
        Distance to 'points at infinity'.

    Returns
    -------
    regions : list of tuples
        Indices of vertices in each revised Voronoi regions.
    vertices : list of tuples
        Coordinates for revised Voronoi vertices. Same as coordinates
        of input vertices, with 'points at infinity' appended to the
        end.

    """

    if vor.points.shape[1] != 2:
        raise ValueError("Requires 2D input")

    new_regions = []
    new_vertices = vor.vertices.tolist()

    center = vor.points.mean(axis=0)
    if radius is None:
        radius = vor.points.ptp().max()

    # Construct a map containing all ridges for a given point
    all_ridges = {}
    for (p1, p2), (v1, v2) in zip(vor.ridge_points, vor.ridge_vertices):
        all_ridges.setdefault(p1, []).append((p2, v1, v2))
        all_ridges.setdefault(p2, []).append((p1, v1, v2))

    # Reconstruct infinite regions
    for p1, region in enumerate(vor.point_region):
        vertices = vor.regions[region]

        if all(v >= 0 for v in vertices):
            # finite region
            new_regions.append(vertices)
            continue

        # reconstruct a non-finite region
        ridges = all_ridges[p1]
        new_region = [v for v in vertices if v >= 0]

        for p2, v1, v2 in ridges:
            if v2 < 0:
                v1, v2 = v2, v1
            if v1 >= 0:
                # finite ridge: already in the region
                continue

            # Compute the missing endpoint of an infinite ridge

            t = vor.points[p2] - vor.points[p1] # tangent
            t /= np.linalg.norm(t)
            n = np.array([-t[1], t[0]])  # normal

            midpoint = vor.points[[p1, p2]].mean(axis=0)
            direction = np.sign(np.dot(midpoint - center, n)) * n
            far_point = vor.vertices[v2] + direction * radius

            new_region.append(len(new_vertices))
            new_vertices.append(far_point.tolist())

        # sort region counterclockwise
        vs = np.asarray([new_vertices[v] for v in new_region])
        c = vs.mean(axis=0)
        angles = np.arctan2(vs[:,1] - c[1], vs[:,0] - c[0])
        new_region = np.array(new_region)[np.argsort(angles)]

        # finish
        new_regions.append(new_region.tolist())

    return new_regions, np.asarray(new_vertices)


# make up data points
N = 4000
points = np.random.rand(N,2)

# compute Voronoi tesselation
vor = Voronoi(points)

#from the centroid takes all polygons intersecting with a line which
#length depends on a noise function
#iterate through polygons clockwise from the centroid
Y_vector = np.array([0,1]);
vectors = points - np.array([0.5,0.5]);
norm_vectors = (vectors.T/np.sqrt(np.sum(vectors**2,1))).T
dotynv = np.dot(Y_vector, norm_vectors.T)
detynv = Y_vector[0]*norm_vectors[:,1] - norm_vectors[:,0]*Y_vector[1]
angles = np.arctan2(detynv, dotynv)
seed = random.random()
#sort in increasing angle
idx = np.argsort(angles)
base = 0.05

#generate a perlin noise vectors with the same elements as points
noise_vec = np.zeros(N)
for i in range(N):
    noise_vec[i] = pnoise1(float(i)/N, 6, base=int(seed*100))

#rescale noise
noise_vec = np.abs(noise_vec) + base;
noise_vec = noise_vec/noise_vec.max()*0.6

#compute distances to the centroid of all points
distances = np.sqrt(np.sum((points - np.array([0.5, 0.5]))**2,1))

#make inverse indexing of points-voronoi regions
idx_regions_points = np.ones(vor.point_region.max()+1)*-1
idx_n = 0
for i in range(N):
    idx_regions_points[vor.point_region[i]] = idx_n;
    idx_n = idx_n + 1

#the corresponding point is land or water if its distance to the centroid is within the noise-range
max_dist = distances.max()
land_vector = np.zeros(N)
coast_vector = np.zeros(N)
land_index = []
land_points = np.empty((0,2), float)
water_points = np.empty((0,2), float)



idx_regions_land = []
idx_regions_water = []

for i in range(N):
    if distances[idx[i]] > max_dist * noise_vec[i]:
        land_vector[idx[i]] = 0
        water_points = np.vstack([water_points, points[idx[i],:]])
        idx_regions_water.append(i)
    else:
        land_vector[idx[i]] = 1
        land_index.append(idx[i])
        land_points = np.vstack([land_points, points[idx[i],:]])
        idx_regions_land.append(i)



#point indices that are the closest to land points
distLandWater = cdist(points, water_points).min(1)
distLandWater = distLandWater/distLandWater.max()
distLandWater = distLandWater/2.;

distWaterLandIdx = cdist(land_points, water_points).argmin(1) 


        
#add one or two random lake areas
#randomly choose a node that is not coastal
idx_lake_candidates = np.where(distLandWater[land_index] > 0.04)[0]
lake_pol = random.randint(0, len(idx_lake_candidates))
lake_index = land_index[idx_lake_candidates[lake_pol]];
idx = land_index.index(lake_index)
land_vector[lake_index] = 0
water_points = np.vstack((water_points, points[lake_index,:]))

closest_lake_regions = np.argsort(cdist([points[lake_index,:]], land_points)[0])
lake_size = random.randint(3, 10)
for i in range(lake_size):
    lake_index = land_index[closest_lake_regions[i]]
    land_vector[lake_index] = 0
    water_points = np.vstack((water_points, points[lake_index,:]))

for i in range(lake_size):    
    land_index.remove(land_index[closest_lake_regions[i]])

#recompute height 
distLandWater = cdist(points, water_points).min(1)
distLandWater = distLandWater/distLandWater.max()
distLandWater = distLandWater/2.;


#pdb.set_trace()

#add some perlin noise to the non-zero elements:
#generate a low resolution height-map, and query the image from the polygon centroids



#land_points[


        

#build polygon list

#create image mask of valid map area (pixels)
coastline_points = cdist(water_points, points).min(1)
coastline_points_idx = cdist(water_points, points).argmin(1)
coastline_regions = vor.point_region[coastline_points_idx]

coastline_vertices_idx = sum([vor.regions[i] for i in coastline_regions],[]);
coastline_vertices = vor.vertices[coastline_vertices_idx]

bottom_left = vor.vertices[coastline_vertices_idx].min(0)
top_right = vor.vertices[coastline_vertices_idx].max(0)


#transform everything to tilemaps by intersecting with the cropped voronoi
min_x = vor.min_bound[0]
max_x = vor.max_bound[0]
min_y = vor.min_bound[1]
max_y = vor.max_bound[1]

#generate tilemap
NTILES = 500
TILESIZE = 16
tilemap = np.zeros([NTILES, NTILES])
A = np.meshgrid(np.arange(NTILES)/float(NTILES), np.arange(NTILES)/float(NTILES))
centroids = np.array([A[0].ravel(), A[1].ravel()]).T

#construct list of visible polygons
polygons = []

#remove the ones that exceed the point boundaries
min_x = points[:,0].min()
max_x = points[:,0].max()
min_y = points[:,1].min()
max_y = points[:,1].max()

# valid_polygons = []
# for po in polygon_list:
#     valid = 1
#     for i in range(po.shape[0]):
#         p = po[i];
#         if not(p[0] > min_x and p[0] < max_x and p[1] > min_y and p[1] < max_y):
#             valid = 0

#     if valid:    
#         valid_polygons.append(po)


#Disturb the height map with 2D perlin noise
#snoise2
#pdb.set_trace()
regions, vertices = voronoi_finite_polygons_2d(vor)

vor.regions.remove([])

# colorize
idx = 0
idx_land = 0
polygon_list = []
color_list = []
for region in regions:
    polygon = vertices[region]
    if land_vector[idx] == 0:
        plt.fill(*zip(*polygon), color='blue', alpha=0.4)
    else:
        c = distLandWater[idx]
        polygon_list.append(polygon);
        color_list.append(c)

        idx_land = idx_land+1
        col = [c,c,c]
        plt.fill(*zip(*polygon), color=col, alpha=0.4)
    idx = idx + 1

all_polys = np.vstack(polygon_list);
bottom_right = all_polys.max(0)
top_left = all_polys.min(0)
max_range_w = bottom_right[1] - top_left[1];
max_range_h = bottom_right[0] - top_left[0];
scale = [NTILES/max_range_h, NTILES/max_range_w]

#generate perlin height map
hmap = np.zeros((NTILES, NTILES))
freq = NTILES * .2
turbulence = 0.5
for i in range(NTILES):
    for j in range(NTILES):
        hmap[i,j] = pnoise2(float(i)/freq, float(j)/freq, 5, base=int(seed*100))

hmap = hmap - hmap.min()


#re-scale polygon height acoording to map
for i in range(len(polygon_list)):
    center = (polygon_list[i].mean(0) - top_left) * scale
    color_list[i] = np.sqrt(color_list[i] * hmap[int(center[0]), int(center[1])]);


#follow the edges of the voronoi diagram
#G = nx.Graph()
#G.add_node()
#G.add_nodes_from(range(vor.vertices.shape[0]))
#G.add_edges_from(vor.ridge_vertices )

    
#label biomes acoording to height/moisture
#moisture: distance to closest water
#height:

img = Image.new('L', (NTILES, NTILES), 0)
idx = 0
for polygon in polygon_list:
    po = (polygon - top_left) * scale
    c =  int(1.*color_list[idx]*255)
    ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline=c, fill=c)
    idx = idx + 1

#img2 = Image.fromarray(img + .0*255*hmap)
img.show()

#make rivers!
img_arr = np.asarray(img)
vertexMatrix = vertices
vertexPixels = ((vertexMatrix - top_left) * scale).astype(int)


#discard vertices outside?
#valid = np.logical_and(np.logical_and(vertexPixels[:,0] > 0 , vertexPixels[:,0] < 500),np.logical_and(vertexPixels[:,1] > 0 , vertexPixels[:,1] < 500))
#vertexPixels[~valid,:] = 0
#vertexHeights = img_arr[vertexPixels[:,0], vertexPixels[:,1]]

ridgeMatrix = np.asarray(vor.ridge_vertices)

#filter valid ridges
#validRidgeMatrix = ridgeMatrix[np.logical_and(np.in1d(ridgeMatrix[:,0], valid),np.in1d(ridgeMatrix[:,1], valid)),:]
#vertexHeights = img_arr[vertexPixels[valid,0], vertexPixels[valid,1]]
#highest_vertex_idx = vertexHeights.argmax()

#look for the ridges of that region
#ridges = np.where(validRidgeMatrix[:,0] == valid[highest_vertex_idx])[0]

#make all water distance 0
distLandWater[~land_vector.astype(bool)] = 0
point_river_start = np.array(distLandWater).argmax()
river_start = cdist([points[point_river_start,:]], vertices).argmin()

ridges0 = np.where(ridgeMatrix[:,0] == river_start)[0]
ridges1 = np.where(ridgeMatrix[:,1] == river_start)[0]
hit_water = False

#This does not work well! Use dijkstra instead!!
while not(hit_water):
    candidate_vertices = np.hstack((ridgeMatrix[ridges0,1], ridgeMatrix[ridges1,0]))
    vertex_heights = img_arr[vertexPixels[candidate_vertices,0], vertexPixels[candidate_vertices,1]]
        
    if ~candidate_vertices.any():
        break
    
    min_h = vertex_heights.min()
    lower_vertex = candidate_vertices[vertex_heights.argmin()]

    #paint a line from to this vertex
    v1 = vertexPixels[river_start,:]
    v2 = vertexPixels[lower_vertex,:]
    ImageDraw.Draw(img).line(np.hstack((v1,v2)).tolist(), fill=0, width=3)

    pdb.set_trace()
    #assign moist 4 levels to the regions
    
    
    #remove the ridge we've used
    used_ridge = np.logical_or(np.logical_and(ridgeMatrix[:,0] == river_start,ridgeMatrix[:,1] == lower_vertex), np.logical_and(ridgeMatrix[:,1] == river_start,ridgeMatrix[:,0] == lower_vertex))

    ridgeMatrix = np.delete(ridgeMatrix, np.where(used_ridge)[0], axis = 0)
    
    river_start = lower_vertex
    ridges0 = np.where(ridgeMatrix[:,0] == river_start)[0]
    ridges1 = np.where(ridgeMatrix[:,1] == river_start)[0]

    if min_h == 0:
        hit_water = True

img.show()


snow_color = [248,248,248] #M: 6,5,4 H:4
tundra_color = [221, 221, 187] #M:3 H: 4
bare_color = [187, 187, 187] #M2: 
scorched = [153, 153, 153] #M1
taiga = [204, 212, 187] #M6,5 H3
shrubland = [196, 204, 187] #M4,3 H3
temperate_desert = [228, 232, 202] #M21 H3
temperate_rainforest = [164, 196, 168] #M6 H2
temperate_dec_forest = [180, 201, 169] #M4,5 H2
grassland = [196,212,170] #M3,2 H2
temperate_dessert = [228,232,202] #M1 H2
tropical_rainforest = [156, 187, 169] #M6,5
tropical_season_forest = [169,204,164] #M3,4
subtropical_desert = [233,221,199] #M1









#imFiltered = im2.filter(ImageFilter.BLUR)
#imFiltered.show()

#him = Image.fromarray(np.uint8(hmap*255))
#him.show()

#plt.plot(points[:,0], points[:,1], 'ko')
#plt.xlim(vor.min_bound[0] - 0.1, vor.max_bound[0] + 0.1)
#plt.ylim(vor.min_bound[1] - 0.1, vor.max_bound[1] + 0.1)

#plt.show()

# # plot
# voronoi_plot_2d(vor)
# plt.show()
