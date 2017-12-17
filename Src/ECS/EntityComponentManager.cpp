#include "EntityComponentManager.hpp"

namespace ECS {

//===========================================================================//	
EntityManager::~EntityManager()
{ }
//===========================================================================//
Entity EntityManager::CreateEntity()
{
	Entity ID = generateEntityID();
	EntityMap_.insert(std::pair<Entity, u64>(ID, 0));
	IndexMap_.insert(
		std::pair<Entity, std::vector<usize>>(ID, std::vector<usize>()));
	return ID;
}
//===========================================================================//
std::vector<Entity> EntityManager::GetEntitiesWithComponents(u64 Components)
{
	std::vector<Entity> Entities;
	for (auto& it : EntityMap_)
	{
		if ((it.second & Components) == Components)
		{
			Entities.push_back(it.first);
		}
	}

	return Entities;
}
//===========================================================================//
Entity EntityManager::generateEntityID() const
{
	return static_cast<Entity>(EntityMap_.size());
}
//===========================================================================//

}
