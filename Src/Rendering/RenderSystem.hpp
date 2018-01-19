#pragma once
#include "../ECS/System.hpp"
#include <SDL.h>
#include <vector>

namespace Rendering {
	
class Window;
class RenderSystem : public ECS::System
{
public:
    ~RenderSystem() override;

    virtual bool InitSystem() override;
    virtual void PreTick() override {}
	virtual void Tick(double DeltaTime) override;
	virtual void FixedTick(double FixedDeltaTime) override;
	virtual std::vector<usize> GetRequiredComponents() const override;
	virtual void ReceiveEvent(ECS::EventBase& a_Event) override;

	void SetWindowContext(Window* Wnd);

private:
	Window* Window_;

    SDL_Renderer*	Renderer_		= nullptr;
    SDL_Surface*	ScreenSurface_	= nullptr;
};


}
