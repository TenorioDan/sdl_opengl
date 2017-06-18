#pragma once
#include "AnimatedGameObject.h"
#include "SpriteSheet.h"
#include "Enemy.h"
#include <math.h>

#define SQRT_TWO std::sqrtf(2);

class Projectile : public AnimatedGameObject
{
	
public:
	Projectile(GLfloat x, GLfloat y, Direction d, GLfloat aimDirectionX, GLfloat aimDirectionY, GLfloat damage);
	~Projectile();

	void update(int time);
	void render();

private:
	GLfloat horizontalProjectileSpeed;
	GLfloat verticalProjectileSpeed;
	GLfloat damage;

	std::vector<Enemy*>* enemies;

	void checkCollisions();
};