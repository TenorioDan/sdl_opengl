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

	// set clips
	LFRect clip = { 0.f, 0.f, 64.f, 64.f };

	// Top left
	clip.x = 0.f;
	clip.y = 0.f;
	tileSheet.addClipSprite(clip);

	// Top right
	clip.x = 64.f + tileSpriteOffeset;
	clip.y = 0.f;
	tileSheet.addClipSprite(clip);

	// Bottom left
	clip.x = 128.f + (tileSpriteOffeset * 2);
	clip.y = 0.f;
	tileSheet.addClipSprite(clip);

	// TODO: Fix sprite sheet tile index allocations
	for (int i = 0; i < 9; ++i)
	{
		clip.x = (i * tileWidth) + (tileSpriteOffeset * i);
		clip.y = 74.f;
		tileSheet.addClipSprite(clip);
	}

	if (!tileSheet.generateDataBuffer())
	{
		printf("Unable to clip tile sheet!\n");
		return false;
	}

	createTileset(40, 40, "test_level.txt");
	createColliders("Platforms.txt");

	return true;
}

// Called when creating the tilset of a new level.
void TileManager::createTileset(GLint tilesX, GLint tilesY, std::string path)
{
	numTilesX = tilesX;
	numTilesY = tilesY;

	// dynamically create the number of tiles for this this area
	tileset = new Tile*[numTilesX * numTilesY];

	// Add all tiles that will be rendered to the screen
	std::ifstream  tilesetFile(path.c_str());
	std::string line;
	int y = 0;
	int x = 0;

	while (std::getline(tilesetFile, line))
	{
		std::istringstream buffer(line);
		std::istream_iterator<std::string> beg(buffer), end;
		std::vector<std::string> values(beg, end);
		x = 0;

		for (auto s : values)
		{
			int val = std::stoi(s);
			Tile* t = new Tile();
			t->spriteIndex = val;
			t->positionX = x * tileWidth;
			t->positionY = y * tileWidth;

			tileset[x*numTilesY + y] = t;
			++x;
		}
		++y;
	}
}


// Create the colliders from the provided text file
void TileManager::createColliders(std::string path)
{
	// Read the platforms file and create colliders based on input
	std::ifstream colliderFile(path.c_str());
	std::string line;
	char n;

	while (std::getline(colliderFile, line))
	{
		std::istringstream buffer(line);
		std::istream_iterator<std::string> beg(buffer), end;
		std::vector<std::string> values(beg, end);

		auto it = values.begin();
		int minX = std::stoi(*it++);
		int minY = std::stoi(*it++);
		int maxX = std::stoi(*it++);
		int maxY = std::stoi(*it);

		Collider* c = new Collider();
		c->maxX = maxX - (tileWidth / 2.f);
		c->maxY = maxY - (tileWidth / 2.f);
		c->minX = minX - (tileWidth / 2.f);
		c->minY = minY - (tileWidth / 2.f);
		platforms.push_back(c);
	}
}


// Return the colliders int his level. Typically used for collision detection
std::vector<Collider*> TileManager::getPlatforms()
{
	return platforms;
}


// Render all the tiles in the tileset
void TileManager::render()
{
	for (int y = 0; y < numTilesY; ++y)
	{
		for (int x = 0; x < numTilesX; ++x)
		{
			Tile t = *tileset[x * numTilesY + y];

			if (t.spriteIndex > 0)
			{
				glTranslatef(t.positionX, t.positionY, 0.f);
				tileSheet.renderSprite(t.spriteIndex - 1);
				glTranslatef(-t.positionX, -t.positionY, 0.f);
			}
		}
	}
}

// Clear the tileset to give room for loading different levels
void TileManager::clearTiles()
{
	delete[] tileset;
	numTilesX = 0;
	numTilesY = 0;
}