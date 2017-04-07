#pragma once

#include "Collider.h"

class GameObject
{
public:
	enum PhysicsState
	{
		AT_REST,
		IN_MOTION,
	};

	enum Direction
	{
		LEFT,
		RIGHT
	};

	GameObject();

	virtual void update(int time);
	virtual void render();

	virtual void translate(GLfloat x, GLfloat y);
	
	//void applyHorizontalVelocity;
	Collider getCollider();

	GLfloat PositionX();
	GLfloat PositionY();
	void setVerticalVelocity(GLfloat v);
	void setHorizontalVelocity(GLfloat v);

protected:
	bool useGravity;
	GLuint spriteIndex;
	GLfloat positionX;
	GLfloat positionY;
	GLfloat width;
	GLfloat height;
	GLfloat verticalVelocity;
	GLfloat horizontalVelocity;

	const GLfloat gravity = 4.f;
	const GLfloat maxVerticleSpeed = 24.f; // TODO figure out what max positive verticle velocity should be

	Collider collider;
	Collider* currentPlatform;

	// Protected methods
	virtual void checkCollisions() = 0;
	void applyGravity();

	// Enums
	PhysicsState horizontalPhysicsState;
	PhysicsState verticalPhysicsState;
	Direction direction;
};