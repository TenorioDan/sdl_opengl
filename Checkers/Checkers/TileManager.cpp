#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include "TileManager.h"

TileManager* TileManager::instance = NULL;

TileManager::TileManager() {}
TileManager::TileManager(TileManager const&) {}
//TileManager& TileManager::operator=(TileManager const&) { return; }

TileManager* TileManager::getInstance()
{
	if (!instance)
		instance = new TileManager();

	return instance;
}

bool TileManager::loadMedia()
{
	if (!tileSheet.loadTextureFromFileWithColorKey("tileset_platforms.png", 255, 0, 255))
	{
		printf("Unable to load tile sheet!\n");
		return false;
	}

	int spriteOffset = 5;

	// set clips
	LFRect clip = { 0.f, 0.f, 32.f, 32.f };

	// Top left
	clip.x = 0.f;
	clip.y = 0.f;
	tileSheet.addClipSprite(clip);

	// Top right
	clip.x = 32.f + spriteOffset;
	clip.y = 0.f;
	tileSheet.addClipSprite(clip);

	// Bottom left
	clip.x = 64.f + ( spriteOffset * 2);
	clip.y = 0.f;
	tileSheet.addClipSprite(clip);


	if (!tileSheet.generateDataBuffer())
	{
		printf("Unable to clip tile sheet!\n");
		return false;
	}

	// Add all tiles that will be rendered to the screen
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

	// Read the platforms file and create colliders based on input
	std::ifstream colliderFile("Platforms.txt");
	char n;

	while (std::getline(colliderFile, line))
	{
		std::istringstream buffer(line);
		std::istream_iterator<std::string> beg(buffer), end;
		std::vector<std::string> values(beg, end);

		int counter = 0;
		int maxX = 0, maxY = 0, minX = 0, minY = 0;

		for (auto s : values)
		{
			int val = std::stoi(s);
			printf("%d\n", val);
			switch (counter)
			{
			case 0:
				minX = val;
				break;
			case 1:
				minY = val;
				break;
			case 2:
				maxX = val;
				break;
			case 3:
				maxY = val;
				break;
			}

			++counter;
		}

		Collider c;
		c.setBounds(maxX, maxY, minX, minY);
		platforms.push_back(c);
	}	

	return true;
}

std::vector<Collider> TileManager::getPlatforms()
{
	return platforms;
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