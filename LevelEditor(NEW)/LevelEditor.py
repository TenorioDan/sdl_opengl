import Tkinter as tk
from tkMessageBox import *
from EditorGraphics import SpriteSheet, Tile

class TileEditor(tk.Tk):
    def __init__(self):
        tk.Tk.__init__(self)
        self.grid_created = False
        self.canvas_width = 0
        self.canvas_height = 0
        # Frame handling
        self.control_frame = tk.Frame(self, width=500, height=500)
        self.control_frame.pack(side=tk.RIGHT, expand=0)
        label = tk.Label(self.control_frame, text="Level Dimensions")
        label.grid(row=0, column=0)
        #label.pack()
        self.entry_tiles_x = tk.Entry(self.control_frame)
        self.entry_tiles_x.grid(row=0, column=1)

        label2 = tk.Label(self.control_frame, text=" X ")
        label2.grid(row=0, column=2)

        self.entry_tiles_y = tk.Entry(self.control_frame)
        self.entry_tiles_y.grid(row=0, column=3)


        button = tk.Button(self.control_frame, text="Generate Tiles", command=self.generateTilesButtonCall)
        button.grid(row=2, column=0)

        self.tile_frame = tk.Frame(self, width=1200, height=500)
        self.tile_frame.pack(side=tk.LEFT, expand=0)
        self.tile_canvas = tk.Canvas(self.tile_frame, width=1800, height=1080, bg='#000000')
        self.tile_canvas.bind("<Button-1>", self.addTile)
        horizontal_bar = tk.Scrollbar(self.tile_frame, orient=tk.HORIZONTAL)
        horizontal_bar.pack(side=tk.BOTTOM, fill=tk.X)
        horizontal_bar.config(command=self.tile_canvas.xview)
        vertical_bar = tk.Scrollbar(self.tile_frame, orient=tk.VERTICAL)
        vertical_bar.pack(side=tk.RIGHT, fill=tk.Y)
        vertical_bar.config(command=self.tile_canvas.yview)
        self.tile_canvas.config(xscrollcommand=horizontal_bar.set, yscrollcommand=vertical_bar.set)
        self.tile_canvas.pack(side=tk.LEFT)

        # Tile editor
        self.tiles = []

        self.loadSpritesheet("images/tileset_platforms.png")

    def addTile(self, event):
        if self.grid_created:
           canvas = event.widget
           # calculate the x/y position of the tile that the sprite will be drawn on.
           x = (int(canvas.canvasx(event.x) / (self.spritesheet.tile_width + 1))) * (self.spritesheet.tile_width + 1) + 1
           y = (int(canvas.canvasy(event.y) / (self.spritesheet.tile_height + 1))) * (self.spritesheet.tile_height + 1) + 1
           self.tile_canvas.create_image(x, y, image=self.tiles[0], anchor=tk.NW)


    def generateTilesButtonCall(self):
        self.tile_canvas.delete("all")
        tilesX = self.entry_tiles_x.get()
        tilesY = self.entry_tiles_y.get()

        try:
            self.drawLines(int(tilesX), int(tilesY))
            self.grid_created = True
        except:
            showerror("You Fucked Up", "Tile dimensions are invalid")

    # Draw the GRID for the tile editor
    def drawLines(self, tile_x_count, tile_y_count):
        self.canvas_width = (self.spritesheet.tile_width + 1) * tile_x_count
        self.canvas_height = (self.spritesheet.tile_height + 1) * tile_y_count

        for i in range(tile_x_count + 1):
            self.tile_canvas.create_line((self.spritesheet.tile_width + 1) * i, 0,
                                         (self.spritesheet.tile_width + 1) * i, self.canvas_height, fill="white")

        for i in range(tile_y_count + 1):
            self.tile_canvas.create_line(0, (self.spritesheet.tile_height + 1) * i,
                                         self.canvas_width, (self.spritesheet.tile_height + 1) * i, fill="white")

        # Now that the grid has been created, the scroll area can be set to accommodate the number of tiles in the
        # editor
        self.tile_canvas.config(scrollregion=(0, 0, (self.spritesheet.tile_width + 1) * tile_x_count, (self.spritesheet.tile_height + 1) * tile_y_count))

    # Load the images from the sprite sheet into the editor
    def loadSpritesheet(self, file_name):
        # TODO: Dynamically load the sprites
        self.spritesheet = SpriteSheet(file_name, 10, 64, 64)
        self.spritesheet.CreateRowImages(0, 3)
        self.spritesheet.CreateRowImages(1, 9)

        for row in range(self.spritesheet.row_count):
            for i in range(len(self.spritesheet.tiles_set[row])):
                self.tiles.append(self.spritesheet.tiles_set[row][i])

app = TileEditor()
app.mainloop()

