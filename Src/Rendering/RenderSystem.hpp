#pragma once
#include "../ECS/System.hpp"

namespace Rendering {
	
class Window;
class RenderSystem : public ECS::System
{
public:
	virtual void Tick(double DeltaTime) override;
	virtual void FixedTick(double FixedDeltaTime) override;
	virtual u64 GetRequiredComponents() const override;
	virtual void ReceiveEvent(ECS::EventBase* a_Event) override;

	void SetWindowContext(Window* Wnd);

private:
	Window* Window_;
};


}
