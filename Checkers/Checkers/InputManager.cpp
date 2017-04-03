#include "InputManager.h"
#include <SDL.h>

InputManager::InputManager()
{
	spaceReleased = true;

	// Set command objects
	pressKeyA = new MoveLeftCommand();
	pressKeyD = new MoveRightCommand();
	pressKeyP = new ResetPositionCommand();
	pressKeySpace = new JumpCommand();
	releaseKeyA = new StopCommand();
	releaseKeySpace = new ResetJumpCommand();
}

void InputManager::update()
{
	
}

Command* InputManager::handleInput()
{
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		SDL_Keycode key = e.key.keysym.sym;

		//User requests quit
		if (e.type == SDL_KEYUP)
		{
			if (currentState == GAME)
			{
				// if key is a or d then stop moving
				if (key == SDLK_a || key == SDLK_d) { return releaseKeyA; }
				if (e.key.keysym.sym == SDLK_SPACE) { return releaseKeySpace; }
			}
		}
		else if (e.type == SDL_KEYDOWN)
		{
			if (currentState == GAME)
			{
				// if key is a or d then start moving left to right
				if (key == SDLK_a) { return pressKeyA; }
				if (key == SDLK_d) { return pressKeyD; }
				if (key == SDLK_p) { return pressKeyP; }
				if (key == SDLK_SPACE) { return pressKeySpace; }
			}
			else if (currentState == MENU)
			{

			}
		}
		else if (e.type == SDL_QUIT)
		{

		}
	}

	return NULL;
}

InputManager::InputState InputManager::CurrentState()
{
	return currentState;
}

void InputManager::swapStates()
{
	currentState = currentState == GAME ? MENU : GAME;
}