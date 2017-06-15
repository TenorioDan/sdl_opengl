#include "Projectile.h"
#include "LevelManager.h"

Projectile::Projectile(SpriteSheet& ssheet, GLfloat x, GLfloat y,  Direction d, GLfloat aimDirectionX, GLfloat aimDirectionY, GLfloat dmg)
	: AnimatedGameObject()
{
	// TODO: Clean up magic numbers
	useGravity = false;
	toDelete = false;
	spriteSheet = &ssheet;
	positionX = x;
	positionY = y;
	direction = d;
	horizontalPhysicsState = IN_MOTION;
	animationSpeed = 100;
	damage = dmg;

	GLfloat horizontalProjectileSpeed = 25;
	GLfloat verticalProjectileSpeed = 25;


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
		endAnimationIndex = 0;

		if (aimDirectionX == 0 && aimDirectionY == 0)
			aimDirectionX = 1;
	}
	else
	{
		startAnimationIndex = 1;
		endAnimationIndex = 1;

		if (aimDirectionX == 0 && aimDirectionY == 0)
			aimDirectionX = -1;
	}

	horizontalVelocity = horizontalProjectileSpeed * aimDirectionX;
	verticalVelocity = verticalProjectileSpeed * -aimDirectionY;

	if (aimDirectionX != 0 && aimDirectionY != 0)
	{
		horizontalVelocity /= SQRT_TWO;
		verticalVelocity /= SQRT_TWO;
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
	// check against platforms that 
	for (auto p : *LevelManager::getInstance()->getPlatforms())
	{
		if (collider.collision(*p) != Collider::NO_COLLISION)
		{
			toDelete = true;
		}
	}

	for (auto e : *enemies)
	{
		if (collider.collision(e->getCollider()) != Collider::NO_COLLISION)
		{
			toDelete = true;
			e->takeDamage(damage);
		}
	}
}

void Projectile::setEnemies(std::vector<Enemy*>* e)
{
	enemies = e;
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
