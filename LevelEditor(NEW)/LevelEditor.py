from EditorGraphics import TileEditor
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



app = TileEditor()
app.mainloop()

