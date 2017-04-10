#pragma once
#include "AnimatedGameObject.h"
#include "SpriteSheet.h"
#include "Enemy.h"
#include <math.h>

#define SQRT_TWO std::sqrtf(2);

class Projectile : public AnimatedGameObject
{
	
public:
	Projectile(SpriteSheet& spritesheet, GLfloat x, GLfloat y, Direction d, GLfloat aimDirectionX, GLfloat aimDirectionY, GLfloat damage);
	~Projectile();

	void update(int time);
	void render();
	bool loadMedia();
	void setEnemies(std::vector<Enemy*>* e);	

private:
	GLfloat horizontalProjectileSpeed;
	GLfloat verticalProjectileSpeed;
	GLfloat damage;

	std::vector<Enemy*>* enemies;

	void checkCollisions();
};