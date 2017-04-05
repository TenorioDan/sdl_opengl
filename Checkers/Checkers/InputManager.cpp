#include "InputManager.h"
#include <SDL.h>
#include <math.h>

InputManager::InputManager()
{
	spaceReleased = true;

	// Set command objects
	moveLeftStick_Left = new MoveLeftCommand();
	moveLeftStick_Right = new MoveRightCommand();
	pressKeyP = new ResetPositionCommand();
	pressButtonA = new JumpCommand();
	pressButtonX = new AttackCommand();

	releaseLeftStick = new StopCommand();
	releaseButtonA = new ResetJumpCommand();
}

Command* InputManager::handleInput()
{
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		// Joystick Axis Input Handling
		if (e.type == SDL_JOYAXISMOTION)
		{
			if (e.jaxis.which == JOYSTICK_LEFT)
			{
				if (e.jaxis.axis == JOYSTICK_X_AXIS)
				{
					if (e.jaxis.value > JOYSTICK_DEADZONE) { return moveLeftStick_Right; }
					if (e.jaxis.value < -JOYSTICK_DEADZONE) { return moveLeftStick_Left; }
					if (std::abs(e.jaxis.value) <= JOYSTICK_DEADZONE) { return releaseLeftStick; }
				}

			}
		}
		// Joystick Button Down Handling
		else if (e.type == SDL_JOYBUTTONDOWN)
		{
			switch (e.jbutton.button)
			{
			case 0:
				return pressButtonA;
			case 2:
				return pressButtonX;
			}
		}
		// Joystick Button Up Handling
		else if (e.type == SDL_JOYBUTTONUP)
		{
			switch (e.jbutton.button)
			{
			case 0:
				return releaseButtonA;
			}
		}
	}

	return NULL;
}

void InputManager::setJoystick(SDL_Joystick* joystick)
{
	gameController = joystick;
}

void InputManager::swapStates()
{
	currentState = currentState == GAME ? MENU : GAME;

	// TODO: Swap command objects depending on state
}

InputManager::InputState InputManager::CurrentState()
{
	return currentState;
}

InputManager::~InputManager()
{
	delete moveLeftStick_Left;
	delete moveLeftStick_Right;
	delete pressKeyP;
	delete pressButtonA;
	delete pressKeyEsc;

	delete releaseLeftStick;
	delete releaseButtonA;
}