#pragma once

#include "../Globals.h"
#include "Event.hpp"

namespace ECS {
	
class EntityManager;
class System
{
public:
	System() : EntityManager_(nullptr)
	{ }

	virtual ~System() = default;

    virtual bool InitSystem() = 0;

    virtual void PreTick() = 0;

	virtual void Tick(double DeltaTime) = 0;

	virtual void FixedTick(double FixedDeltaTime) = 0;

	virtual u64 GetRequiredComponents() const = 0;

	virtual void ReceiveEvent(EventBase* a_Event) = 0;


	void SetEntityManager(EntityManager* Mgr)
	{
		EntityManager_ = Mgr;
	}

protected:
	EntityManager* EntityManager_;

};

}