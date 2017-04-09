#include "Projectile.h"
#include "TileManager.h"

Projectile::Projectile(SpriteSheet& ssheet, GLfloat x, GLfloat y,  Direction d)
	: AnimatedGameObject()
{
	useGravity = false;
	toDelete = false;
	spriteSheet = &ssheet;
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
		endAnimationIndex = 1;
		horizontalVelocity = 35;
	}
	else
	{
		startAnimationIndex = 2;
		endAnimationIndex = 3;
		horizontalVelocity = -35;
	}
}

Projectile::~Projectile()
{

}

bool Projectile::loadMedia()
{
	// TODO: Flyweight pattern ensures that media is only loaded once
	// in the weapon class so this virtual method does nothing. Seems 
	// like bad design if this method has to exist.
	return true;
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

bool Projectile::ToDelete()
{
	return toDelete;
}

// Similar 
void Projectile::update(int time)
{
	AnimatedGameObject::update(time);
}

void Projectile::render()
{
	AnimatedGameObject::render();
}
