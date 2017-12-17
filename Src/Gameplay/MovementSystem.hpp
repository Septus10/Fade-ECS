#pragma once
#include <vector>
#include "../ECS/System.hpp"

namespace Gameplay {
	
class MovementSystem : public ECS::System
{
public:
    virtual bool InitSystem() override { return true; }

    virtual void PreTick() override {}

	virtual void Tick(double DeltaTime) override;

	virtual void FixedTick(double FixedDeltaTime) override;

	virtual u64 GetRequiredComponents() const;

	virtual void ReceiveEvent(ECS::EventBase* a_Event) override;
};

}
