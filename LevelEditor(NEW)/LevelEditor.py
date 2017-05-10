import Tkinter as tk
from tkMessageBox import *
from EditorGraphics import SpriteSheet, Tile

# Taken from http://stackoverflow.com/questions/16188420/python-tkinter-scrollbar-for-frame
class VerticalScrolledFrame(tk.Frame):
    '''A pure Tkinter scrollable frame that actually works!
    * Use the 'interior' attribute to place widgets inside the scrollable frame
    * Construct and pack/place/grid normally
    * This frame only allows vertical scrolling
    '''
    def __init__(self, parent, *args, **kw):
        tk.Frame.__init__(self, parent, *args, **kw)

        # create a canvas object and a vertical scrollbar for scrolling it
        vscrollbar = tk.Scrollbar(self, orient=tk.VERTICAL)
        vscrollbar.pack(fill=tk.Y, side=tk.RIGHT, expand=tk.FALSE)
        canvas = tk.Canvas(self, bd=0, highlightthickness=0,
                        yscrollcommand=vscrollbar.set)
        canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=tk.TRUE)
        vscrollbar.config(command=canvas.yview)

        # reset the view
        canvas.xview_moveto(0)
        canvas.yview_moveto(0)

        # create a frame inside the canvas which will be scrolled with it
        self.interior = interior = tk.Frame(canvas)
        interior_id = canvas.create_window(0, 0, window=interior,
                                           anchor=tk.NW)

        # track changes to the canvas and frame width and sync them,
        # also updating the scrollbar
        def _configure_interior(event):
            # update the scrollbars to match the size of the inner frame
            size = (interior.winfo_reqwidth(), interior.winfo_reqheight())
            canvas.config(scrollregion="0 0 %s %s" % size)
            if interior.winfo_reqwidth() != canvas.winfo_width():
                # update the canvas's width to fit the inner frame
                canvas.config(width=interior.winfo_reqwidth())
        interior.bind('<Configure>', _configure_interior)

        def _configure_canvas(event):
            if interior.winfo_reqwidth() != canvas.winfo_width():
                # update the inner frame's width to fill the canvas
                canvas.itemconfigure(interior_id, width=canvas.winfo_width())
        canvas.bind('<Configure>', _configure_canvas)


class TileLabel(tk.Label, object):
    def __init__(self, tile_type, *args, **kwargs):
        super(TileLabel, self).__init__(*args, **kwargs)
        self.tile_type = tile_type
        self.image = kwargs.pop('image')


class TileEditor(tk.Tk):
    def __init__(self):
        tk.Tk.__init__(self)
        self.grid_created = False
        self.canvas_width = 0
        self.canvas_height = 0
        self.tiles = []
        self.editor_current_image = None

        self.createControls()
        # Create the canvas that will contain the GUI for the tile placement functionality
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

    # Creates the GUI for the controls section in the editor
    def createControls(self):
        # Creates the controls frame
        self.control_frame = tk.Frame(self, width=500, height=500)
        self.control_frame.pack(side=tk.RIGHT, expand=0)

        # Create the tile generation inputs and buttons
        label = tk.Label(self.control_frame, text="Level Dimensions")
        label.grid(row=0, column=0)
        self.entry_tiles_x = tk.Entry(self.control_frame)
        self.entry_tiles_x.grid(row=0, column=1)
        label2 = tk.Label(self.control_frame, text=" X ")
        label2.grid(row=0, column=2)
        self.entry_tiles_y = tk.Entry(self.control_frame)
        self.entry_tiles_y.grid(row=0, column=3)
        button = tk.Button(self.control_frame, text="Generate Tiles", command=self.generateTilesButtonCall)
        button.grid(row=2, column=0)

        # Create the tile selector
        self.tile_select_frame = VerticalScrolledFrame(self.control_frame)
        self.tile_select_frame.grid(row=3, column=0, columnspan=4)

        # TODO: Load all sprite sheets uploaded
        # Load the tile sprites into the editor for selecting
        self.loadSpritesheet("images/tileset_platforms.png")

        for i in range(len(self.tiles)):
            tile_label = TileLabel(i, self.tile_select_frame.interior, image=self.tiles[i])
            tile_label.bind("<Button-1>", self.setCurrentTile)
            tile_label.grid(row=int(i / 5), column=(i % 5))


    # Should only be called by a TileLabel to set the current image of the editor for drawing on the tile canvas
    def setCurrentTile(self, event):
        self.editor_current_image = event.widget.image


    # takes a mouse click event and adds a tile to the space clicked on
    def addTile(self, event):
        if self.grid_created:
            canvas = event.widget
            # calculate the x/y position of the tile that the sprite will be drawn on.
            x = (int(canvas.canvasx(event.x) / (self.spritesheet.tile_width + 1))) * (
                self.spritesheet.tile_width + 1) + 1
            y = (int(canvas.canvasy(event.y) / (self.spritesheet.tile_height + 1))) * (
                self.spritesheet.tile_height + 1) + 1

            if self.editor_current_image != None:
                self.tile_canvas.create_image(x, y, image=self.editor_current_image, anchor=tk.NW)
            else:
                showerror("You Fucked Up", "No image selected")

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
        self.tile_canvas.config(scrollregion=(
            0, 0, (self.spritesheet.tile_width + 1) * tile_x_count, (self.spritesheet.tile_height + 1) * tile_y_count))

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
