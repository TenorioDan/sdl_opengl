#include "GameObject.h"
#include "TileManager.h"

GameObject::GameObject()
{
	verticleVelocity = 0.f;
	positionX = 0;
	positionY = 0;
	verticleVelocity = 0;
	currentPhysicsState = FALLING;
	landingCollisionNextFrame = false;
}

Collider GameObject::getCollider()
{
	return collider;
}


void GameObject::translate(GLfloat x, GLfloat y)
{
	positionX += x;
	positionY += y;
	collider.setBounds(positionX + (width / 2), positionY + (height / 2 ), positionX - (width / 2), positionY - (height / 2));
}

void GameObject::applyGravity()
{
	if (currentPhysicsState == FALLING && useGravity)
	{
		if (landingCollisionNextFrame)
		{
			currentPhysicsState = AT_REST;
			landingCollisionNextFrame = false;
			positionY = currentPlatform.MinY() - (height / 2);
		}
		else
		{
			translate(0.f, verticleVelocity);
			verticleVelocity = verticleVelocity >= maxVerticleSpeed ? verticleVelocity : verticleVelocity += 4.f;

			if (verticleVelocity > 0)
			{
				translate(0.f, verticleVelocity);
				std::vector<Collider> platforms = TileManager::getInstance()->getPlatforms();

				// check against platforms that 
				for (auto p : platforms)
				{
					if (collider.collision(p))
					{
						landingCollisionNextFrame = true;
						currentPlatform = p;
					}
				}

				translate(0.f, -verticleVelocity);
			}
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