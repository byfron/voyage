from PIL import Image, ImageDraw
from noise import pnoise2, snoise2
import pdb
import subprocess
from settings import Settings

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

        #wbar = self.width/10;        
        # for y in range(self.height):
        #     for x in range(self.width):
        #         if int(x/wbar)%2 == 0:
        #             self.image.putpixel((x,y), 255)
        #         else:
        #             self.image.putpixel((x,y), 0)
        
        octaves = 2
        freq = 16.0 * octaves
        for y in range(self.height):
            for x in range(self.width):
		value = int(snoise2(x / freq, y / freq, octaves) * 127.0 + 128.0)
                self.image.putpixel((x,y), value)

    def save_and_compile(self, texture_name):
        settings = Settings()
        texture_file = settings.path_textures + '/' + texture_name + '.jpg'
        texture_compiled = settings.path_textures + '/' + texture_name + '.ktx'
        self.image.save(texture_file)

        #compile texture to .ktx
        subprocess.call([settings.texture_compiler, '-f', texture_file, '-o', texture_compiled])

    def show(self):
        self.image.show()
