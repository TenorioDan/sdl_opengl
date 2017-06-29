#pragma once

#include "GL_helpers.h"
#include "World.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include "InputManager.h"
#include "LevelManager.h"
#include "MediaManager.h"

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
	const int RESOLUTION_WIDTH = 1280;
	const int RESOLUTION_HEIGHT = 720;
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	int SCREEN_FPS = 60;
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	// OpenGL Context
	SDL_GLContext gContext = NULL;
	
	LevelManager* levelManager;
	MediaManager* mediaManager;
	InputManager inputManager;

	int previousFrameTime, currentFrameTime;
};