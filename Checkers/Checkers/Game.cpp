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
	glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);
	checkGL_Error(glGetError(), success);

	// Initialize Modelview Matrix and check for errors
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	checkGL_Error(glGetError(), success);

	// Save the default modelview matrix
	glPushMatrix();

	// Initialize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);
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

	// Pop default matrix onto current matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// save default matrix again
	glPushMatrix();

	// Move to center of the screen;
	glTranslatef(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f);

	glBegin(GL_QUADS);
	glColor3f(1.f, 0.f, 0.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glEnd();

	//Move to the right of the screen
	glTranslatef(SCREEN_WIDTH, 0.f, 0.f);

	//Green quad
	glBegin(GL_QUADS);
	glColor3f(0.f, 1.f, 0.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glEnd();

	glTranslatef(-SCREEN_WIDTH, 0.f, 0.f);
	glTranslatef((SCREEN_WIDTH / 2.f), (SCREEN_HEIGHT / 2.f), 0.f);

	//Green quad
	glBegin(GL_QUADS);
	glColor3f(1.f, 1.f, 1.f);
	glVertex2f(-100, -100);
	glVertex2f(100, -100);
	glVertex2f(100, 100);
	glVertex2f(-100, 100);
	glEnd();

	glTranslatef(SCREEN_WIDTH, 0.f, 0.f);
	glTranslatef(-((SCREEN_WIDTH / 2.f)), -((SCREEN_HEIGHT / 2.f)), 0.f);

	//Move to the lower right of the screen
	glTranslatef(0.f, SCREEN_HEIGHT, 0.f);

	//Blue quad
	glBegin(GL_QUADS);
	glColor3f(0.f, 0.f, 1.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glEnd();

	//Move below the screen
	glTranslatef(-SCREEN_WIDTH, 0.f, 0.f);

	//Yellow quad
	glBegin(GL_QUADS);
	glColor3f(1.f, 1.f, 0.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, -SCREEN_HEIGHT / 4.f);
	glVertex2f(SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glVertex2f(-SCREEN_WIDTH / 4.f, SCREEN_HEIGHT / 4.f);
	glEnd();

	SDL_GL_SwapWindow(gWindow);
}