#pragma once
#include "World.h"
#include <SDL_opengl.h>
#include <gl\GLU.h>

class Game
{
public:
	// Initializes variables
	Game();

	// initializes SDL
	bool init();
	bool initGL();

	// loads assets
	bool loadMedia();

	// deletes objects and ends SDL
	void close();

	// handle input for the game
	bool manageInput(SDL_KeyboardEvent key);

	void render();

	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

private:
	//Screen dimension constants
	const int SCREEN_WIDTH = 1920;
	const int SCREEN_HEIGHT = 1080;
	// The checkers board that will contain all spaces and peices

	World world;
	
};