#include "Game.h"
#include "Command.h"

Game::Game()
{
	previousFrameTime = 0;
	currentFrameTime = 0;
	quit = false;
}

bool Game::initGL()
{
	bool success = true;
	// Initialize GLEW
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error intiailizing GLEW! %s\n", glewGetErrorString(glewError));
		success = false;
	}

	// Make sure OpenGL 2.1 is supported
	if (!GLEW_VERSION_2_1)
	{
		printf("OpenGL 2.1 not supported!\n");
		success = false;
	}

	// Set the viewport
	glViewport(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Initalize projection matrix and check for errors
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.f, 1.f, -1.f);
	checkGL_Error(glGetError(), success);

	// Initialize Modelview Matrix and check for errors
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	checkGL_Error(glGetError(), success);

	// Save the default modelview matrix
	glPushMatrix();

	// Initialize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glEnable(GL_TEXTURE_2D);
	checkGL_Error(glGetError(), success);

	// Set blending
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	checkGL_Error(glGetError(), success);
	
	//Initialize DevIL
	ilInit();
	iluInit();
	ilClearColour(255, 255, 255, 000);

	//Check for error
	ILenum ilError = ilGetError();
	if (ilError != IL_NO_ERROR)
	{
		printf("Error initializing DevIL! %s\n", iluErrorString(ilError));
		return false;
	}


	return success;
}

bool Game::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		// Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!\n");
		}

		if (SDL_NumJoysticks() < 1)
		{
			printf("Warning: No joysticks detected\n");
		}
		else
		{
			inputManager.setJoystick(SDL_JoystickOpen(0));
		}

		// Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Create Context
			gContext = SDL_GL_CreateContext(gWindow);

			if (gContext == NULL)
			{
				printf("OpenGl context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// Use vysnc
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				// Initialize OpenGL
				if (!initGL())
				{
					printf("Unable to initialize OpenGL!\n");
					success = false;
				}
			}
		}
	}


	// Create instance of Tile Manager
	tileManager = TileManager::getInstance();
	return success;
}

bool Game::loadMedia() {
	// Loading success flag
	bool success = true;

	success = character.loadMedia();
	success = tileManager->loadMedia();
	success = enemy.loadMedia();
	success = enemy2.loadMedia();

	if (success)
	{
		camera.objectToFollow = dynamic_cast<GameObject*>(&character);
		camera.offsetX = SCREEN_WIDTH / 2.f;
		camera.offsetY = SCREEN_HEIGHT / 1.75f;
		enemy.setCurrentPlatform(9);
		enemy2.setCurrentPlatform(10);
	}

	return success;
}


void Game::close()
{
	// Destroy window	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool Game::manageInput(SDL_KeyboardEvent key)
{
	return false;
}

void Game::update()
{
	int time = SDL_GetTicks();

	// Update at 60fps
	if (time - previousFrameTime >= 16)
	{
		currentFrameTime = SDL_GetTicks();
		Command* command = inputManager.handleInput();
	
		if (command)
		{
			command->execute(character);
		}
			
		previousFrameTime = currentFrameTime;
		character.update(time);
		enemy.update(time);
		enemy2.update(time);
	}
}

void Game::render()
{
	// Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	camera.render();
	character.render();
	enemy.render();
	enemy2.render();
	//glTranslatef(camera.positionX, camera.positionY, 0.f)
	tileManager->render();

	SDL_GL_SwapWindow(gWindow);
}