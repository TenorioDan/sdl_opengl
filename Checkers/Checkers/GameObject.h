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

	GameObject();

	virtual void update(int time);
	virtual void render();

	virtual void translate(GLfloat x, GLfloat y);
	//void applyHorizontalVelocity;
	Collider getCollider();

	GLfloat PositionX();
	GLfloat PositionY();

protected:
	bool useGravity;
	GLfloat positionX;
	GLfloat positionY;
	GLfloat width;
	GLfloat height;
	GLfloat verticleVelocity;
	GLfloat horizontalVelocity;

	const GLfloat gravity = 4.f;
	const GLfloat maxVerticleSpeed = 24.f; // TODO figure out what max positive verticle velocity should be

	Collider collider;
	Collider* currentPlatform;

	// Protected methods
	void checkCollisions();
	void applyGravity();

	bool landingCollisionNextFrame;

	PhysicsState horizontalPhysicsState;
	PhysicsState verticalPhysicsState;
};