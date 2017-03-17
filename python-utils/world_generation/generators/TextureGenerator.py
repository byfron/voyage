from PIL import Image, ImageDraw
from noise import pnoise2, snoise2
import pdb

class Texture:

    def __init__(self):
        self.width = []
        self.height = []
        self.image = []

    def generate(self, w, h):
        self.image = Image.new('RGB', (w, h), 0)
        self.width = w
        self.height = h
        self.generateNoisyImage()

    def generateNoisyImage(self):
        octaves = 2
        freq = 16.0 * octaves
        for y in range(self.height):
            for x in range(self.width):
		value = int(snoise2(x / freq, y / freq, octaves) * 127.0 + 128.0)
                self.image.putpixel((x,y), value)

    def show(self):
        self.image.show()
