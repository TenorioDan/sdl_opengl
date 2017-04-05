#include "Projectile.h"
#include "TileManager.h"

Projectile::Projectile(SpriteSheet& spritesheet, GLfloat x, GLfloat y,  Direction d)
	: GameObject()
{
	useGravity = false;
	toDelete = false;
	this->spritesheet = spritesheet;
	positionX = x;
	positionY = y;
	direction = d;
	horizontalPhysicsState = IN_MOTION;
	animationSpeed = 100;

	// TODO: Dynamically set width/height
	width = 40;
	height = 20;

	// TODO: fix initialization of collider variables
	collider.minX = positionX - (width / 2.f);
	collider.minY = positionY - (height / 2.f);
	collider.maxX = positionX + (width / 2.f);
	collider.maxY = positionY + (width / 2.f);

	collider.prevMaxX = positionX - (width / 2.f);
	collider.prevMinY = positionY - (height / 2.f);
	collider.prevMaxX = positionX + (width / 2.f);
	collider.prevMaxY = positionY + (width / 2.f);

	if (direction == RIGHT)
	{
		startAnimationIndex = 0;
		endAnimationIndex = 2;
		horizontalVelocity = 30;
	}
	else
	{
		startAnimationIndex = 3;
		endAnimationIndex = 5;
		horizontalVelocity = -30;
	}
}

void Projectile::checkCollisions()
{
	std::vector<Collider*> platforms = TileManager::getInstance()->getPlatforms();

	// check against platforms that 
	for (auto p : platforms)
	{
		if (collider.collision(*p) != Collider::NO_COLLISION)
		{
			toDelete = true;
		}
	}
}

void Projectile::update(int time)
{
	GameObject::update(time);

	if (time - currentAnimationTime >= animationSpeed && horizontalPhysicsState == IN_MOTION)
	{
		previousAnimationTime = currentAnimationTime;
		currentAnimationTime = time;

		spriteIndex++;

		if (spriteIndex < startAnimationIndex || spriteIndex > endAnimationIndex)
		{
			spriteIndex = startAnimationIndex;
		}
	}

	if (toDelete)
	{
		//delete this;
	}
}

void Projectile::render()
{
	GameObject::render();
	spritesheet.renderSprite(spriteIndex);
	glTranslatef(-positionX, -positionY, 0.f);
}