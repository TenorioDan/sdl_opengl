import Tkinter as tk
from PIL import Image as ImagePIL
from PIL import ImageTk

class TileEditor(tk.Tk):
    def __init__(self):
        tk.Tk.__init__(self)
        #image = ImagePIL.open("images/tileset_platforms.png")
        #self.spritesheet = ImageTk.PhotoImage(image)
        self.tileFrame = tk.Frame(self, width=500, height=500)
        self.tileFrame.grid(row=0, column=0)
        self.controlFrame = tk.Frame(self, width=500, height=500)
        self.controlFrame.grid(row=0, column=10)
        #self.spritesheet = ImageTk.PhotoImage(image)
        self.num_sprintes = 4
        self.last_img = None
        #self.images = [self.subimage(64*i, 0, 64*(i+1), 48) for i in range(self.num_sprintes)]
        self.canvas = tk.Canvas(self.tileFrame, width=1920, height=1080, bg='#6495ED', scrollregion=(0, 0, 2000, 2000))
        hbar = tk.Scrollbar(self.tileFrame, orient=tk.HORIZONTAL)
        hbar.pack(side=tk.BOTTOM, fill=tk.X)
        hbar.config(command=self.canvas.xview)
        vbar = tk.Scrollbar(self.tileFrame, orient=tk.VERTICAL)
        vbar.pack(side=tk.RIGHT, fill=tk.Y)
        vbar.config(command=self.canvas.yview)
        self.canvas.config(width=1920, height=1080, xscrollcommand=hbar.set, yscrollcommand=vbar.set)
        self.canvas.pack(side=tk.LEFT, expand=True, fill=tk.BOTH)
        #self.updateimage(0)

        self.spritesheet = SpriteSheet("images/tileset_platforms.png", 10, 64, 64)
        self.spritesheet.CreateRowImages(0, 3)
        self.spritesheet.CreateRowImages(1, 9)

        for row in range(self.spritesheet.row_count):
            for i in range(len(self.spritesheet.tiles_set[row])):
                self.canvas.create_image(i * self.spritesheet.tile_width + 100, row * self.spritesheet.tile_height + 100, image=self.spritesheet.tiles_set[row][i])

    def subimage(self, l, t, r, b):
        print(l,t,r,b)
        dst = tk.PhotoImage()
        dst.tk.call(dst, 'copy', self.spritesheet, '-from', l, t, r, b, '-to', 0, 0)
        return dst

    def updateimage(self, sprite):
        self.canvas.delete(self.last_img)
        self.last_img = self.canvas.create_image(100, 100, image=self.images[sprite])
        self.after(100, self.updateimage, (sprite+1) % self.num_sprintes)

class SpriteSheet():
    def __init__(self, file_name, tile_offset, tile_width, tile_height):
        self.image = ImageTk.PhotoImage(ImagePIL.open(file_name))
        self.tile_offset = tile_offset
        self.tile_width = tile_width
        self.tile_height = tile_height
        self.tiles_set = {}
        self.row_count = 0

    def CreateRowImages(self, row, image_count):
        self.tiles_set[row] = []

        for i in range(image_count):
            l = self.tile_width * i + (self.tile_offset * i)
            t = self.tile_height * row + (self.tile_offset  * row)
            r = l + self.tile_width
            b = t + self.tile_height
            self.tiles_set[row].append(self.subimage(l, t, r, b))

        self.row_count += 1

    def subimage(self, l, t, r, b):
        print(l,t,r,b)
        dst = tk.PhotoImage()
        dst.tk.call(dst, 'copy', self.image, '-from', l, t, r, b, '-to', 0, 0)
        return dst
