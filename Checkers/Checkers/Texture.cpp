#include "Texture.h"
#include <SDL_image.h>

Texture::Texture()
{
	// Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	positionX = 0;
	positionY = 0;
}

Texture::~Texture()
{
	// Dealocate texture
	free();
}

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
