#include "Character.h"

Character::Character()
{
	positionX = 128;
	positionY = 450;
	spriteIndex = 0;
	totalSprites = 0;
	moveSpeed = 2;
	previousAnimationTime = 0;
	currentAnimationTime = 0;
	animationSpeed = 100;
}

Character::~Character()
{
	characterSpriteSheet.freeSheet();
}

bool Character::loadMedia()
{
	if (!characterSpriteSheet.loadTextureFromFileWithColorKey("character_test.png", 255, 0, 255))
	{
		printf("Unable to load sprite sheet!\n");
		return false;
	}

	// set clips
	LFRect clip = { 0.f, 0.f, 64.f, 128.f };

	// Top left
	clip.x = 0.f;
	clip.y = 0.f;
	characterSpriteSheet.addClipSprite(clip);

	// Top right
	clip.x = 64.f;
	clip.y = 0.f;
	characterSpriteSheet.addClipSprite(clip);

	// Bottom left
	clip.x = 128.f;
	clip.y = 0.f;
	characterSpriteSheet.addClipSprite(clip);

	// Bottom right
	clip.x = 192.f;
	clip.y = 0.f;
	characterSpriteSheet.addClipSprite(clip);

	if (!characterSpriteSheet.generateDataBuffer())
	{
		printf("Unable to clip sprite sheet!\n");
		return false;
	}

	totalSprites = 4;

	return true;
}

void Character::update(int time)
{
	if (time - currentAnimationTime >= animationSpeed)
	{
		previousAnimationTime = currentAnimationTime;
		currentAnimationTime = time;

		spriteIndex++;

		if (spriteIndex >= totalSprites)
		{
			spriteIndex = 0;
		}

	}
}

void Character::render()
{
	//glLoadIdentity();
	glTranslated(positionX, positionY, 0.f);
	characterSpriteSheet.renderSprite(spriteIndex);
}

int Character::PositionX()
{
	return positionX;
}

int Character::PositionY()
{
	return positionY;
}

int Character::MoveSpeed()
{
	return moveSpeed;
}

void Character::setMoveSpeed(int newSpeed)
{
	moveSpeed = newSpeed;
}


void Character::moveX(int displacement)
{
	positionX += displacement;
}

void Character::moveY(int displacement)
{
	positionY += displacement;
}