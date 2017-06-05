import Tkinter as tk
from PIL import Image as ImagePIL
from PIL import ImageTk


# Spritesheet representation that will be used to draw tiles in the controls for selecting individual tiles
class SpriteSheet(object):
    def __init__(self, file_name, tile_offset, tile_width, tile_height):
        self.image = ImageTk.PhotoImage(ImagePIL.open(file_name))
        self.tile_offset = tile_offset
        self.tile_width = tile_width
        self.tile_height = tile_height
        self.image_set = {}
        self.row_count = 0

    # Each row will contain a specific number of images in the tileset
    def create_row_images(self, row, image_count):
        self.image_set[row] = []

        for i in range(image_count):
            l = self.tile_width * i + (self.tile_offset * i)
            t = self.tile_height * row + (self.tile_offset * row)
            r = l + self.tile_width
            b = t + self.tile_height
            self.image_set[row].append(self.subimage(l, t, r, b))

        self.row_count += 1

    # Isolate each image in the spritesheet based on its position coordinates
    def subimage(self, l, t, r, b):
        dst = tk.PhotoImage()
        dst.tk.call(dst, 'copy', self.image, '-from', l, t, r, b, '-to', 0, 0)
        return dst


# Tile object that gets created when tiles are added to the spaces in the editor.
# Contains the position of the tile, the type of tile, the image representing the tile object in the editor
class Tile(object):
    def __init__(self, row, column):
        self.tile_type = 0
        self.is_destructible = False
        self.is_false_tile = False
        self.canvas_image = None
        self.row = row
        self.column = column


class Collider(object):
    def __init__(self, min_x, min_y, max_x, max_y):
        self.min_x = min_x
        self.min_y = min_y
        self.max_x = max_x
        self.max_y = max_y
        self.rect = None

    def add_rect(self, rect):
        self.rect = rect
