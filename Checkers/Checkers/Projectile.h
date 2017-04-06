#pragma once
#include "GameObject.h"
#include "SpriteSheet.h"

class Projectile : public GameObject
{
public:
	Projectile(SpriteSheet& spritesheet, GLfloat x, GLfloat y, Direction d);
	~Projectile();

	void update(int time);
	void render();
	bool ToDelete();

private:
	bool toDelete;
	SpriteSheet* spritesheet;

	void checkCollisions();
};