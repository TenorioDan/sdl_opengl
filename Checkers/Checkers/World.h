#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Texture.h"
#include "Tile.h"

class World
{
public:
	World();
	~World();
	bool loadTextures(SDL_Renderer* renderer);
	void createTileSet();
	void render(SDL_Renderer* renderer);
private:
	Texture grass_dirt_texture;
	Texture sky_texture;
	std::vector<Texture> textures;

	// The number of tiles in the x and y directions
	int levelWidth = 30;
	int levelHeight = 17;
	Tile* tileset[30][17];
};