#include "RenderSystem.hpp"
#include "../ECS/ComponentArray.hpp"

#include "Window.hpp"
#include "../ECS/EntityManager.hpp"
#include "../ECS/Components/Transform.hpp"

namespace Rendering {	

void RenderSystem::Tick(double DeltaTime)
{
	std::vector<ECS::Entity> ValidEntities =
		EntityManager_->GetEntitiesWithComponents(GetRequiredComponents());

	for (auto i = 0; i < ValidEntities.size(); i++)
	{
		ECS::Entity Cur = ValidEntities[i];
		Transform* Trans = EntityManager_->GetComponentFromEntity<Transform>(Cur);
		Window_->SetRect(Trans->PosX - 1, Trans->PosY - 1, Trans->PosX + 1, Trans->PosY + 1, 0xFFFFFF);
	}
}

void RenderSystem::FixedTick(double FixedDeltaTime)
{

}

u64 RenderSystem::GetRequiredComponents() const
{
	return static_cast<u64>(ECS::CF_Transform);
}

void RenderSystem::ReceiveEvent(ECS::EventBase* a_Event)
{

}

void RenderSystem::SetWindowContext(Window* Wnd)
{
	Window_ = Wnd;
}

}

