#pragma once

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

	bool Keys[NUM_KEYS];
};
