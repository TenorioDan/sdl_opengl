#include "Enemy.h"
#include "TileManager.h"

Enemy::Enemy()
	: AnimatedGameObject()
{
	// TODO: Clean up magic numbers
	width = 50.f;
	height = 50.f;
	health = 50.f;
	damage = 15.f;
	horizontalVelocity = 2.f;
	verticalVelocity = 0.f;
	useGravity = false;
}

Enemy::~Enemy()
{

}

void Enemy::setCurrentPlatform(int platformIndex)
{
	currentPlatform = TileManager::getInstance()->getPlatforms()[platformIndex];
	positionX = currentPlatform->maxX + (width / 2.f);;
	positionY = currentPlatform->minY - (height / 2.f);
}

bool Enemy::loadMedia()
{
	spriteSheet = new SpriteSheet();
	if (!spriteSheet->loadTextureFromFileWithColorKey("star_enemy.png", 255, 0, 255))
	{
		printf("Unable to load sprite sheet!\n");
		return false;
	}

	spriteSheet->createAnimations(width, height, 0.f, 0.f, 0.f, 1);

	if (!spriteSheet->generateDataBuffer())
	{
		printf("Unable to clip sprite sheet!\n");
		return false;
	}
}

void Enemy::attackRoutine()
{
	if (currentPlatform != NULL)
	{
		// TODO: Clean up magic numbers
		if (collider.collision(*currentPlatform) == Collider::NO_COLLISION)
		{
			// assumes clockwise movement
			if (horizontalVelocity > 0)
			{
				horizontalVelocity = 0;
				verticalVelocity = 2;
				positionX = currentPlatform->maxX + (width / 2.f);
			}
			else if (horizontalVelocity < 0)
			{
				horizontalVelocity = 0;
				verticalVelocity = -2;
				positionX = currentPlatform->minX - (width / 2.f);
			}
			else if (verticalVelocity > 0)
			{
				verticalVelocity = 0;
				horizontalVelocity = -2;
				positionY = currentPlatform->maxY + (height / 2.f);
			}
			else if (verticalVelocity < 0)
			{
				verticalVelocity = 0;
				horizontalVelocity = 2;
				positionY = currentPlatform->minY - (height / 2.f);
			}
		}
	}
}

void Enemy::checkCollisions()
{
}

GLfloat Enemy::Health()
{
	return health;
}

GLfloat Enemy::Damage()
{
	return damage;
}

void Enemy::update(int time)
{
	AnimatedGameObject::update(time);
	attackRoutine();
}

void Enemy::render()
{
	AnimatedGameObject::render();
}