#pragma once

#include "../ECS/System.hpp"
#include <queue>

namespace Gameplay {
	
class InputSystem : public ECS::System
{
public:
	virtual void Tick(double DeltaTime) override;
	virtual void FixedTick(double FixedDeltaTime) override;
	virtual u64 GetRequiredComponents() const override;
	virtual void ReceiveEvent(ECS::EventBase* a_Event) override;

private:
	std::queue<ECS::InputEvent> InputEventQueue;
};

}
