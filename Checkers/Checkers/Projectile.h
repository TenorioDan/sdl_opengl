#pragma once
#include "AnimatedGameObject.h"
#include "SpriteSheet.h"

class Projectile : public AnimatedGameObject
{
public:
	Projectile(SpriteSheet& spritesheet, GLfloat x, GLfloat y, Direction d);
	~Projectile();

	void update(int time);
	void render();
	bool loadMedia();
	bool ToDelete();

private:
	bool toDelete;

	void checkCollisions();
};