from EditorGraphics import SpriteSheet


class GameObject(object):
    def __init__(self):
        pass


class Enemy(GameObject):
    width = 50
    height = 50

    def __init__(self, position_x, position_y, canvas_image):
        super(Enemy, self).__init__()
        self.position_x = position_x
        self.position_y = position_y
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
    def __init__(self, position_x, position_y, canvas_image):
        super(StarMonster, self).__init__(position_x, position_y, canvas_image)

    def place(self):
        pass
