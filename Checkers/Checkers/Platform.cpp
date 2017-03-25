#include "Platform.h"

Platform::Platform()
	: GameObject()
{
	useGravity = false;
}

bool Platform::loadMedia()
{
	if (!platformTexture.loadTextureFromFileWithColorKey("grass_dirt_tile.png", 255, 0, 255))
	{
		printf("Unable to load sprite sheet!\n");
		return false;
	}

	positionX = 0;
	positionY = 514;

	return true;
}

void Platform::update(int time)
{

}

void Platform::render()
{
	
}