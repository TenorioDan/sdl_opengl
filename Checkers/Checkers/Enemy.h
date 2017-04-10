#pragma once
#include "AnimatedGameObject.h"

class Enemy : public AnimatedGameObject
{
public:
	Enemy();
	~Enemy();

	// Virtual methods
	virtual void attackRoutine();

	void update(int time);
	void render();
	bool loadMedia();
	void setCurrentPlatform(Collider* c);
	void takeDamage(GLfloat damage);

	GLfloat Health();
	GLfloat Damage();

protected:
	void checkCollisions();
	GLfloat health;
	GLfloat damage;
};