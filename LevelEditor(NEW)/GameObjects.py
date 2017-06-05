from EditorGraphics import SpriteSheet


class GameObject(object):
    def __init__(self):
        pass


class Enemy(GameObject):
    width = 50
    height = 50

    def __init__(self, position_x, position_y):
        super(Enemy, self).__init__()
        self.position_x = position_x
        self.position_y = position_y

    @staticmethod
    def load_properties():
        pass

    # Place the enemy on the specified tile/location? This should probably be a drag and drop type since enemies
    # and other character aren't going to be tile based
    def place(self):
        pass


class StarMonster(Enemy):
    def __init__(self, position_x, position_y):
        super(StarMonster, self).__init__(position_x, position_y)

