#include "InputManager.h"
#include <SDL.h>
#include <math.h>
#include <vector>

InputManager::InputManager()
{
	spaceReleased = true;

	// Set command objects
	moveLeftStick_Left = new MoveLeftCommand();
	moveLeftStick_Right = new MoveRightCommand();
	moveLeftStick_Up = new AimUpCommand();
	moveLeftStick_Down = new AimDownCommand();

	pressKeyP = new ResetPositionCommand();
	pressButtonA = new JumpCommand();
	pressButtonX = new AttackCommand();
	pressButtonLeftShoulder = new AimCommand();

	releaseLeftStick_X_Axis = new StopMovementCommand();
	releaseLeftStick_Y_Axis = new ReleaseAimDirectionCommand();
	releaseButtonA = new ResetJumpCommand();
	releaseButtonLeftShoulder = new StopAimCommand();
}

std::vector<Command*> InputManager::handleInput()
{
	//Event handler
	SDL_Event e;
	std::vector<Command*> commandQueue;

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
					if (e.jaxis.value > JOYSTICK_X_DEADZONE) { commandQueue.push_back(moveLeftStick_Right); }
					if (e.jaxis.value < -JOYSTICK_X_DEADZONE) { commandQueue.push_back(moveLeftStick_Left); }
					if (std::abs(e.jaxis.value) <= JOYSTICK_X_DEADZONE) { commandQueue.push_back(releaseLeftStick_X_Axis); }
				}

				if (e.jaxis.axis == JOYSTICK_Y_AXIS)
				{
					if (e.jaxis.value > JOYSTICK_Y_DEADZONE) { commandQueue.push_back(moveLeftStick_Up); }
					if (e.jaxis.value < -JOYSTICK_Y_DEADZONE) { commandQueue.push_back(moveLeftStick_Down); }
					if (std::abs(e.jaxis.value) <= JOYSTICK_Y_DEADZONE) { commandQueue.push_back(releaseLeftStick_Y_Axis); }
				}
			}
		}
		// Joystick Button Down Handling
		else if (e.type == SDL_JOYBUTTONDOWN)
		{
			switch (e.jbutton.button)
			{
			// TODO: Clean up magic numbers
			case 0:
				commandQueue.push_back(pressButtonA);
				break;
			case 2:
				commandQueue.push_back(pressButtonX);
				break;
			case 4:
				commandQueue.push_back(pressButtonLeftShoulder);
				break;
			case 6:
				commandQueue.push_back(pressKeyP);
				break;
			}
		}
		// Joystick Button Up Handling
		else if (e.type == SDL_JOYBUTTONUP)
		{
			switch (e.jbutton.button)
			{
			case 0:
				commandQueue.push_back(releaseButtonA);
				break;
			case 4:
				commandQueue.push_back(releaseButtonLeftShoulder);
				break;
			}
		}
	}

	return commandQueue;
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

	delete releaseLeftStick_X_Axis;
	delete releaseLeftStick_Y_Axis;
	delete releaseButtonA;
}