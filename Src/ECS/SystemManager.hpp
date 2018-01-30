#pragma once

#include <vector>
#include <memory>

#include "EntityComponentManager.hpp"
#include "System.hpp"
#include "Event.hpp"

namespace ECS {

class SystemManager
{
public:
	SystemManager(EntityManager* eMgr) :
		EntityManager_(eMgr)
	{
	}

	template <typename T>
	T* RegisterSystem()
	{
		std::unique_ptr<T> System = std::make_unique<T>();
		System->SetEntityManager(EntityManager_);
		Systems_.push_back(move(System));
		return static_cast<T*>(Systems_.back().get());
	}

	void PreTickSystems();

	void TickSystems(double DeltaTime);

	void FixedTickSystems(double FixedDeltaTime);

	void SendEvent(EventBase& a_Event);

private:
	std::vector<std::unique_ptr<System>> Systems_;
	EntityManager* EntityManager_;

};

}
