#include "Character.h"
#include "TileManager.h"
#include <cmath>

Character::Character()
	: GameObject()
{
	positionX = 128;
	positionY = 0;
	width = 85.f;
	height = 87.f;
	spriteIndex = 0;
	moveSpeed = 10;
	previousAnimationTime = 0;
	currentAnimationTime = 0;
	animationSpeed = 100;
	jumpAnimationSpeed = 50;
	currentState = IDLE;
	verticalVelocity = 0;
	useGravity = true;
	canJump = true;
	direction = RIGHT;
}

Character::~Character()
{
	characterSpriteSheet.freeSheet();
	for (auto p : projectiles)
	{
		delete p;
	}
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
	if (!characterSpriteSheet.loadTextureFromFileWithColorKey("shovel_knight_original.png", 255, 255, 255))
	{
		printf("Unable to load sprite sheet!\n");
		return false;
	}

	createAnimations(characterSpriteSheet, width, height, 5.f, 266.f, 10.f, 4);

	if (!characterSpriteSheet.generateDataBuffer())
	{
		printf("Unable to clip sprite sheet!\n");
		return false;
	}

	if (!projectileSpriteSheet.loadTextureFromFileWithColorKey("blaster-1.png", 255, 0, 255))
	{
		printf("Unable to load blaster sprite sheet\n");
		return false;
	}

	createAnimations(projectileSpriteSheet, 40.f, 20.f, 0.f, 0.f, 0.f, 2);

	if (!projectileSpriteSheet.generateDataBuffer())
	{
		printf("Unable to clip blaster sprite sheet");
		return false;
	}

	return true;
}

// Create the character animations based on the spritesheets provided
void Character::createAnimations(SpriteSheet& spritesheet, GLfloat spriteWidth, GLfloat spriteHeight, GLfloat spriteOffset, 
								 GLfloat spriteStartPositionX, GLfloat spriteStartPositionY, int animationCount)
{
	// TODO: Create character spritesheet and define locations
	// set clips
	LFRect clip = { 0.f, 0.f, spriteWidth, spriteHeight };

	// Add running right sprites
	for (int i = 0; i < animationCount; ++i)
	{
		clip.x = spriteStartPositionX + (spriteWidth * i) + (spriteOffset * i);
		clip.y = spriteStartPositionY;
		spritesheet.addClipSprite(clip);
	}
	
	for (int i = 0; i < animationCount; ++i)
	{
		clip.x = spritesheet.imageWidth() - (spriteStartPositionX + (spriteWidth * (i + 1)) + (spriteOffset * i));
		clip.y = spriteStartPositionY;
		spritesheet.addClipSprite(clip);
	}
}


void Character::attack()
{
	Projectile *p = new Projectile(projectileSpriteSheet, positionX, positionY, direction);
	projectiles.push_back(p);
}

void Character::jump()
{
	if (verticalPhysicsState == AT_REST && canJump)
	{
		verticalPhysicsState = IN_MOTION;
		verticalVelocity = baseJumpSpeed;
		currentPlatform = NULL;
		canJump = false;
	}
}

// TODO: Change collision results based on object type
// TODO: Quad Trees for per section collision detection
void Character::checkCollisions()
{
	std::vector<Collider*> platforms = TileManager::getInstance()->getPlatforms();

	if (currentPlatform != NULL && collider.collision(*currentPlatform) == Collider::CollisionDirection::NO_COLLISION)
	{
		currentPlatform = NULL;
		verticalPhysicsState = IN_MOTION;
		verticalVelocity = 0.f;
	}

	// check against platforms that 
	for (auto p : platforms)
	{
		// Predictive collision detection
		translate(horizontalVelocity, verticalVelocity);

		switch (collider.collision(*p))
		{
		case Collider::CollisionDirection::LEFT:
			translate(-horizontalVelocity, 0.f);
			break;
		case Collider::CollisionDirection::RIGHT:
			translate(-horizontalVelocity, 0.f);
			break;
		case Collider::CollisionDirection::ABOVE:
			currentPlatform = p;
			positionY = p->minY - (height / 2.f);
			verticalPhysicsState = AT_REST;
			verticalVelocity = 0;
			break;
		case Collider::CollisionDirection::BELOW:
			verticalVelocity = 0;
			break;
		}

		translate(-horizontalVelocity, -verticalVelocity);
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

	if (directionModifier > 0)
	{
		direction = RIGHT;
		startAnimationIndex = 0;
		endAnimationIndex = 3;
	}
	else if (directionModifier < 0)
	{
		direction = LEFT;
		startAnimationIndex = 4;
		endAnimationIndex = 7;
	}
}

void Character::reduceHorizontalMovement()
{
	horizontalVelocity = 0;
	horizontalPhysicsState = AT_REST;
}

GLfloat Character::MoveSpeed()
{
	return moveSpeed;
}

void Character::setMoveSpeed(int newSpeed)
{
	moveSpeed = newSpeed;
}


// For debugging
void Character::resetPosition()
{
	positionX = 128;
	positionY = 0;
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

		if (spriteIndex < startAnimationIndex || spriteIndex > endAnimationIndex)
		{
			spriteIndex = startAnimationIndex;
		}
	}

	for (auto it = projectiles.begin(); it != projectiles.end();)
	{
		Projectile* p = *it;
		p->update(time);

		if (p->ToDelete())
		{
			delete p;
			it = projectiles.erase(it);
		}
		else
		{
			++it;
		}

	}
}

void Character::render()
{
	GameObject::render();
	characterSpriteSheet.renderSprite(spriteIndex);
	glTranslatef(-positionX, -positionY, 0.f);

	for (auto p : projectiles)
	{
		p->render();
	}
}