#include <iostream>
#include <fstream>
#include "World.h"

World::World()
{
	
}

void World::createTileSet()
{
	std::ifstream tiles_text("test_level.txt");
	std::string line;

	int heightPos = 0;
	// The width/height of the text file needs to match the width/height of the tileset array
	// TODO: Can this be dynamic instead?
	while (std::getline(tiles_text, line))
	{
		for (int widthPos = 0; widthPos < levelWidth; ++widthPos)
		{
			Texture* currentTexture;

			switch (line[widthPos])
			{
			case '0':
				currentTexture = &sky_texture;
				break;
			case '1':
				currentTexture = &grass_dirt_texture;
				break;
			default:
				currentTexture = &sky_texture;
				break;
			}

			tileset[widthPos][heightPos] = new Tile(*currentTexture, currentTexture->getWidth() * widthPos, currentTexture->getHeight() * heightPos);
		}

		++heightPos;
	}
}

bool World::loadTextures(SDL_Renderer* renderer)
{
	bool success = true;

	// Load the grass/dirt texture for the tileset
	if (!grass_dirt_texture.loadFromFile("grass_dirt_tile.png", renderer))
	{
		printf("Failed to load grass/dirt texture image!\n");
		success = false;
	}
	else
	{
		textures.push_back(grass_dirt_texture);
	}

	if (!sky_texture.loadFromFile("sky_texture.png", renderer))
	{
		printf("Failed to load sky texture image!\n");
		success = false;
	}
	else
	{
		// TODO: Why the fuck is this causing the first texture to become invalid?
		//textures.push_back(sky_texture);
	}

	// create the tileset
	createTileSet();

	return success;
}

// Go through the tileset and render each tile
void World::render(SDL_Renderer* renderer)
{
	for (int x = 0; x < levelWidth; ++x)
	{
		for (int y = 0; y < levelHeight; ++y)
		{
			tileset[x][y]->render(renderer);
		}
	}
}

// destroy everything in the world and free the textures
// TODO: Turn into destructor
World::~World()
{
	for (auto t : textures)
	{
		t.free();
	}

}