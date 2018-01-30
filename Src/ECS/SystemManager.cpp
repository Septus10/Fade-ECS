#include "SystemManager.hpp"

namespace ECS {

//===========================================================================//
void SystemManager::PreTickSystems()
{
	for (auto& System : Systems_)
	{
		System->PreTick();
	}
}
//===========================================================================//
void SystemManager::TickSystems(double DeltaTime)
{
	for (auto& System : Systems_)
	{
		System->Tick(DeltaTime);
	}
}
//===========================================================================//
void SystemManager::FixedTickSystems(double FixedDeltaTime)
{
	for (auto& System : Systems_)
	{
		System->FixedTick(FixedDeltaTime);
	}
}
//===========================================================================//
void SystemManager::SendEvent(EventBase& a_Event)
{
	for (auto& System : Systems_)
	{
		System->ReceiveEvent(a_Event);
	}
}
//===========================================================================//

}