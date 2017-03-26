#pragma once

#include "GL_helpers.h"
#include "World.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "Character.h"
#include "Platform.h"
#include "TileManager.h"
#include "InputManager.h"

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

	void update();
	void render();

	bool quit;

private:
	friend class InputManager;
	//Screen dimension constants
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	const int SCREEN_FPS = 60;
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	// OpenGL Context
	SDL_GLContext gContext = NULL;

	Character character;
	Platform ground;
	TileManager* tileManager;
	InputManager inputManager;

	int previousFrameTime, currentFrameTime;
	
	GLfloat gCameraX = 0.f, gCameraY = 0.f;

};