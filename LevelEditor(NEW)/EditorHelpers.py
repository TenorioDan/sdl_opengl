from os import listdir
from os.path import isfile, join
from tkMessageBox import showerror
import Tkinter as tk

LEVEL_DIRECTORY = "../Checkers/Checkers/Levels"
TILE_WIDTH = 64
TILE_HEIGHT = 64
GRID_LINE_WIDTH = 1

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