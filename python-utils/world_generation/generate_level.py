from utils.graphUtils import RegionGraph
from levelGen import GameLevel
from exporters.FbxExporter import Exporter

# Make voronoi diagram and start painting a "path".
# Choose some polygons as rooms and edges as corridors.

N = 250
graph = RegionGraph(N)
level = GameLevel(graph)

fbxExporter = Exporter(level)
fbxExporter.exportToFbx('test2')

level.show(1000)
