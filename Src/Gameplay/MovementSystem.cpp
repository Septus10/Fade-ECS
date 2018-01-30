#include "MovementSystem.hpp"
#include "../ECS/EntityComponentManager.hpp"
#include "../ECS/Components/Velocity.hpp"
#include "../ECS/Components/Transform.hpp"
#include "../ECS/Components/Input.hpp"

namespace Gameplay {

float g_Gravity = 9.f;

struct MovementTuple
{
	Transform*	Trans;
	Velocity*	Vel;
	Input*		Inp;
};

//===========================================================================//
void MovementSystem::Tick(double DeltaTime)
{
	float fDeltaTime = static_cast<float>(DeltaTime);

	auto MovementComponents = EntityManager_->GetComponentsOfTuple<Transform, Velocity, Input>();

	for (auto& it: MovementComponents)
	{
		Transform*	Trans	= std::get<0>(it);
		Velocity*	Vel		= std::get<1>(it);
		Input*		Inp		= std::get<2>(it);

		if (Inp->Keys[SDL_SCANCODE_D] || Inp->Keys[SDL_SCANCODE_RIGHT])
		{
			Trans->PosX += (Vel->VelX * fDeltaTime);
		}
		else if (Inp->Keys[SDL_SCANCODE_A] || Inp->Keys[SDL_SCANCODE_LEFT])
		{
			Trans->PosX -= (Vel->VelX * fDeltaTime);
		}

		if (Inp->Keys[SDL_SCANCODE_W] || Inp->Keys[SDL_SCANCODE_UP])
		{
			Trans->PosY -= (Vel->VelY * fDeltaTime);
		}
		else if (Inp->Keys[SDL_SCANCODE_S] || Inp->Keys[SDL_SCANCODE_DOWN])
		{
			Trans->PosY += (Vel->VelY * fDeltaTime);
		}
	}
}
//===========================================================================//
void MovementSystem::FixedTick(double FixedDeltaTime)
{

}
//===========================================================================//
void MovementSystem::ReceiveEvent(ECS::EventBase& a_Event)
{
}
//===========================================================================//

}
