#include "Enemy.h"
#include "TileManager.h"

Enemy::Enemy()
{
	width = 50;
	height = 50;
	positionX = 128;
	positionY = 0;
	useGravity = true;
}

Enemy::~Enemy()
{

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

void Enemy::checkCollisions()
{
}

void Enemy::update(int time)
{
	AnimatedGameObject::update(time);
}

void Enemy::render()
{
	AnimatedGameObject::render();
}