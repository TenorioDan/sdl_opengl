#include "Texture.h"
#include "Game.h"

int main(int argc, char* args[])
{
	Game game;
	// Start up SDL and create window
	if (!game.init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!game.loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!game.quit)
			{
				// Render the game
				game.update();
				game.render();
			}
		}
	}

	//Free resources and close SDL
	game.close();

	return 0;
}