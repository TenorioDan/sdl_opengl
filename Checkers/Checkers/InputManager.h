#pragma once

#include "Command.h"
#include "Character.h"

#define JOYSTICK_LEFT 0
#define JOYSTICK_RIGHT 1
#define JOYSTICK_X_AXIS 0
#define JOYSTICK_Y_AXIS 1
#define JOYSTICK_X_DEADZONE 8000
#define JOYSTICK_Y_DEADZONE 20000

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

	std::vector<Command*> handleInput();
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
	Command* pressButtonX;
	Command* pressButtonLeftShoulder;
	Command* moveLeftStick_Up;
	Command* moveLeftStick_Down;

	// Key release commands
	Command* releaseLeftStick_X_Axis;
	Command* releaseLeftStick_Y_Axis;
	Command* releaseButtonA;
	Command* releaseButtonLeftShoulder;
};