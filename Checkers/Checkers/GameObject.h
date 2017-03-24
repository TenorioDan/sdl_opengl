#pragma once

#include "Collider.h"

class GameObject
{
public:
	enum PhysicsState
	{
		AT_REST,
		IN_MOTION, 
		FALLING
	};

	GameObject();

	void update(int time);
	void translate(GLfloat x, GLfloat y);
	Collider getCollider();

	GLfloat PositionX();
	GLfloat PositionY();
	PhysicsState getPhysicsState();

protected:
	bool useGravity;
	GLfloat positionX;
	GLfloat positionY;
	GLfloat verticleVelocity;

	const GLfloat maxVerticleSpeed = 50.f; // TODO figure out what max positive verticle velocity should be

	Collider collider;

	// Protected methods
	void applyGravity();

	PhysicsState currentPhysicsState;

};