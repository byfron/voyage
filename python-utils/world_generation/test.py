import pdb
from graphUtils import RegionGraph
from procWorld import World
import math
from noise import pnoise1, pnoise2, snoise2
import random
import numpy as np
import numpy.matlib
import matplotlib.pyplot as plt
from PIL import Image

N = 300
g = RegionGraph(N)
w = World(g, [])
im = w.generateImage(1200)

#Add a layer of noise to make the colors more realistic
cols, rows = im.size
freq = rows/10.
seed = random.random()
nmap = np.zeros((rows, cols, 3))
for i in range(rows):
    for j in range(cols):
        nmap[i,j,0] = pnoise2(float(i)/freq, float(j)/freq, 12, base=int(seed*1))
        nmap[i,j,1] = nmap[i,j,0]
        nmap[i,j,2] = nmap[i,j,0]


#nmap = (nmap - nmap.min())
#nmap = nmap/nmap.max()

#mask water out
npim = np.asarray(im)
water_mask = (npim == [0,0,255]).all(2)
noise_im = ~water_mask[:,:,np.newaxis]*nmap + npim

pdb.set_trace()

noise_scale = 30;
A = (nmap*noise_scale).astype('uint8') + npim

pdb.set_trace()

B = np.asarray(im);
pdb.set_trace()

plt.imshow(A)
plt.show()
     
im2 = Image.fromarray(A)
im2.show()
#im.show()
#im.save('test.bmp')

