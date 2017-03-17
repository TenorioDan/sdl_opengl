#include "Texture.h"
#include <SDL_image.h>
#include <IL/il.h>;
#include <IL/ilu.h>

#pragma region OpenGL Methods

Texture::Texture()
{
	// Initialize texture ID
	mTextureID = 0;

	// Initialize image dimensions
	mImageWidth = 0;
	mImageHeight = 0;

	// Initialize texture dimensions
	mTextureWidth = 0;
	mTextureHeight = 0;
}

Texture::~Texture()
{
	// Dealocate texture
	freeTexture();
}

void Texture::freeTexture()
{
	// Delete texture
	if (mTextureID != 0)
	{
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}

	mTextureWidth = 0;
	mTextureHeight = 0;
}


bool Texture::loadTextureFromFile(std::string path)
{
	bool textureLoaded = false;

	// Generate and set current image ID
	ILuint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);


	//load iamge
	ILboolean success = ilLoadImage(path.c_str());

	// Image loaded successfully
	if (success == IL_TRUE)
	{
		// Convert image to RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (success == IL_TRUE)
		{
			// Initialize dimensions
			GLuint imgWidth = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
			GLuint imgHeight = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

			// Calculate required texture dimensions
			GLuint texWidth = powerOfTwo(imgWidth);
			GLuint texHeight = powerOfTwo(imgHeight);

			if (imgWidth != texWidth || imgHeight != texHeight)
			{
				// Place image at upper left
				iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);

				// Resize image
				iluEnlargeCanvas((int)texWidth, (int)texHeight, 1);
			}

			// Create texture from file pixels
			textureLoaded = loadTextureFromPixels32((GLuint*)ilGetData(), imgWidth, imgHeight, texWidth, texHeight);
		}

		// Delete file from memory
		ilDeleteImages(1, &imgID);
	}

	// Report error
	if (!textureLoaded)
	{
		printf("Unable to laod %s\n", path.c_str());
	}

	return textureLoaded;
}

// New code for padding non-power-of-two textures
bool Texture::loadTextureFromPixels32(GLuint* pixels, GLuint imgWidth, GLuint imgHeight, GLuint texWidth, GLuint texHeight)
{
	// Free texture if it exists
	freeTexture();
	
	// Get image dimensions
	mImageWidth = imgWidth;
	mImageHeight = imgHeight;
	mTextureWidth = texWidth;
	mTextureHeight = texHeight;

	// Generate texture ID
	glGenTextures(1, &mTextureID);

	// Bind texture ID
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// Generate Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Ubind Texture
	glBindTexture(GL_TEXTURE_2D, NULL);

	//Check for error
	bool success = true;
	checkGL_Error(glGetError(), success);

	return success;
	
}

void Texture::render(GLfloat x, GLfloat y, LFRect* clip)
{
	// if the texture exists
	if (mTextureID != 0)
	{
		// Remove any previous transformations
		glLoadIdentity();

		// Texture coordinates
		GLfloat texTop = 0.f;
		GLfloat texBottom = (GLfloat)mImageHeight / (GLfloat)mTextureHeight;
		GLfloat texLeft = 0.f;
		GLfloat texRight = (GLfloat)mImageWidth / (GLfloat)mTextureWidth;

		// Vertex Coordinates
		GLfloat quadWidth = mImageWidth;
		GLfloat quadHeight = mImageHeight;

		// Handle clipping
		if (clip != NULL)
		{
			// Texture coordinates
			texLeft = clip->x / mTextureWidth;
			texRight = (clip->x + clip->w) / mTextureWidth;
			texTop = clip->y / mTextureHeight;
			texBottom = (clip->y + clip->h) / mTextureHeight;

			// Vertex coordinates
			quadWidth = clip->w;
			quadHeight = clip->h;
		}

		// move to rendering point
		glTranslatef(x, y, 0.f);

		// Set texture ID
		glBindTexture(GL_TEXTURE_2D, mTextureID);

		// Render textured quad
		glBegin(GL_QUADS);
			glTexCoord2f(texLeft,  texTop);    glVertex2f(0.f,       0.f);
			glTexCoord2f(texRight, texTop);    glVertex2f(quadWidth, 0.f);
			glTexCoord2f(texRight, texBottom); glVertex2f(quadWidth, quadHeight);
			glTexCoord2f(texLeft,  texBottom); glVertex2f(0.f,       quadHeight);
		glEnd();
	}
}

GLuint Texture::powerOfTwo(GLuint num)
{
	if (num != 0)
	{
		num--;
		num |= (num >> 1); // Or first two bits
		num |= (num >> 2); // Or next 2 bits
		num |= (num >> 4); // Or next 4 bits
		num |= (num >> 8); // Or next 4 bits
		num |= (num >> 16); // or next 16 bits
		num++;
	}

	return num;
}


GLuint Texture::getTextureID()
{
	return mTextureID;
}

GLuint Texture::textureWidth()
{
	return mTextureWidth;
}

GLuint Texture::textureHeight()
{
	return mTextureHeight;
}

GLuint Texture::imageWidth()
{
	return mImageWidth;
}

GLuint Texture::imageHeight()
{
	return mImageHeight;
}

#pragma endregion

#pragma region STL Methods


bool Texture::loadFromFile(std::string path, SDL_Renderer*renderer)
{
	free();

	// The final texture
	SDL_Texture* newTexture = NULL;

	// Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		// Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0, 0xFF));

		// Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture form %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			// Get image deimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	// Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void Texture::free()
{
	// Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture::render(SDL_Renderer* renderer, int renderPosX, int renderPosY)
{
	// Set rendering space and render to screen
	SDL_Rect renderQuad = { renderPosX, renderPosY, mWidth, mHeight };
	int success = SDL_RenderCopy(renderer, mTexture, NULL, &renderQuad);
	//printf("%d\n", success);
	
	if (success == -1)
	{
		printf("%s", SDL_GetError());
	}
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}

int Texture::getX()
{
	return positionX;
}

int Texture::getY()
{
	return positionY;
}

#pragma endregion