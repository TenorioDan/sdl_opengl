#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <string>
#include "GL_helpers.h"

class Texture
{
public:
	// Initializes variables
	Texture();
	// Deallocates memory
	~Texture();

#pragma region OpenGL Methods

	// Creates a textures from the give pixels
	bool loadTextureFromPixels32(GLuint* pixels, GLuint width, GLuint height);

	void freeTexture();

	// Tranlsates to given position and renders textured quad
	void render(GLfloat x, GLfloat y);

	// getters
	GLuint getTextureID();
	GLuint textureWidth();
	GLuint textureHeight();

private:
	// Texture name
	GLuint mTextureID;
	
	// Texture Dimensions
	GLuint mTextureWidth;
	GLuint mTextureHeight;

#pragma endregion

#pragma region SDL Methods

public:
	// Loads image at specified path
	bool loadFromFile(std::string path, SDL_Renderer* renderer);

	// Dealocates texture
	void free();

	// Renders texture at given point
	void render(SDL_Renderer* renderer, int renderPosX, int renderPosY);
	int getWidth();
	int getHeight();
	int getX();
	int getY();

private:
	// The actual hardware texture
	SDL_Texture* mTexture;

	// Image dimension
	int mWidth;
	int mHeight;
	int positionX;
	int positionY;

#pragma endregion


};