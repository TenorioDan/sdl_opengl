#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <afx.h>
#include <iterator>
#include <Windows.h>
#include "TileManager.h"

TileManager::TileManager() 
{

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

	createTileset(findMostRecentLevelFile());

	return true;
}

// Called when creating the tilset of a new level.
void TileManager::createTileset(std::string path)
{
	// dynamically create the number of tiles for this this area
	// Add all tiles that will be rendered to the screen
	std::ifstream  tilesetFile(path.c_str());
	std::string line;

	while (std::getline(tilesetFile, line))
	{
		std::istringstream buffer(line);
		std::istream_iterator<std::string> beg(buffer), end;
		std::vector<std::string> values(beg, end);
		auto it = values.begin();

		if (*it == "TILES")
		{
			readState = Tiles;
			numTilesX = std::stoi(*++it);
			numTilesY = std::stoi(*++it);
			tileset = new Tile*[numTilesX * numTilesY];
		}
		else if (*it == "COLLIDERS")
		{
			readState = Colliders;
		}
		else if (*it == "END")
		{
			// Do nothing, we're done reading the file
		}
		else 
		{
			switch (readState)
			{
				case Tiles: 
				{
					int row = std::stoi(*it++);
					int column = std::stoi(*it++);
					int tileType = std::stoi(*it);
					createTile(row, column, tileType);
					break;
				}
				case Colliders:
				{
					int minX = std::stoi(*it++);
					int minY = std::stoi(*it++);
					int maxX = std::stoi(*it++);
					int maxY = std::stoi(*it);
					createCollider(minX, minY, maxX, maxY);
					break;
				}
			}
		}
	}
}


void TileManager::createTile(int row, int column, int tileType)
{
	Tile* t = new Tile();
	t->spriteIndex = tileType;
	t->positionX = column * tileWidth;
	t->positionY = row * tileHeight;

	tileset[row*numTilesY + column] = t;
}


void TileManager::createCollider(int minX, int minY, int maxX, int maxY)
{
	Collider* c = new Collider();
	c->minX = minX - (tileWidth / 2.f);
	c->minY = minY - (tileWidth / 2.f);
	c->maxX = maxX - (tileWidth / 2.f);
	c->maxY = maxY - (tileWidth / 2.f);
	platforms.push_back(c);
}
// Create the colliders from the provided text file
void TileManager::createColliders(std::string path)
{
	// Read the platforms file and create colliders based on input
	std::ifstream colliderFile(path.c_str());
	std::string line;

	while (std::getline(colliderFile, line))
	{
		std::istringstream buffer(line);
		std::istream_iterator<std::string> beg(buffer), end;
		std::vector<std::string> values(beg, end);
		auto it = values.begin();

		Collider* c = new Collider();
		c->minX = std::stoi(*it++) - (tileWidth / 2.f);
		c->minY = std::stoi(*it++) - (tileWidth / 2.f);
		c->maxX = std::stoi(*it++) - (tileWidth / 2.f);
		c->maxY = std::stoi(*it) - (tileWidth / 2.f);
		platforms.push_back(c);
	}
}


// Return the colliders int his level. Typically used for collision detection
std::vector<Collider*>* TileManager::getPlatforms()
{
	return &platforms;
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
				// Move to the spot to render and then move back to render the next texture 
				// in the correct spot
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

std::string TileManager::findMostRecentLevelFile()
{
	FILETIME bestDate = { 0, 0 };
	FILETIME curDate;
	std::string name;
	CFileFind finder;

	finder.FindFile("/Levels/*.lvl");
	while (finder.FindNextFile())
	{
		finder.GetCreationTime(&curDate);

		if (CompareFileTime(&curDate, &bestDate) > 0)
		{
			bestDate = curDate;
			name = finder.GetFileName().GetString();
		}
	}
	return name;
}