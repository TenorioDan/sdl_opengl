#pragma once

#include "Command.h"
#include "Character.h"

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
	Command* handleInput();
	InputState CurrentState();

private:
	bool spaceReleased;

	GLfloat cameraX;
	GLfloat cameraY;

	InputState currentState;
	Character* inputCharacter;
	
	// Key press commands
	Command* pressKeyA;
	Command* pressKeyD;
	Command* pressKeyP;
	Command* pressKeySpace;
	Command* pressKeyEsc;

	// Key release commands
	Command* releaseKeyA;
	Command* releaseKeyD;
	Command* releaseKeySpace;
};