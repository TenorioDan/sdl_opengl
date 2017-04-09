#pragma once
#include "AnimatedGameObject.h"
#include "SpriteSheet.h"
#include <math.h>

#define SQRT_TWO std::sqrtf(2);

class Projectile : public AnimatedGameObject
{
	
public:
	Projectile(SpriteSheet& spritesheet, GLfloat x, GLfloat y, Direction d, GLfloat aimDirectionX, GLfloat aimDirectionY);
	~Projectile();

	void update(int time);
	void render();
	bool loadMedia();
	bool ToDelete();

private:
	bool toDelete;

	GLfloat horizontalProjectileSpeed;
	GLfloat verticalProjectileSpeed;

	void checkCollisions();
};