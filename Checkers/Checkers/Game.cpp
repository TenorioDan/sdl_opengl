#include "Game.h"

Game::Game()
{
}

bool Game::initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

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

	return success;
}

bool Game::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
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
			printf("Warning: Linear texture filtering not enabled!");
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

	return success;
}

bool Game::loadMedia() {
	// Loading success flag
	bool success = true;

	// TODO: Add menu textures and load them here
	
	// checkerboard pixels
	const int CHECKERBOARD_WIDTH = 128;
	const int CHECKERBOARD_HEIGHT = 128;
	const int CHECKERBOARD_PIXEL_COUNT = CHECKERBOARD_WIDTH * CHECKERBOARD_HEIGHT;
	GLuint checkerBoard[CHECKERBOARD_PIXEL_COUNT];

	// go through the pixels
	for (int i = 0; i < CHECKERBOARD_PIXEL_COUNT; ++i)
	{
		// Get the individual color components
		GLubyte* colors = (GLubyte*)&checkerBoard[i];

		// If the 5th bit of the x and y offsets of the pixel do not match
		if (i / 128 & 16 ^ i % 128 & 16)
		{
			// Set pixel to white
			colors[0] = 0xFF;
			colors[1] = 0xFF;
			colors[2] = 0xFF;
			colors[3] = 0xFF;
		}
		else
		{
			// Set pixel to red
			colors[0] = 0xFF;
			colors[1] = 0x00;
			colors[2] = 0x00;
			colors[3] = 0xFF;
		}

		// load texutre
		if (!gCheckerBoardTexture.loadTextureFromPixels32(checkerBoard, CHECKERBOARD_WIDTH, CHECKERBOARD_HEIGHT))
		{
			printf("unable to load checkerboard texture!\n");
			success = false;
		}
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
	switch (key.keysym.sym)
	{
	case SDLK_ESCAPE:
		return true;
		break;
	case SDLK_w:
		gCameraY -= 16.f;
		break;
	case SDLK_s:
		gCameraY += 16.f;
		break;
	case SDLK_a:
		gCameraX -= 16.f;
		break;
	case SDLK_d:
		gCameraX += 16.f;
		break;
	default:
		break;
	}

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glLoadIdentity();

	// Move camera to position
	glTranslatef(-gCameraX, -gCameraY, 0.f);

	// Save default matrix again with camera translation

	return false;
}

void Game::update()
{

}

void Game::render()
{
	// Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//Calculate centered offsets
	GLfloat x = (SCREEN_WIDTH - gCheckerBoardTexture.textureWidth()) / 2.f;
	GLfloat y = (SCREEN_HEIGHT - gCheckerBoardTexture.textureHeight()) / 2.f;

	//Render checkerboard texture
	gCheckerBoardTexture.render(x, y);
	SDL_GL_SwapWindow(gWindow);
}