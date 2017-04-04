#include "Character.h"
#include <cmath>

Character::Character()
	: GameObject()
{
	positionX = 128;
	positionY = 0;
	width = 90.f;
	height = 90.f;
	spriteIndex = 0;
	totalSprites = 0;
	moveSpeed = 8;
	previousAnimationTime = 0;
	currentAnimationTime = 0;
	animationSpeed = 100;
	jumpAnimationSpeed = 50;
	currentState = IDLE;
	verticleVelocity = 0;
	useGravity = true;
	canJump = true;
}

Character::~Character()
{
	characterSpriteSheet.freeSheet();
}

void Character::translate(GLfloat x, GLfloat y)
{
	positionX += x;
	positionY += y;

	collider.prevMaxX = collider.maxX;
	collider.prevMaxY = collider.maxY;
	collider.prevMinX = collider.minX;
	collider.prevMinY = collider.minY;

	collider.maxX = positionX + (width / 2) - colliderOffset;
	collider.maxY = positionY + (height / 2);
	collider.minX = positionX - (width / 2) + colliderOffset;
	collider.minY = positionY - (height / 2);
}

bool Character::loadMedia()
{
	if (!characterSpriteSheet.loadTextureFromFileWithColorKey("Fusion-Samus.png", 255, 255, 255))
	{
		printf("Unable to load sprite sheet!\n");
		return false;
	}

	// TODO: Create character spritesheet and define locations
	// set clips
	LFRect clip = { 0.f, 0.f, 80.f, 90.f };
	int offset = 15;
	int initialX = 15;
	int initialY = 765;
	int runningSpriteCount = 10;

	for (int i = 0; i < runningSpriteCount; ++i)
	{
		clip.x = initialX + (width * i) + (i * offset);
		clip.y = 765;
		characterSpriteSheet.addClipSprite(clip);
	}

	if (!characterSpriteSheet.generateDataBuffer())
	{
		printf("Unable to clip sprite sheet!\n");
		return false;
	}

	totalSprites = 10;

	return true;
}


// Virtual character update that switches between character states
// applies gravity and swaps animations 
void Character::update(int time)
{
	GameObject::update(time);

	if (time - currentAnimationTime >= animationSpeed && horizontalPhysicsState == IN_MOTION)
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

void Character::jump()
{
	if (verticalPhysicsState == AT_REST && canJump)
	{
		verticalPhysicsState = IN_MOTION;
		verticleVelocity = baseJumpSpeed;
		currentPlatform = NULL;
		canJump = false;
	}
}

void Character::resetJump()
{
	canJump = true;
}

void Character::applyHorizontalMovement(GLfloat directionModifier)
{

	horizontalVelocity = (moveSpeed * directionModifier);
	horizontalPhysicsState = IN_MOTION;
}

void Character::reduceHorizontalMovement()
{
	horizontalVelocity = 0;
	horizontalPhysicsState = AT_REST;
}

void Character::render()
{
	GameObject::render();
	characterSpriteSheet.renderSprite(spriteIndex);
	glTranslatef(-positionX, -positionY, 0.f);
}


GLfloat Character::MoveSpeed()
{
	return moveSpeed;
}

void Character::setMoveSpeed(int newSpeed)
{
	moveSpeed = newSpeed;
}

void Character::resetPosition()
{
	positionX = 128;
	positionY = 0;
}