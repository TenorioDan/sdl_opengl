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
	// Set clip rectangles
	//gArrowClips[0].x = 0.f;
	//gArrowClips[0].y = 0.f;
	//gArrowClips[0].w = 128.f;
	//gArrowClips[0].h = 128.f;

	//gArrowClips[1].x = 128.f;
	//gArrowClips[1].y = 0.f;
	//gArrowClips[1].w = 128.f;
	//gArrowClips[1].h = 128.f;

	//gArrowClips[2].x = 0.f;
	//gArrowClips[2].y = 128.f;
	//gArrowClips[2].w = 128.f;
	//gArrowClips[2].h = 128.f;

	//gArrowClips[3].x = 128.f;
	//gArrowClips[3].y = 128.f;
	//gArrowClips[3].w = 128.f;
	//gArrowClips[3].h = 128.f;

	/*if (!gNon2NTexture.loadTextureFromFile("opengl.png"))
	{
		printf("Unable to load arrow texture!\n");
		success = false;
	}*/

	if (!gRepeatingTexture.loadTextureFromFile("texture.png"))
	{
		printf("Unable to load texture!\n");
		success = false;
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
	case SDLK_q:
		// Cycle through texture repetitions
		gTextureWrapType++;
		if (gTextureWrapType >= 2)
		{
			gTextureWrapType = 0;
		}

		// Set texture repetition
		glBindTexture(GL_TEXTURE_2D, gRepeatingTexture.getTextureID());

		switch (gTextureWrapType)
		{
		case 0:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			printf("%d: GL_REPEAT\n", gTextureWrapType);
			break;
		case 1:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			printf("%d: GL_REPEAT\n", gTextureWrapType);
			break;
		}
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
	// Scroll texture
	gTexX++;
	gTexY++;

	// Cap Scrolling
	if (gTexX >= gRepeatingTexture.textureWidth())
		gTexX = 0.f;

	if (gTexY >= gRepeatingTexture.textureHeight())
		gTexY = 0.f;

}

void Game::render()
{
	// Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Calculate texture maxima
	GLfloat textureRight = (GLfloat)SCREEN_WIDTH / (GLfloat)gRepeatingTexture.textureWidth();
	GLfloat textureBottom = (GLfloat)SCREEN_HEIGHT / (GLfloat)gRepeatingTexture.textureHeight();

	// Use repeating texture
	glBindTexture(GL_TEXTURE_2D, gRepeatingTexture.getTextureID());

	// Switch to texture matrix
	glMatrixMode(GL_TEXTURE);

	// Reset transformation
	glLoadIdentity();

	// Scroll texture
	glTranslatef(gTexX / gRepeatingTexture.textureWidth(), gTexY / gRepeatingTexture.textureHeight(), 0.f);

	// Render
	glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f); glVertex2f(0.f, 0.f);
		glTexCoord2f(textureRight, 0.f); glVertex2f(SCREEN_WIDTH, 0.f);
		glTexCoord2f(textureRight, textureBottom); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
		glTexCoord2f(0.f, textureBottom); glVertex2f(0.f, SCREEN_HEIGHT);
	glEnd();

	// Render arrows
	//gArrowTexture.render(0.f, 0.f, &gArrowClips[0]);
	//gArrowTexture.render(SCREEN_WIDTH - gArrowClips[1].w, 0.f, &gArrowClips[1]);
	//gArrowTexture.render(0.f, SCREEN_HEIGHT - gArrowClips[2].h, &gArrowClips[2]);
	//gArrowTexture.render(SCREEN_WIDTH - gArrowClips[3].w, SCREEN_HEIGHT - gArrowClips[3].h, &gArrowClips[3]);

	//Render OpenGL texture
	//gNon2NTexture.render((SCREEN_WIDTH - gNon2NTexture.imageWidth()) / 2.f, (SCREEN_HEIGHT - gNon2NTexture.imageHeight()) / 2.f);

	SDL_GL_SwapWindow(gWindow);
}