#pragma once

#include "Character.h"

class Command
{
public:
	virtual ~Command() {}
	virtual void execute(Character& character) = 0;
};

class JumpCommand : public Command
{
public:
	void execute(Character& character)
	{
		character.jump();
	}
};

class ResetJumpCommand : public Command
{
public:
	void execute(Character& character)
	{
		character.resetJump();
	}
};

class MoveLeftCommand : public Command
{
public:
	void execute(Character& character)
	{
		character.applyHorizontalMovement(-1);
	}
};


class MoveRightCommand : public Command
{
public:
	void execute(Character& character)
	{
		character.applyHorizontalMovement(1);
	}
};


class StopCommand : public Command
{
public:
	void execute(Character& character)
	{
		character.reduceHorizontalMovement();
	}
};

class ResetPositionCommand : public Command
{
public:
	void execute(Character& character)
	{
		character.resetPosition();
	}
};

class AttackCommand : public Command
{
public:
	void execute(Character& character)
	{
		character.attack();
	}
};