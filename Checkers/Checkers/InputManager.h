#pragma once

#include "Command.h"
#include "Character.h"

#define JOYSTICK_LEFT 0
#define JOYSTICK_RIGHT 1
#define JOYSTICK_X_AXIS 0
#define JOYSTICK_Y_AXIS 1
#define JOYSTICK_DEADZONE 8000

class InputManager
{
public:
	enum InputState
	{
		GAME,
		MENU
	};

	InputManager();
	~InputManager();

	void swapStates();
	void setJoystick(SDL_Joystick* joystick);

	Command* handleInput();
	InputState CurrentState();

private:
	bool spaceReleased;

	GLfloat cameraX;
	GLfloat cameraY;

	InputState currentState;
	SDL_Joystick* gameController = NULL;

	// Key press commands
	Command* moveLeftStick_Left;
	Command* moveLeftStick_Right;
	Command* pressKeyP;
	Command* pressButtonA;
	Command* pressKeyEsc;

	// Key release commands
	Command* releaseLeftStick;
	Command* releaseButtonA;
};