#pragma once

#include <SDL.h>
#include <string>
class Texture
{
public:
	// Initializes variables
	Texture();
	// Deallocates memory
	~Texture();

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


};