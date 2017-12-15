#pragma once

#include "ComponentFamilies.hpp"

#include <SDL.h>

#define NUM_KEYS SDL_NUM_SCANCODES

struct Input
{
	Input()
	{
		for (int i = 0; i < NUM_KEYS; i++)
		{
			Keys[i] = false;
		}
	}

	static ECS::ComponentFamily GetComponentFamily()
	{
		return ECS::CF_Input;
	}

	bool Keys[NUM_KEYS];
};
