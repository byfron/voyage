import pdb
from graphUtils import RegionGraph
from procWorld import World
import math
from noise import pnoise1, pnoise2, snoise2
import random
import numpy as np

N = 500
g = RegionGraph(N)
w = World(g, [])
im = w.generateImage(1200)

#Add a layer of noise to make the colors more realistic
rows, cols = im.size
freq = rows*0.4
seed = random.random()
nmap = np.zeros((rows, cols))
for i in range(rows):
    for j in range(cols):
        nmap[i,j] = pnoise2(float(i)/freq, float(j)/freq, 5, base=int(seed*100))
        
pdb.set_trace()
im.show()
im.save('test.bmp')

