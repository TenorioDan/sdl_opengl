from os import listdir
from os.path import isfile, join
import Tkinter as tk
from tkMessageBox import *
from EditorGraphics import SpriteSheet, Tile, Collider

GRID_LINE_WIDTH = 1
LEVEL_DIRECTORY = "../Checkers/Checkers/Levels"
MODES = ["TILE_PLACEMENT", "ENEMY_PLACEMENT"]


class TileLabel(tk.Label, object):
    def __init__(self, tile_type, *args, **kwargs):
        super(TileLabel, self).__init__(*args, **kwargs)
        self.tile_type = tile_type
        self.image = kwargs.pop('image')


class Dialog:
    def __init__(self, parent, dialog_text, button_text):
        self.parent = parent
        top = self.top = tk.Toplevel(parent)

        tk.Label(top, text=dialog_text).pack()

        self.entry = tk.Entry(top)
        self.entry.pack(padx=5)

        b = tk.Button(top, text=button_text, command=self.save)
        b.pack(pady=5)

    def save(self):
        val = self.entry.get()

        if val == "":
            showerror("You Fucked Up", "Enter a level name")
        else:
            self.parent.current_level_name = val
            self.parent.export_level(val)
            self.top.destroy()


# Taken from http://stackoverflow.com/questions/16188420/python-tkinter-scrollbar-for-frame
class VerticalScrolledFrame(tk.Frame):
    """A pure Tkinter scrollable frame that actually works!
    * Use the 'interior' attribute to place widgets inside the scrollable frame
    * Construct and pack/place/grid normally
    * This frame only allows vertical scrolling
    """

    def __init__(self, parent, *args, **kw):
        tk.Frame.__init__(self, parent, *args, **kw)

        # create a canvas object and a vertical scrollbar for scrolling it
        vscrollbar = tk.Scrollbar(self, orient=tk.VERTICAL)
        vscrollbar.pack(fill=tk.Y, side=tk.RIGHT, expand=tk.FALSE)
        canvas = tk.Canvas(self, bd=0, highlightthickness=0, yscrollcommand=vscrollbar.set)
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


