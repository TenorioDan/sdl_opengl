#include "GameObject.h"
#include "TileManager.h"

GameObject::GameObject()
{
	verticleVelocity = 0.f;
	positionX = 0;
	positionY = 0;
	verticleVelocity = 0;
	currentPhysicsState = AT_REST;
}

Collider GameObject::getCollider()
{
	return collider;
}


void GameObject::translate(GLfloat x, GLfloat y)
{
	positionX += x;
	positionY += y;
	collider.setBounds(positionX + width, positionY + height, positionX, positionY);
}

void GameObject::applyGravity()
{
	if (currentPhysicsState == FALLING && useGravity)
	{
		translate(0.f, verticleVelocity);

		if (verticleVelocity > 0)
		{
			std::vector<Collider> platforms = TileManager::getInstance()->getPlatforms();
			// check against platforms

			for (auto p : platforms)
			{
				printf("(%f, %f, %f, %f) vs (%f, %f, %f, %f)\n", collider.MaxX(), collider.MaxY(), collider.MinX(), collider.MinY(), p.MaxX(), p.MaxY(), p.MinX(), p.MinY());
				if (collider.collision(p))
				{
					currentPhysicsState = AT_REST;
					printf("collision");
				}
			}
		}

		if (verticleVelocity < maxVerticleSpeed)
		{
			verticleVelocity += 4.f;
		}
	}
}


GameObject::PhysicsState GameObject::getPhysicsState()
{
	return currentPhysicsState;
}

GLfloat GameObject::PositionX()
{
	return positionX;
}

GLfloat GameObject::PositionY()
{
	return positionY;
}


void GameObject::update(int time)
{
	applyGravity();
}

void GameObject::render()
{
	glLoadIdentity();
	glTranslatef(positionX, positionY, 0.f);
}