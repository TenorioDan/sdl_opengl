#pragma once

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
	void update();
	void swapStates();
	InputState CurrentState();
	void setInputCharacter(Character& c);

private:
	InputState currentState;
	Character* inputCharacter;
	bool spaceReleased;

};