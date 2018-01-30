#pragma once

#include "../Globals.h"

namespace ECS {

enum Events
{
	E_InputEvent = 1
};

struct EventBase
{
	virtual ~EventBase() = default;
};

template <u32 EvID>
struct Event : public EventBase
{
	Event() :
		EventID(EvID)
	{
	}

	virtual ~Event() = default;

	u32 EventID;
};

enum InputAction
{
	IA_Pressed,
	IA_Released,
	IA_Repeat
};

struct InputEvent : public Event<E_InputEvent>
{
	u32				Key;
	InputAction		Action;
};

}