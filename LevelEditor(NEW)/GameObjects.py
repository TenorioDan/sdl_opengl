from EditorGraphics import SpriteSheet


class GameObject(object):
    def __init__(self):
        self._editable_properties = []

    def return_editable_properties(self):
        return self._editable_properties


class Enemy(GameObject):
    width = 50
    height = 50

    def __init__(self, position_x, position_y, velocity_x, velocity_y, canvas_image):
        super(Enemy, self).__init__()
        self.position_x = position_x
        self.position_y = position_y
        self.velocity_x = velocity_x
        self.velocity_y = velocity_y
        self.canvas_image = canvas_image

    @staticmethod
    def load_properties():
        pass

    # Place the enemy on the specified tile/location? This should probably be a drag and drop type since enemies
    # and other character aren't going to be tile based
    def place(self):
        pass


# These must always be attached to a platform collider.
class StarMonster(Enemy):
    def __init__(self, position_x, position_y, velocity_x, velocity_y, canvas_image, collider):
        super(StarMonster, self).__init__(position_x, position_y, velocity_x, velocity_y, canvas_image)
        self.collider = collider
        self.direction_modifier = 1
        self._editable_properties = ["direction_modifier"]

    def place(self):
        pass


# Essentially a special type of AABB that will be used in the game to transition the player between levels/rooms
class TransitionTile(GameObject):
    width = 64
    height = 64

    def __init__(self, position_x, position_y, canvas_rect):
        super(TransitionTile, self).__init__()
        self.position_x = position_x
        self.position_y = position_y
        self.canvas_rect = canvas_rect
        self.level_to_transition_to = ""
        self.name = "CHANGE_NAME"
        self.transition_tile_name = "CHANGE_NAME"
        self._editable_properties = ["name", "level_to_transition_to", "transition_to_name"]
        self._editable_properties = {"name": str, "level_to_transition_to": str}
