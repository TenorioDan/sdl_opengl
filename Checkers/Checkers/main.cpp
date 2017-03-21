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
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						quit = game.manageInput(e.key);
					}
				}

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