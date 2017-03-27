#include "InputManager.h"
#include <SDL.h>

InputManager::InputManager()
{
	spaceReleased = true;
}

void InputManager::update()
{
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_KEYUP)
		{
			if (currentState == GAME)
			{
				// if key is a or d then stop moving
				if (e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_d)
				{
					inputCharacter->reduceHorizontalMovement();
				}
				if (e.key.keysym.sym == SDLK_SPACE)
				{
					spaceReleased = true;
				}
			}
		}
		else if(e.type == SDL_KEYDOWN)
		{
			if (currentState == GAME)
			{
				// if key is a or d then start moving left to right
				switch (e.key.keysym.sym)
				{
				case SDLK_a:
					inputCharacter->applyHorizontalMovement(-1.f);
					break;
				case SDLK_d:
					inputCharacter->applyHorizontalMovement(1.f);
					break;
				case SDLK_p:
					inputCharacter->resetPosition();
				}

				if (e.key.keysym.sym == SDLK_SPACE && spaceReleased)
				{
					inputCharacter->jump();
					spaceReleased = false;
				}

			}
			else if (currentState == MENU)
			{

			}
		}
		else if (e.type == SDL_QUIT)
		{
			
		}
	}
}


// TODO: Do i really need an accessor for input state?
InputManager::InputState InputManager::CurrentState()
{
	return currentState;
}

void InputManager::swapStates()
{
	currentState = currentState == GAME ? MENU : GAME;
}

void InputManager::setInputCharacter(Character& c)
{
	inputCharacter = &c;
}
