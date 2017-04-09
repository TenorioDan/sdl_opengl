#pragma once

#include "Character.h"

class Command
{
public:
	virtual ~Command() {}
	virtual void execute(Character& character) = 0;
};


#pragma region  Movement Commands

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

class StopMovementCommand : public Command
{
public:
	void execute(Character& character)
	{
		character.reduceHorizontalMovement();
	}
};

#pragma endregion

class AimUpCommand : public Command
{
public:
	void execute(Character& character)
	{
		character.applyVerticalAimDirection(-1);
	}
};


class AimDownCommand : public Command
{
public:
	void execute(Character& character)
	{
		character.applyVerticalAimDirection(1);
	}
};

class ReleaseAimDirectionCommand : public Command
{
	void execute(Character& character)
	{
		character.releaseVerticalAimDirection();
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

class AimCommand : public Command
{
public:
	void execute(Character& character)
	{
		character.aim();
	}
};

class StopAimCommand : public Command
{
public:
	void execute(Character& character)
	{
		character.stopAiming();
	}
};