class TileEditor(tk.Tk):
    def __init__(self):
        tk.Tk.__init__(self)
        self.grid_created = False
        self.canvas_width = 0
        self.canvas_height = 0
        self.tiles_row_count = 0
        self.tiles_column_count = 0
        self.tiles = []
        self.editor_tiles = []
        self.colliders = []
        self.editor_current_image = None
        self.editor_current_tile_type = 0
        self.mode = "TILE_PLACEMENT"
        self.current_top_level = None
        self.current_level_name = "last_level_created"

        # Create the frame that will contain the controls for the all placement types.
        self.control_frame = tk.Frame(self, width=500, height=500)
        self.control_frame.pack_propagate(False)
        self.control_frame.pack(side=tk.RIGHT, expand=0)

        # Create the load level variable to be used with the dropdown
        self.level_to_load_stringvar = tk.StringVar(self.control_frame)
        self.level_to_load_stringvar.set(self.current_level_name)

        self.create_misc_controls()
        self.create_tile_controls()

        # Create the canvas that will contain the GUI for the tile placement functionality
        self.tile_frame = tk.Frame(self, width=1200, height=500)
        self.tile_frame.pack(side=tk.LEFT, expand=0)
        self.tile_canvas = tk.Canvas(self.tile_frame, width=1800, height=1080, bg='#000000')
        self.tile_canvas.bind("<Button-1>", self.add_tile_button_call)
        self.tile_canvas.bind("<Shift-Button-1>", self.delete_tile_button_call)
        horizontal_bar = tk.Scrollbar(self.tile_frame, orient=tk.HORIZONTAL)
        horizontal_bar.pack(side=tk.BOTTOM, fill=tk.X)
        horizontal_bar.config(command=self.tile_canvas.xview)
        vertical_bar = tk.Scrollbar(self.tile_frame, orient=tk.VERTICAL)
        vertical_bar.pack(side=tk.RIGHT, fill=tk.Y)
        vertical_bar.config(command=self.tile_canvas.yview)
        self.tile_canvas.config(xscrollcommand=horizontal_bar.set, yscrollcommand=vertical_bar.set)
        self.tile_canvas.pack(side=tk.LEFT)

        # Load the most recently created level
        self.import_level()

    def create_misc_controls(self):
        menu_bar = tk.Menu(self)
        file_menu = tk.Menu(menu_bar, tearoff=0)
        file_menu.add_command(label="New", command=self.load_level)
        file_menu.add_separator()
        file_menu.add_command(label="Load", command=self.load_level)
        file_menu.add_command(label="Save", command=self.export_level)
        file_menu.add_command(label="Save As", command=self.save_as)
        menu_bar.add_cascade(label="File", menu=file_menu)
        self.config(menu=menu_bar)

    # Creates the GUI for the controls section in the editor
    def create_tile_controls(self):
        # Create the tile generation inputs and button
        label = tk.Label(self.control_frame, text="Level Dimensions")
        label.grid(row=0, column=0)
        self.entry_tiles_x = tk.Entry(self.control_frame)
        self.entry_tiles_x.grid(row=0, column=1)
        label2 = tk.Label(self.control_frame, text=" X ")
        label2.grid(row=0, column=2)
        self.entry_tiles_y = tk.Entry(self.control_frame)
        self.entry_tiles_y.grid(row=0, column=3)
        generate_tiles_button = tk.Button(self.control_frame, text="Generate Tiles",
                                          command=self.generate_tiles_button_call)
        generate_tiles_button.grid(row=2, column=0)
        generate_colliders_button = tk.Button(self.control_frame, text="Generate Colliders",
                                              command=self.generate_colliders)
        generate_colliders_button.grid(row=2, column=1)
        delete_colliders_button = tk.Button(self.control_frame, text="Clear Colliders", command=self.clear_colliders)
        delete_colliders_button.grid(row=2, column=3)

        # Create the tile selector
        self.tile_select_frame = VerticalScrolledFrame(self.control_frame)
        self.tile_select_frame.grid(row=3, column=0, columnspan=4)

        # TODO: Load all sprite sheets uploaded
        # Load the tile sprites into the editor for selecting
        self.load_spritesheet("images/tileset_platforms.png")

        for i in range(len(self.tiles)):
            tile_label = TileLabel(i, self.tile_select_frame.interior, image=self.tiles[i])
            tile_label.bind("<Button-1>", self.set_current_tile)
            tile_label.grid(row=int(i / 5), column=(i % 5))

        #load_level_button.grid(row=5, column=1)

    def create_enemy_controls(self):
        pass

    # Should only be called by a TileLabel to set the current image of the editor for drawing on the tile canvas
    def set_current_tile(self, event):
        self.editor_current_image = event.widget.image
        self.editor_current_tile_type = event.widget.tile_type + 1

    def add_tile(self, tile):
        if tile:
            if tile.canvas_image:
                self.tile_canvas.delete(tile.canvas_image)

            y = (tile.row * (self.spritesheet.tile_width + 1)) + 1
            x = (tile.column * (self.spritesheet.tile_height + 1)) + 1

            canvas_image = self.tile_canvas.create_image(x, y, image=self.editor_current_image, anchor=tk.NW)
            # TODO: Fix offset issue with the tile types
            tile.tile_type = self.editor_current_tile_type
            tile.canvas_image = canvas_image

    # takes a mouse click event and adds a tile to the space clicked on
    def add_tile_button_call(self, event):
        if self.grid_created and self.editor_current_image is not None:
            current_tile = self.get_tile_clicked(event)
            self.add_tile(current_tile)

    def delete_tile_button_call(self, event):
        if self.grid_created and self.editor_current_image is not None:
            current_tile = self.get_tile_clicked(event)

            if current_tile:
                self.tile_canvas.delete(current_tile.canvas_image)
                current_tile.tile_type = 0
                current_tile.image = None

    def get_tile_clicked(self, event):
        canvas = event.widget

        # calculate the x/y position of the tile that the sprite will be drawn on.
        row_index = int(canvas.canvasy(event.y) / (self.spritesheet.tile_height + GRID_LINE_WIDTH))
        column_index = int(canvas.canvasx(event.x) / (self.spritesheet.tile_width + GRID_LINE_WIDTH))

        if row_index < self.tiles_row_count and column_index < self.tiles_column_count:
            return self.editor_tiles[row_index][column_index]
        else:
            return None

    # Empty the current tileset in the editor and create empty tile objects to be used in rendering and setting tile
    # properties
    def generate_tiles(self, tiles_rows, tiles_columns):
        self.editor_tiles = []

        for r in range(tiles_rows):
            self.editor_tiles.append([])
            for c in range(tiles_columns):
                self.editor_tiles[r].append(Tile(r, c))

        self.grid_created = True

    # Take the input entered by the user and generate a grid the correct size
    # Create Tile Label objects on each spot in the grid that can be clicked to
    def generate_tiles_button_call(self):
        try:
            tiles_rows = int(self.entry_tiles_x.get())
            tiles_columns = int(self.entry_tiles_y.get())
            self.draw_lines(tiles_rows, tiles_columns)
        except:
            showerror("You Fucked Up", "Tile dimensions are invalid")
        else:
            self.tile_canvas.delete("all")
            self.generate_tiles(tiles_rows, tiles_columns)

    # Loop through the tiles in the level and generate colliders and represent them visually with green rectangles
    def generate_colliders(self):
        self.clear_colliders()

        for row in range(self.tiles_row_count):
            current_collider = None
            last_collider_column = 0

            for column in range(self.tiles_column_count):
                current_tile = self.editor_tiles[row][column]

                # If a tile that can be collided with is present, set it and use it as the basis for the
                # next collider
                if current_tile.tile_type != 0:
                    if current_collider is None or column - last_collider_column > 1:
                        if current_collider is not None:
                            self.colliders.append(current_collider)

                        top_left_x = column * self.spritesheet.tile_width
                        top_left_y = row * self.spritesheet.tile_height
                        current_collider = Collider(top_left_x, top_left_y,
                                                    top_left_x + self.spritesheet.tile_width,
                                                    top_left_y + self.spritesheet.tile_height)
                        last_collider_column = column
                    else:
                        current_collider.max_x += self.spritesheet.tile_width
                        last_collider_column = column

            if current_collider is not None:
                self.colliders.append(current_collider)

        self.colliders = sorted(self.colliders, key=lambda col: col.min_x)

        # combine colliders to create larger AABBs instead of having several smaller ones
        colliders_to_delete = []
        previous = None
        for i in range(len(self.colliders)):
            if i > 0:
                if previous is None:
                    previous = self.colliders[i - 1]

                current = self.colliders[i]

                if previous.min_x == current.min_x and previous.max_x == current.max_x \
                        and previous.max_y == current.min_y:
                    previous.max_y = current.max_y
                    colliders_to_delete.append(current)
                else:
                    previous = None

        for c in colliders_to_delete:
            self.colliders.remove(c)

        for i in range(len(self.colliders)):
            c = self.colliders[i]
            x_min_offset = (int(c.min_x / self.spritesheet.tile_width))
            x_max_offset = (int(c.max_x / self.spritesheet.tile_width))
            y_min_offset = (int(c.min_y / self.spritesheet.tile_height))
            y_max_offset = (int(c.max_y / self.spritesheet.tile_height))
            c.rect = self.tile_canvas.create_rectangle(c.min_x + x_min_offset, c.min_y + y_min_offset,
                                                       c.max_x + x_max_offset,
                                                       c.max_y + y_max_offset, outline="green")

    def clear_colliders(self):
        if len(self.colliders) > 0:
            for c in self.colliders:
                self.tile_canvas.delete(c.rect)

            self.colliders = []

    # Draw the GRID for the tile editor
    def draw_lines(self, tile_x_count, tile_y_count):
        self.tiles_row_count = tile_x_count
        self.tiles_column_count = tile_y_count
        self.canvas_width = (self.spritesheet.tile_width + GRID_LINE_WIDTH) * tile_x_count
        self.canvas_height = (self.spritesheet.tile_height + GRID_LINE_WIDTH) * tile_y_count

        for i in range(tile_x_count + 1):
            self.tile_canvas.create_line((self.spritesheet.tile_width + GRID_LINE_WIDTH) * i, 0,
                                         (self.spritesheet.tile_width + GRID_LINE_WIDTH) * i, self.canvas_height,
                                         fill="white")

        for i in range(tile_y_count + 1):
            self.tile_canvas.create_line(0, (self.spritesheet.tile_height + GRID_LINE_WIDTH) * i,
                                         self.canvas_width, (self.spritesheet.tile_height + GRID_LINE_WIDTH) * i,
                                         fill="white")

        # Now that the grid has been created, the scroll area can be set to accommodate the number of tiles in the
        # editor
        self.tile_canvas.config(scrollregion=(
            0, 0, (self.spritesheet.tile_width + GRID_LINE_WIDTH) * tile_x_count,
            (self.spritesheet.tile_height + GRID_LINE_WIDTH) * tile_y_count))

    # Load the images from the sprite sheet into the editor
    def load_spritesheet(self, file_name):
        # TODO: Dynamically load the sprites
        self.spritesheet = SpriteSheet(file_name, 10, 64, 64)
        self.spritesheet.CreateRowImages(0, 3)
        self.spritesheet.CreateRowImages(1, 9)

        for row in range(self.spritesheet.row_count):
            for i in range(len(self.spritesheet.tiles_set[row])):
                self.tiles.append(self.spritesheet.tiles_set[row][i])

    # Use etree to create an xml file for each tile in the
    def export_level(self, level_name=None):
        if not level_name:
            level_name = self.current_level_name

        if self.grid_created:
            new_level_file = open("../Checkers/Checkers/Levels/{0}.lvl".format(level_name), 'w')
            last_level_created = open("../Checkers/Checkers/Levels/last_level_created.lvl", 'w')
            level_files = [new_level_file, last_level_created]

            for level_file in level_files:
                if level_name not in (None, ""):
                    # Insert the tiles and the number of tiles for the game to read and process
                    level_file.write("TILES {} {}\n".format(self.tiles_row_count, self.tiles_column_count))

                    for row in range(self.tiles_row_count):
                        for column in range(self.tiles_column_count):
                            current_tile = self.editor_tiles[row][column]

                            # Set all the properties in the tile element from the tile object
                            level_file.write("{} {} {} {} {}\n".format(row, column, str(current_tile.tile_type),
                                                                       str(current_tile.is_destructible),
                                                                       str(current_tile.is_false_tile)))

                    # Generate the colliders and place in the level file
                    self.generate_colliders()
                    level_file.write("COLLIDERS {}\n".format(str(len(self.colliders))))
                    # Create the collider file
                    for c in self.colliders:
                        level_file.write("{0} {1} {2} {3}\n".format(c.min_x, c.min_y, c.max_x, c.max_y))

                    level_file.write("END")
                    level_file.close()
                else:
                    showerror("You Fucked Up", "Enter a level name")
        else:
            showerror("You Fucked up", "Generate a level grid")

    def import_level(self):
        level = self.level_to_load_stringvar.get()
        self.current_level_name = level
        self.title(level)
        self.level_to_load_stringvar.set(level)
        self.tile_canvas.delete("all")

        with open("{0}/{1}.lvl".format(LEVEL_DIRECTORY, level), 'r') as level_file:
            mode = ""
            for line in level_file:
                properties = line.split(" ")

                if properties[0] == "TILES":
                    mode = "TILES"
                    self.tiles_row_count = int(properties[1])
                    self.tiles_column_count = int(properties[2])
                    self.draw_lines(self.tiles_row_count, self.tiles_column_count)
                    self.generate_tiles(self.tiles_row_count, self.tiles_column_count)
                elif properties[0] == "COLLIDERS":
                    mode = "COLLIDERS"
                elif properties[0] == "END":
                    pass
                else:
                    if mode == "TILES":
                        tile_type = int(properties[2])
                        if tile_type > 0:
                            row = int(properties[0])
                            column = int(properties[1])
                            tile = self.editor_tiles[row][column]
                            self.editor_current_tile_type = tile_type
                            self.editor_current_image = self.tiles[tile_type - 1]
                            self.add_tile(tile)

                    elif mode == "COLLIDERS":
                        self.colliders.append(Collider(properties[0], properties[1], properties[2], properties[3]))

        # Destroy the load level dialog
        if self.current_top_level:
            self.current_top_level.destroy()

    def save_as(self):
        dialog = Dialog(self, "Level Name", "Save")
        self.wait_window(dialog.top)

    # Create the load level dialog
    def load_level(self):
        top = self.current_top_level = tk.Toplevel()
        top.title("Load Level")
        tk.Label(top, text="Choose Level").pack()

        available_levels = [f.replace(".lvl", "") for f in listdir(LEVEL_DIRECTORY) if isfile(join(LEVEL_DIRECTORY, f))]
        level_options = tk.OptionMenu(top, self.level_to_load_stringvar, *available_levels)
        level_options.config(width=15)
        level_options.pack()
        load_level_button = tk.Button(top, text="Import", command=self.import_level)
        load_level_button.pack(pady=5)


app = TileEditor()
app.mainloop()
