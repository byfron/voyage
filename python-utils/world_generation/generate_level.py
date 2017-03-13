from graphUtils import RegionGraph
from levelGen import GameLevel

# Make voronoi diagram and start painting a "path".
# Choose some polygons as rooms and edges as corridors.

N = 200
graph = RegionGraph(N)
level = GameLevel(graph)

level.exportToFbx()

level.show(2000)
