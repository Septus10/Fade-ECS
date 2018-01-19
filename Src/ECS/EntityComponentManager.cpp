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
std::vector<Entity> EntityManager::GetEntitiesWithComponents(std::vector<usize> Components)
{
	std::vector<Entity> Entities(EntityMap_.size());
	for (auto& it : EntityMap_)
	{
		usize RequiredComponents = Components.size();
		for (auto& hash : it.second)
		{
			for (auto& hash2 : Components)
			{
				if (hash == hash2)
				{
					RequiredComponents--;
				}
			}
		}

		if (RequiredComponents == 0)
		{
			Entities.push_back(it.first);
		}
	}
	Entities.shrink_to_fit();
	return Entities;
}
//===========================================================================//
Entity EntityManager::generateEntityID() const
{
	return static_cast<Entity>(EntityMap_.size());
}
//===========================================================================//

}
