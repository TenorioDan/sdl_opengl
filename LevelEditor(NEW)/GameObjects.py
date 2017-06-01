from EditorGraphics import SpriteSheet


class GameObject(object):
    def __init__(self):
        pass


class Enemy(GameObject):
    def __init__(self):
        super(Enemy, self).__init__()

    # Place the enemy on the specified tile/location? This should probably be a drag and drop type since enemies
    # and other character aren't going to be tile based
    def place(self):
        pass


class StarMonster(Enemy):
    def __init__(self):
        super(Enemy, self).__init__()
