#pragma once

#include "Collider.h"

class GameObject
{
public:
	enum PhysicsState
	{
		AT_REST,
		IN_MOTION, // IN_MOTION and FALLING: too similar? If falling then IN_MOTION, but if IN_MOTION not always falling
		FALLING
	};

	GameObject();

	virtual void update(int time);
	virtual void render();

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