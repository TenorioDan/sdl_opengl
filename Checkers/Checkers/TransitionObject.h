#pragma once

#include <string>
#include "Collider.h"

class TransitionObject
{
public:
	TransitionObject();

private:
	std::string levelToTransitionTo;
	std::string tileToTransitionTo;
	Collider collider;

};