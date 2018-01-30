#include "InputSystem.hpp"

#include "../ECS/Components/Input.hpp"
#include "../ECS/Event.hpp"
#include "../ECS/EntityComponentManager.hpp"

#include <iostream>

namespace Gameplay {

//===========================================================================//
void InputSystem::PreTick()
{
	auto InputComponents = EntityManager_->GetComponentsOfType<Input>();
	for (int i = 0; i < InputEventQueue.size(); i++)
	{
		ECS::InputEvent Cur = InputEventQueue.front();
		for (auto& Comp : InputComponents)
		{
			auto* Key = &Comp->Keys[Cur.Key];
			switch (Cur.Action)
			{
			case ECS::IA_Pressed:
				*Key = true;
				break;
			case ECS::IA_Released:
				*Key = false;
				break;
			default:
				break;
			}
		}
		InputEventQueue.pop();
	}
}
//===========================================================================//
void InputSystem::Tick(double DeltaTime)
{

}
//===========================================================================//
void InputSystem::FixedTick(double FixedDeltaTime)
{

}
//===========================================================================//
void InputSystem::ReceiveEvent(ECS::EventBase& a_Event)
{
	ECS::InputEvent& InputEvent = dynamic_cast<ECS::InputEvent&>(a_Event);
	InputEventQueue.push(ECS::InputEvent(InputEvent));
}
//===========================================================================//

}
