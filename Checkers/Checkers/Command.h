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
	virtual void execute(Character& character)
	{
		character.jump();
	}
};

class ResetJumpCommand : public Command
{
public:
	virtual void execute(Character& character)
	{
		character.resetJump();
	}
};

class MoveLeftCommand : public Command
{
public:
	virtual void execute(Character& character)
	{
		character.applyHorizontalMovement(-1);
	}
};


class MoveRightCommand : public Command
{
public:
	virtual void execute(Character& character)
	{
		character.applyHorizontalMovement(1);
	}
};


class StopCommand : public Command
{
public:
	virtual void execute(Character& character)
	{
		character.reduceHorizontalMovement();
	}
};

class ResetPositionCommand : public Command
{
public:
	virtual void execute(Character& character)
	{
		character.resetPosition();
	}
};