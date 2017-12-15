#include "InputSystem.hpp"

#include "../ECS/Components/ComponentFamilies.hpp"

#include <iostream>

namespace Gameplay {
	
void InputSystem::Tick(double DeltaTime)
{
	
}

void InputSystem::FixedTick(double FixedDeltaTime)
{
	
}

u64 InputSystem::GetRequiredComponents() const
{
	return static_cast<u64>(ECS::CF_Input);
}

void InputSystem::ReceiveEvent(ECS::EventBase* a_Event)
{
	ECS::InputEvent* InputEvent = dynamic_cast<ECS::InputEvent*>(a_Event);
	if (InputEvent)
	{
		InputEventQueue.push(ECS::InputEvent(*InputEvent));
	}
}

}
