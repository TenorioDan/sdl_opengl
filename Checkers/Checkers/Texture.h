#pragma once

#include "GL_helpers.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <string>
#include <IL/il.h>
#include "LFRect.h"

class Texture
{
public:
	// Initializes variables
	Texture();
	// Deallocates memory
	virtual ~Texture();

#pragma region OpenGL Methods

	// Creates a textures from the give pixels
	bool loadTextureFromPixels32(GLuint* pixels, GLuint imgWidth, GLuint imgHeight, GLuint texWidth, GLuint texHeight);
	bool loadTextureFromFile(std::string path);
	virtual void freeTexture();

	// Loads member pixels frok the given file
	bool loadPixelsFromFile(std::string path);

	// Creates a texture from the given file, pads image to have power-of-two dimensions, sets given RGBA value to RFFGFFBFFA00
	// in pixel data. If A = 0, only RGB components are compared
	bool loadTextureFromFileWithColorKey(std::string path, GLubyte r, GLubyte g, GLubyte b, GLubyte a = 000);

	// Creates a texure from the member pixels.
	// Deletes member pixels on success
	// Reports error to console if texture could not be created
	bool loadTextureFromPixels32();

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

	// Translate to given position and renders the texturea rea mapped to a quad
	// If given texture clip is NULL, the full image is rendered
	// If a stretch area is given, texture area is scaled to the streth area size
	void render(GLfloat x, GLfloat y, LFRect* clip = NULL);

	// getters
	GLuint getTextureID();
	GLuint textureWidth();
	GLuint textureHeight();
	GLuint imageWidth();
	GLuint imageHeight();

private:
	// Returns nearest power of two integer that is greater
	GLuint powerOfTwo(GLuint num);

	// Generates VBO and IBO to use for rendering
	void initVBO();

	// Frees VBO and IBO
	void freeVBO();

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

	// VBO IDs
	GLuint mVBOID;
	GLuint mIBOID;

	GLenum DEFAULT_TEXTURE_WRAP = GL_REPEAT;

#pragma endregion

};