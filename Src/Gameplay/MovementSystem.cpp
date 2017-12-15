#include "MovementSystem.hpp"
#include "../ECS/EntityManager.hpp"
#include "../ECS/Components/Velocity.hpp"
#include "../ECS/Components/Transform.hpp"
#include "../ECS/Components/Input.hpp"

namespace Gameplay {
	
float g_Gravity = 9.f;
//===========================================================================//
void MovementSystem::Tick(double DeltaTime)
{
	std::vector<ECS::Entity> ValidEntities = 
		EntityManager_->GetEntitiesWithComponents(GetRequiredComponents());

	float fDeltaTime = static_cast<float>(DeltaTime);

	for (int i = 0; i < ValidEntities.size(); i++)
	{
		ECS::Entity Cur = ValidEntities[i];
		Transform*	Trans	= EntityManager_->GetComponentFromEntity<Transform>(Cur);
		Velocity*	Vel		= EntityManager_->GetComponentFromEntity<Velocity>(Cur);
		Input*		Inp		= EntityManager_->GetComponentFromEntity<Input>(Cur);

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
u64 MovementSystem::GetRequiredComponents() const
{
	return static_cast<u64>(
		ECS::CF_Transform | 
		ECS::CF_Input | 
		ECS::CF_Velocity);
}
//===========================================================================//
void MovementSystem::ReceiveEvent(ECS::EventBase* a_Event)
{
}
//===========================================================================//

}
