from os import listdir
from os.path import isfile, join
import Tkinter as tk
from tkMessageBox import *
from EditorGraphics import SpriteSheet, Tile, Collider
from GameObjects import StarMonster, TransitionTile

GRID_LINE_WIDTH = 1
LEVEL_DIRECTORY = "../Checkers/Checkers/Levels"
MODES = ["TILE_PLACEMENT", "ENEMY_PLACEMENT"]


def get_available_levels():
    return [f.replace(".lvl", "") for f in listdir(LEVEL_DIRECTORY) if isfile(join(LEVEL_DIRECTORY, f))]


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
        self.show_grid_lines = True
        self.colliders_on = True
        self.canvas_width = 0
        self.canvas_height = 0
        self.tiles_row_count = 0
        self.tiles_column_count = 0
        self.tiles = []
        self.editor_tiles = []
        self.colliders = []
        self.grid_lines = []
        self.enemies = []
        self.enemy_sprites = []
        self.transition_tiles = []
        self.editor_current_image = None
        self.current_enemy = None
        self.editor_current_tile_type = 0
        self.current_top_level = None
        self.tile_spritesheet = None
        self.enemy_spritesheet = None
        self.current_properties_frame = None
        self.tile_properties_frame = None
        self.enemy_properties_frame = None
        self.transition_tile_properties_frame = None
        self.current_object_to_modify_properties = None
        self.current_level_name = "last_level_created"

        self.mode = tk.StringVar()
        self.mode.trace("w", self.change_mode)

        # Create the frame that will contain the controls for the all placement types.
        self.control_frame = tk.Frame(self, width=500, height=500)
        self.control_frame.pack_propagate(False)
        self.control_frame.pack(side=tk.RIGHT, expand=0)

        # Create the load level variable to be used with the dropdown
        self.level_to_load_stringvar = tk.StringVar(self.control_frame)
        self.level_to_load_stringvar.set(self.current_level_name)
        self.transition_level_stringvar = tk.StringVar(self.control_frame)
        self.transition_level_stringvar.trace("w", self.get_level_transition_tiles)
        self.transition_tile_stringvar = tk.StringVar(self.control_frame)

        self.create_misc_controls()
        self.create_tile_controls()
        self.create_enemy_controls()
        self.create_transition_controls()

        # Create the canvas that will contain the GUI for the tile placement functionality
        self.tile_frame = tk.Frame(self, width=1200, height=500)
        self.tile_frame.pack(side=tk.LEFT, expand=0)
        self.tile_canvas = tk.Canvas(self.tile_frame, width=1800, height=1080, bg='#000000')
        self.tile_canvas.bind("<Button-1>", self.add_gameobject_button_call)
        self.tile_canvas.bind("<Shift-Button-1>", self.delete_tile_button_call)
        self.tile_canvas.bind("<Control-Button-1>", self.view_object_properties_callback)
        horizontal_bar = tk.Scrollbar(self.tile_frame, orient=tk.HORIZONTAL)
        horizontal_bar.pack(side=tk.BOTTOM, fill=tk.X)
        horizontal_bar.config(command=self.tile_canvas.xview)
        vertical_bar = tk.Scrollbar(self.tile_frame, orient=tk.VERTICAL)
        vertical_bar.pack(side=tk.RIGHT, fill=tk.Y)
        vertical_bar.config(command=self.tile_canvas.yview)
        self.tile_canvas.config(xscrollcommand=horizontal_bar.set, yscrollcommand=vertical_bar.set)
        self.tile_canvas.pack(side=tk.LEFT)

        self.mode.set("Tiles")

        # Load the most recently created level
        self.import_level()

    def change_mode(self, *args):
        current_mode = self.mode.get()
        self.current_properties_frame.grid_forget()

        if current_mode == "Tiles":
            self.current_properties_frame = self.tile_properties_frame
        elif current_mode == "Enemies":
            self.current_properties_frame = self.enemy_properties_frame
        elif current_mode == "Transition Tile":
            self.current_properties_frame = self.transition_tile_properties_frame

        self.current_properties_frame.grid()

    def get_level_transition_tiles(self, *args):
        transition_level = self.transition_level_stringvar.get()
        self.mode.set("Transition")

        with open("{0}/{1}.lvl".format(LEVEL_DIRECTORY, transition_level), 'r') as level_file:
            begin_reading = False
            transition_tiles = []

            for line in level_file:
                if "TRANSITIONS" in line:
                    begin_reading = True
                elif line == "END":
                    begin_reading = False
                elif begin_reading:
                    transition_properties = line.split(' ')
                    transition_tiles.append(transition_properties[0])

        menu = self.transition_tile_options["menu"]
        menu.delete(0, "end")

        for o in transition_tiles:
            menu.add_command(label=o, command=lambda value=o: self.transition_tile_stringvar.set(value))

    def create_misc_controls(self):
        menu_bar = tk.Menu(self)
        file_menu = tk.Menu(menu_bar, tearoff=0)
        file_menu.add_command(label="New", command=self.load_level)
        file_menu.add_separator()
        file_menu.add_command(label="Load", command=self.load_level)
        file_menu.add_command(label="Save", command=self.export_level)
        file_menu.add_command(label="Save As", command=self.save_as)
        menu_bar.add_cascade(label="File", menu=file_menu)

        tiles_menu = tk.Menu(menu_bar, tearoff=0)
        tiles_menu.add_command(label="Toggle Colliders", command=self.toggle_colliders)
        tiles_menu.add_command(label="Toggle Grid Lines", command=self.toggle_grid_lines)
        menu_bar.add_cascade(label="Tiles", menu=tiles_menu)
        self.config(menu=menu_bar)

        # self.mode_dropdown = tk.OptionMenu(self.control_frame, self.mode, "Tiles", "Enemies")
        # self.mode_dropdown.grid(row=0, column=0)

    def toggle_colliders(self):
        if self.colliders_on:
            self.clear_colliders()
            self.colliders_on = False
        else:
            self.generate_colliders()
            self.colliders_on = True

    def toggle_grid_lines(self):
        if self.show_grid_lines:
            self.show_grid_lines = False
            for line in self.grid_lines:
                self.tile_canvas.delete(line)
        else:
            self.show_grid_lines = True
            self.draw_lines(self.tiles_row_count, self.tiles_column_count)

    # Creates the GUI for the controls section in the editor
    def create_tile_controls(self):
        # Create the tile selector
        tile_select_label = tk.Label(self.control_frame, text="Tiles")
        tile_select_label.grid(row=2, column=0)
        self.tile_select_frame = VerticalScrolledFrame(self.control_frame)
        self.tile_select_frame.grid(row=3, column=0)

        # TODO: Load all sprite sheets uploaded
        # Load the tile sprites into the editor for selecting
        self.load_spritesheet("images/tileset_platforms.png")

        self.tile_properties_frame = tk.Frame(self.control_frame, width=500, height=500)
        self.current_properties_frame = self.tile_properties_frame

        for i in range(len(self.tiles)):
            tile_label = TileLabel(i, self.tile_select_frame.interior, image=self.tiles[i])
            tile_label.bind("<Button-1>", self.set_current_tile)
            tile_label.grid(row=int(i / 5), column=(i % 5))

            # load_level_button.grid(row=5, column=1)

    def create_enemy_controls(self):
        tk.Label(self.control_frame, text="Enemies").grid(row=4, column=0)
        self.enemy_select_frame = VerticalScrolledFrame(self.control_frame)
        self.enemy_select_frame.grid(row=4, column=0)
        self.load_enemies()

        self.enemy_properties_frame = tk.Frame(self.control_frame, width=500, height=500)

        for i in range(len(self.enemy_sprites)):
            enemy_label = TileLabel(0, self.enemy_select_frame.interior, image=self.enemy_sprites[i])
            enemy_label.bind("<Button-1>", self.set_current_enemy)
            enemy_label.grid(row=int(i / 5), column=(i % 5))

    def create_transition_controls(self):
        self.transition_frame = tk.Frame(self.control_frame)
        self.transition_frame.grid(row=5, column=0)
        tk.Label(self.transition_frame, text="Transition Tile").pack(side=tk.LEFT)

        level_options = tk.OptionMenu(self.transition_frame, self.transition_level_stringvar, *get_available_levels())
        level_options.pack(side=tk.LEFT)
        self.transition_tile_options = tk.OptionMenu(self.transition_frame, self.transition_tile_stringvar, [])
        self.transition_tile_options.pack(side=tk.LEFT)

    # Properties window adjuster
    def set_properties(self, object_type, obj):
        pass

    # Load the images from the sprite sheet into the editor
    def load_spritesheet(self, file_name):
        # TODO: Dynamically load the sprites
        self.tile_spritesheet = SpriteSheet(file_name, 10, 64, 64)
        self.tile_spritesheet.create_row_images(0, 3)
        self.tile_spritesheet.create_row_images(1, 9)

        for row in range(self.tile_spritesheet.row_count):
            for i in range(len(self.tile_spritesheet.image_set[row])):
                self.tiles.append(self.tile_spritesheet.image_set[row][i])

    def load_enemies(self):
        # The enemy spritesheet and the corresponding enemies
        enemy_dict = {
            "images/star_enemy.png": StarMonster
        }

        for key, value in enemy_dict.items():
            spritesheet = SpriteSheet(key, 0, value.width, value.height)
            spritesheet.create_row_images(0, 1)

            # Only need to add one image from the spritesheet into the enemy images
            self.enemy_sprites.append(spritesheet.image_set[0][0])

    def set_current_enemy(self, event):
        self.mode.set('Enemies')
        self.current_enemy = event.widget.image

    # Should only be called by a TileLabel to set the current image of the editor for drawing on the tile canvas
    def set_current_tile(self, event):
        self.mode.set('Tiles')
        self.editor_current_image = event.widget.image
        self.editor_current_tile_type = event.widget.tile_type + 1

    def add_tile(self, tile):
        if tile:
            if tile.canvas_image:
                self.tile_canvas.delete(tile.canvas_image)

            y = (tile.row * (self.tile_spritesheet.tile_width + 1)) + 1
            x = (tile.column * (self.tile_spritesheet.tile_height + 1)) + 1

            canvas_image = self.tile_canvas.create_image(x, y, image=self.editor_current_image, anchor=tk.NW)
            # TODO: Fix offset issue with the tile types
            tile.tile_type = self.editor_current_tile_type
            tile.canvas_image = canvas_image
            self.generate_colliders()

    def add_enemy(self, position_x, position_y):
        closest_collider = self.colliders[0]
        closest_distance = float("inf")
        collider_index = 0
        closest_collider_index = 0
        velocity_x = 0
        velocity_y = 0

        for c in self.colliders:
            dx, dy, distance = c.distance(position_x, position_y)

            if distance < closest_distance:
                closest_distance = distance
                closest_collider = c
                closest_collider_index = collider_index

            collider_index += 1

        between_x = closest_collider.min_x < position_x < closest_collider.max_x
        between_y = closest_collider.min_y < position_y < closest_collider.max_y
        offset_x, offset_y = self.get_offset(position_x, position_y)

        # set the position according to the distances between the x and y components
        if not between_x:
            if position_x < closest_collider.min_x:
                position_x = closest_collider.min_x - (StarMonster.width / 2) + offset_x
                velocity_y = -2
            else:
                position_x = closest_collider.max_x + (StarMonster.width / 2) + offset_x
                velocity_y = 2

        if not between_y:
            if position_y < closest_collider.min_y:
                position_y = closest_collider.min_y - (StarMonster.height / 2) + offset_y
                velocity_x = 2
            else:
                position_y = closest_collider.max_y + (StarMonster.height / 2) + offset_y
                velocity_x = -2

        if not (between_x and between_y):
            enemy = StarMonster(position_x, position_y, velocity_x, velocity_y,
                                self.tile_canvas.create_image(position_x, position_y, image=self.current_enemy),
                                closest_collider_index)
            self.enemies.append(enemy)

    def add_transition_tile(self, tile):
        if tile:
            y = (tile.row * (self.tile_spritesheet.tile_width + 1)) + 1
            x = (tile.column * (self.tile_spritesheet.tile_height + 1)) + 1
            transition = TransitionTile(tile.column * 64, tile.row * 64,
                                        self.tile_canvas.create_rectangle(x, y, x + TransitionTile.width,
                                                                          y + TransitionTile.height, fill='blue'))
            transition.level_to_transition_to = self.transition_level_stringvar.get()
            self.transition_tiles.append(transition)

    # takes a mouse click event and adds a tile to the space clicked on
    def add_gameobject_button_call(self, event):
        mode = self.mode.get()
        if self.grid_created:
            if mode == "Tiles" and self.editor_current_image is not None:
                self.add_tile(self.get_tile_clicked(event))
            elif mode == "Enemies" and self.current_enemy is not None:
                self.add_enemy(*self.get_position_clicked(event))
            elif mode == "Transition":
                self.add_transition_tile(self.get_tile_clicked(event))

    def delete_tile_button_call(self, event):
        if self.grid_created and self.editor_current_image is not None:
            current_tile = self.get_tile_clicked(event)

            if current_tile:
                self.tile_canvas.delete(current_tile.canvas_image)
                current_tile.tile_type = 0
                current_tile.image = None
                self.generate_colliders()

    # Set the properties view to the first object found at the point clicked
    def view_object_properties_callback(self, event):
        for e in self.enemies:
            e_min_x = e.position_x - (e.width / 2)
            e_min_y = e.position_y - (e.height / 2)
            e_max_x = e.position_x + (e.width / 2)
            e_max_y = e.position_y + (e.height / 2)
            x, y = self.get_position_clicked(event)

            if (e_min_x < x < e_max_x) and (e_min_y < y < e_max_y):
                self.set_properties("ENEMY", e)

    def get_tile_clicked(self, event):
        canvas = event.widget

        # calculate the x/y position of the tile that the sprite will be drawn on.
        row_index = int(canvas.canvasy(event.y) / (self.tile_spritesheet.tile_height + GRID_LINE_WIDTH))
        column_index = int(canvas.canvasx(event.x) / (self.tile_spritesheet.tile_width + GRID_LINE_WIDTH))

        if row_index < self.tiles_row_count and column_index < self.tiles_column_count:
            return self.editor_tiles[row_index][column_index]
        else:
            return None

    def get_offset(self, x, y):
        return int(x / self.tile_spritesheet.tile_width) + 1, int(y / self.tile_spritesheet.tile_height) + 1

    def get_position_clicked(self, event):
        canvas = event.widget
        return canvas.canvasx(event.x), canvas.canvasy(event.y)

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
        # TODO: Add specific exception
        except:
            showerror("You Fucked Up", "Tile dimensions are invalid")
        else:
            self.tile_canvas.delete("all")
            self.generate_tiles(tiles_rows, tiles_columns)

    # Loop through the tiles in the level and generate colliders and represent them visually with green rectangles
    def generate_colliders(self):
        self.clear_colliders()
        self.colliders = []

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

                        top_left_x = column * self.tile_spritesheet.tile_width
                        top_left_y = row * self.tile_spritesheet.tile_height
                        current_collider = Collider(top_left_x, top_left_y,
                                                    top_left_x + self.tile_spritesheet.tile_width,
                                                    top_left_y + self.tile_spritesheet.tile_height)
                        last_collider_column = column
                    else:
                        current_collider.max_x += self.tile_spritesheet.tile_width
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
            x_min_offset = (int(c.min_x / self.tile_spritesheet.tile_width))
            x_max_offset = (int(c.max_x / self.tile_spritesheet.tile_width))
            y_min_offset = (int(c.min_y / self.tile_spritesheet.tile_height))
            y_max_offset = (int(c.max_y / self.tile_spritesheet.tile_height))
            c.rect = self.tile_canvas.create_rectangle(c.min_x + x_min_offset, c.min_y + y_min_offset,
                                                       c.max_x + x_max_offset,
                                                       c.max_y + y_max_offset, outline="green")

    def clear_colliders(self):
        if len(self.colliders) > 0:
            for c in self.colliders:
                self.tile_canvas.delete(c.rect)

                # self.colliders = []

    # Draw the GRID for the tile editor
    def draw_lines(self, tile_x_count, tile_y_count):
        self.tiles_row_count = tile_x_count
        self.tiles_column_count = tile_y_count
        self.canvas_width = (self.tile_spritesheet.tile_width + GRID_LINE_WIDTH) * tile_x_count
        self.canvas_height = (self.tile_spritesheet.tile_height + GRID_LINE_WIDTH) * tile_y_count

        for i in range(tile_x_count + 1):
            self.grid_lines.append(
                self.tile_canvas.create_line((self.tile_spritesheet.tile_width + GRID_LINE_WIDTH) * i, 0,
                                             (self.tile_spritesheet.tile_width + GRID_LINE_WIDTH) * i,
                                             self.canvas_height,
                                             fill="white"))

        for i in range(tile_y_count + 1):
            self.grid_lines.append(
                self.tile_canvas.create_line(0, (self.tile_spritesheet.tile_height + GRID_LINE_WIDTH) * i,
                                             self.canvas_width,
                                             (self.tile_spritesheet.tile_height + GRID_LINE_WIDTH) * i,
                                             fill="white"))

        # Now that the grid has been created, the scroll area can be set to accommodate the number of tiles in the
        # editor
        self.tile_canvas.config(scrollregion=(
            0, 0, (self.tile_spritesheet.tile_width + GRID_LINE_WIDTH) * tile_x_count,
            (self.tile_spritesheet.tile_height + GRID_LINE_WIDTH) * tile_y_count))

    # Load the images from the sprite sheet into the editor
    def load_spritesheet(self, file_name):
        # TODO: Dynamically load the sprites
        self.tile_spritesheet = SpriteSheet(file_name, 10, 64, 64)
        self.tile_spritesheet.create_row_images(0, 3)
        self.tile_spritesheet.create_row_images(1, 9)

        for row in range(self.tile_spritesheet.row_count):
            for i in range(len(self.tile_spritesheet.image_set[row])):
                self.tiles.append(self.tile_spritesheet.image_set[row][i])

    def load_enemies(self):
        # The enemy spritesheet and the corresponding enemies
        enemy_dict = {
            "images/star_enemy.png": StarMonster
        }

        for key, value in enemy_dict.items():
            spritesheet = SpriteSheet(key, 0, value.width, value.height)
            spritesheet.create_row_images(0, 1)

            # Only need to add one image from the spritesheet into the enemy images
            self.enemy_sprites.append(spritesheet.image_set[0][0])

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

                    level_file.write("ENEMIES {}\n".format(str(len(self.enemies))))

                    for e in self.enemies:
                        level_file.write(
                            "{0} {1} {2} {3} {4} {5}\n".format("1", e.collider, e.position_x, e.position_y,
                                                               e.velocity_x, e.velocity_y))

                    level_file.write("TRANSITIONS {}\n".format(str(len(self.transition_tiles))))

                    for t in self.transition_tiles:
                        level_file.write(
                            "{0} {1} {2} {3} {4}\n".format(t.name, t.level_to_transition_to, t.transition_tile_name,
                                                           t.position_x,
                                                           t.position_y))

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
        self.transition_tiles = []
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
                elif properties[0] == "ENEMIES":
                    mode = "ENEMIES"
                elif properties[0] == "END":
                    pass
                elif mode[0] == "TRANSITIONS":
                    mode = "TRANSITIONS"
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
                        self.colliders.append(
                            Collider(int(properties[0]), int(properties[1]), int(properties[2]), int(properties[3])))
                    elif mode == "ENEMIES":
                        pass
                    elif mode == "TRANSITIONS":
                        pass

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

        available_levels = get_available_levels()
        level_options = tk.OptionMenu(top, self.level_to_load_stringvar, *available_levels)
        level_options.config(width=15)
        level_options.pack()
        load_level_button = tk.Button(top, text="Import", command=self.import_level)
        load_level_button.pack(pady=5)


app = TileEditor()
app.mainloop()
