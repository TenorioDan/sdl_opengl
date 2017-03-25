#include "TileManager.h"
#include <iostream>
#include <fstream>

TileManager::TileManager()
{
	spriteOffeset = 5;
}

bool TileManager::loadMedia()
{
	if (!tileSheet.loadTextureFromFileWithColorKey("tileset_platforms.png", 255, 0, 255))
	{
		printf("Unable to load tile sheet!\n");
		return false;
	}

	// set clips
	LFRect clip = { 0.f, 0.f, 32.f, 32.f };

	// Top left
	clip.x = 0.f;
	clip.y = 0.f;
	tileSheet.addClipSprite(clip);

	// Top right
	clip.x = 32.f + spriteOffeset;
	clip.y = 0.f;
	tileSheet.addClipSprite(clip);

	// Bottom left
	clip.x = 64.f + ( spriteOffeset * 2);
	clip.y = 0.f;
	tileSheet.addClipSprite(clip);


	if (!tileSheet.generateDataBuffer())
	{
		printf("Unable to clip tile sheet!\n");
		return false;
	}

	std::ifstream  tilesetFile("test_level.txt");
	std::string line;
	int y = 0;

	while (std::getline(tilesetFile, line))
	{
		for (int x = 0; x < 40; ++x)
		{
			Tile* t = new Tile();
			t->spriteIndex = line[x] - '0';
			t->positionX = x * 32.f;
			t->positionY = y * 32.f;

			tileset[x][y] = t;
		}

		++y;
	}

	return true;
}

void TileManager::render()
{
	for (int y = 0; y < 23; ++y)
	{
		for (int x = 0; x < 40; ++x)
		{
			Tile t = *tileset[x][y];

			if (t.spriteIndex > 0)
			{
				glLoadIdentity();
				glTranslatef(t.positionX, t.positionY, 0.f);
				tileSheet.renderSprite(t.spriteIndex - 1);
			}
		}
	}
}