import pdb
from graphUtils import RegionGraph
from procWorld import World
import math


N = 1000
g = RegionGraph(N)
w = World(g, [])
w.show(500)


exit();

points = np.random.rand(N,2)
vor = Voronoi(points)


# #from the centroid takes all polygons intersecting with a line which
# #length depends on a noise function
# #iterate through polygons clockwise from the centroid
# Y_vector = np.array([0,1]);
# vectors = points - np.array([0.5,0.5]);
# norm_vectors = (vectors.T/np.sqrt(np.sum(vectors**2,1))).T
# dotynv = np.dot(Y_vector, norm_vectors.T)
# detynv = Y_vector[0]*norm_vectors[:,1] - norm_vectors[:,0]*Y_vector[1]
# angles = np.arctan2(detynv, dotynv)
# seed = random.random()
# #sort in increasing angle
# idx = np.argsort(angles)
# base = 0.05

# #generate a perlin noise vectors with the same elements as points
# noise_vec = np.zeros(N)
# for i in range(N):
#     noise_vec[i] = pnoise1(float(i)/N, 6, base=int(seed*100))

# #rescale noise
# noise_vec = np.abs(noise_vec) + base;
# noise_vec = noise_vec/noise_vec.max()*0.6

# #compute distances to the centroid of all points
# distances = np.sqrt(np.sum((points - np.array([0.5, 0.5]))**2,1))

# #make inverse indexing of points-voronoi regions
# idx_regions_points = np.ones(vor.point_region.max()+1)*-1
# idx_n = 0
# for i in range(N):
#     idx_regions_points[vor.point_region[i]] = idx_n;
#     idx_n = idx_n + 1

# #the corresponding point is land or water if its distance to the centroid is within the noise-range
# max_dist = distances.max()
# land_vector = np.zeros(N)
# coast_vector = np.zeros(N)
# land_index = []
# land_points = np.empty((0,2), float)
# water_points = np.empty((0,2), float)

# idx_regions_land = []
# idx_regions_water = []

# for i in range(N):
#     if distances[idx[i]] > max_dist * noise_vec[i]:
#         land_vector[idx[i]] = 0
#         water_points = np.vstack([water_points, points[idx[i],:]])
#         idx_regions_water.append(i)
#     else:
#         land_vector[idx[i]] = 1
#         land_index.append(idx[i])
#         land_points = np.vstack([land_points, points[idx[i],:]])
#         idx_regions_land.append(i)

# #paint an image with water and land regions

polygon_list = []
for i in range(len(vor.regions)):
    if vor.regions[i] != []:
        polygon_list.append(vor.vertices[vor.regions[i],:])

vpolygon_list = []
for i in range(len(polygon_list)):
    p = polygon_list[i]
    #p = (p - top_left)*scale

    if np.bitwise_and(np.bitwise_and(polygon_list[i][:,0] > points.min(0)[0],polygon_list[i][:,0] < points.max(0)[0]), np.bitwise_and(polygon_list[i][:,1] > points.min(0)[1],polygon_list[i][:,1] < points.max(0)[1])).all():
        vpolygon_list.append(p)

all_polys = np.vstack(vpolygon_list);
bottom_right = all_polys.max(0)
top_left = all_polys.min(0)
max_range_w = bottom_right[1] - top_left[1];
max_range_h = bottom_right[0] - top_left[0];
NTILES = 500
scale = [NTILES/max_range_h, NTILES/max_range_w]


img = Image.new('L', (NTILES, NTILES), 0)
idx = 0
for polygon in vpolygon_list:
    po = (polygon - top_left) * scale
 #   po = (polygon - top_left) * scale
#    c =  int(1.*color_list[idx]*255)
    ImageDraw.Draw(img).polygon(np.hstack(po.astype(int)).tolist(), outline=100, fill=0)

for p in range(points.shape[0]):
    p = (points[p,:] - top_left) * scale
    ImageDraw.Draw(img).ellipse((p[0], p[1], p[0]+1, p[1]+1), outline=100, fill= 100)
    
#img2 = Image.fromarray(img + .0*255*hmap)
img.show()
