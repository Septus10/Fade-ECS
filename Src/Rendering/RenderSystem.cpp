#include "RenderSystem.hpp"
#include "../ECS/ComponentArray.hpp"

#include "Window.hpp"
#include "../ECS/EntityComponentManager.hpp"
#include "../ECS/Components/Transform.hpp"

namespace Rendering {

//===========================================================================//
RenderSystem::~RenderSystem()
{
    SDL_DestroyRenderer(Renderer_);
    SDL_Quit();
}
//===========================================================================//
bool RenderSystem::InitSystem()
{
    SDL_Window* wnd = Window_->GetWindowHandle();
    Renderer_ = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
    if (!Renderer_)
    {
        SDL_DestroyWindow(wnd);
        std::cout << "SDL renderer could not be created! SDL_Error: " << SDL_GetError() << "\nPress enter to continue...\n";
        SDL_Quit();
        return false;
    }
    return true;
}
//===========================================================================//
void RenderSystem::Tick(double DeltaTime)
{
	std::vector<ECS::Entity> ValidEntities =
		EntityManager_->GetEntitiesWithComponents(GetRequiredComponents());

	for (auto i = 0; i < ValidEntities.size(); i++)
	{
		ECS::Entity Cur = ValidEntities[i];
		Transform* Trans = EntityManager_->GetComponentFromEntity<Transform>(Cur);
		Window_->SetRect(Trans->PosX - 3, Trans->PosY - 3, 
            Trans->PosX + 3, Trans->PosY + 3, 0xFFFFFF);
	}
}
//===========================================================================//
void RenderSystem::FixedTick(double FixedDeltaTime)
{

}
//===========================================================================//
u64 RenderSystem::GetRequiredComponents() const
{
	return static_cast<u64>(ECS::CF_Transform);
}
//===========================================================================//
void RenderSystem::ReceiveEvent(ECS::EventBase& a_Event)
{

}
//===========================================================================//
void RenderSystem::SetWindowContext(Window* Wnd)
{
	Window_ = Wnd;
    ScreenSurface_ = Window_->GetScreenSurface();
}
//===========================================================================//

}

