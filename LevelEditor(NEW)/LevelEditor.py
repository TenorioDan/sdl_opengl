import Tkinter as tk
from PIL import Image as ImagePIL
from PIL import ImageTk

from Tkinter import *
# root = Tk()
# frame = Frame(root, width=500, height=500)
# frame.grid(row=0, column=0)
# controlsFrame = Frame(root, width=500, height=1000)
# controlsFrame.grid(row=0, column=10)
# canvas = Canvas(frame, bg='#6495ED', width=100, height=100, scrollregion=(0, 0, 2000, 2000))
# hbar = Scrollbar(frame, orient=HORIZONTAL)
# hbar.pack(side=BOTTOM, fill=X)
# hbar.config(command=canvas.xview)
# vbar = Scrollbar(frame, orient=VERTICAL)
# vbar.pack(side=RIGHT, fill=Y)
# vbar.config(command=canvas.yview)
# canvas.config(width=1920, height=1080, xscrollcommand=hbar.set, yscrollcommand=vbar.set)
# canvas.pack(side=LEFT, expand=True, fill=BOTH)
#
# image = Image.open("images/tileset_platforms.png")
# spritesheet = ImageTk.PhotoImage(image)
# numSprites = 4
# lastImage = None
#
# root.mainloop()

class App(tk.Tk):
    def __init__(self):
        tk.Tk.__init__(self)
        image = ImagePIL.open("images/tileset_platforms.png")
        self.spritesheet = ImageTk.PhotoImage(image)
        self.tileFrame = Frame(self, width=500, height=500)
        self.tileFrame.grid(row=0, column=0)
        self.controlFrame = Frame(self, width=500, height=500)
        self.controlFrame.grid(row=0, column=10)
        self.spritesheet = ImageTk.PhotoImage(image)
        self.num_sprintes = 4
        self.last_img = None
        self.images = [self.subimage(64*i, 0, 64*(i+1), 48) for i in range(self.num_sprintes)]
        self.canvas = tk.Canvas(self.tileFrame, width=1920, height=1080, bg='#6495ED', scrollregion=(0, 0, 2000, 2000))
        hbar = Scrollbar(self.tileFrame, orient=HORIZONTAL)
        hbar.pack(side=BOTTOM, fill=X)
        hbar.config(command=self.canvas.xview)
        vbar = Scrollbar(self.tileFrame, orient=VERTICAL)
        vbar.pack(side=RIGHT, fill=Y)
        vbar.config(command=self.canvas.yview)
        self.canvas.config(width=1920, height=1080, xscrollcommand=hbar.set, yscrollcommand=vbar.set)
        self.canvas.pack(side=LEFT, expand=True, fill=BOTH)
        self.updateimage(0)

    def subimage(self, l, t, r, b):
        print(l,t,r,b)
        dst = tk.PhotoImage()
        dst.tk.call(dst, 'copy', self.spritesheet, '-from', l, t, r, b, '-to', 0, 0)
        return dst

    def updateimage(self, sprite):
        self.canvas.delete(self.last_img)
        self.last_img = self.canvas.create_image(16, 24, image=self.images[sprite])
        self.after(100, self.updateimage, (sprite+1) % self.num_sprintes)

app = App()
app.mainloop()

