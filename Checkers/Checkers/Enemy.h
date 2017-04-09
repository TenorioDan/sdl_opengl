#pragma once
#include "AnimatedGameObject.h"

class Enemy : public AnimatedGameObject
{
public:
	Enemy();
	~Enemy();

	void update(int time);
	void render();
	bool loadMedia();

	GLfloat Health();
	GLfloat Damage();

protected:
	void checkCollisions();
	GLfloat health;
	GLfloat damage;
};