#pragma once
#include "World.h"
#include <IL/il.h>
#include <IL/ilu.h>

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

private:
	//Screen dimension constants
	const int SCREEN_WIDTH = 1920;
	const int SCREEN_HEIGHT = 1080;
	const int SCREEN_FPS = 240;
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	// OpenGL Context
	SDL_GLContext gContext = NULL;
	
	GLfloat gCameraX = 0.f, gCameraY = 0.f;
	Texture gCheckerBoardTexture;
	Texture gLoadedTexture;
	Texture gNon2NTexture;
	Texture gCircleTexture;
	Texture gRotatingTexture;
	Texture gRepeatingTexture;

	// Sprite Sheet texture
	Texture gArrowTexture;

	// Srpite areas
	LFRect gArrowClips[4];

	// Rotation angle
	GLfloat gAngle = 0.f;

	// Transformation state
	int gTransformationCombo = 0;

	// Texture offeset
	GLfloat gTexX = 0.f, gTexY = 0.f;

	// Texture wrap type
	int gTextureWrapType = 0;

};