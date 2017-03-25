#pragma once

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
	
};