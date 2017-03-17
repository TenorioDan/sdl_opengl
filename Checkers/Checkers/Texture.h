#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <string>
#include <IL/il.h>
#include "GL_helpers.h"
#include "LFRect.h"

class Texture
{
public:
	// Initializes variables
	Texture();
	// Deallocates memory
	~Texture();

#pragma region OpenGL Methods

	// Creates a textures from the give pixels
	bool loadTextureFromPixels32(GLuint* pixels, GLuint imgWidth, GLuint imgHeight, GLuint texWidth, GLuint texHeight);
	bool loadTextureFromFile(std::string path);
	void freeTexture();

	// Gets member pixels from texture data
	bool lock();

	// Updates texture with member pixels
	bool unlock();

	// Returns member pixels
	GLuint* getPixelData32();

	// Returns pixel at given position
	GLuint getPixel32(GLuint x, GLuint y);

	// Sets pixel at given position
	void setPixel32(GLuint x, GLuint y, GLuint pixel);

	// Tranlsates to given position and renders textured quad
	void render(GLfloat x, GLfloat y, LFRect* clip = NULL);

	// getters
	GLuint getTextureID();
	GLuint textureWidth();
	GLuint textureHeight();
	GLuint imageWidth();
	GLuint imageHeight();

private:
	GLuint powerOfTwo(GLuint num);

	// Texture name
	GLuint mTextureID;

	// Current pixels
	GLuint* mPixels;
	
	// Texture Dimensions
	GLuint mTextureWidth;
	GLuint mTextureHeight;

	// Unpadded image dimensions
	GLuint mImageWidth;
	GLuint mImageHeight;

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