#include "EntityComponentManager.hpp"

#include <limits>

namespace ECS {

//===========================================================================//	
EntityManager::~EntityManager()
{
}
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
	usize ComponentBits = 0;
	for (auto& it: Components)
	{
		ComponentBits |= BitMap_[it];
	}

	std::vector<Entity> Entities(EntityMap_.size());
	for (auto& it : EntityMap_)
	{
		if ((it.second & ComponentBits) == ComponentBits)
		{
			Entities.push_back(it.first);
		}		
	}
	return Entities;
}
//===========================================================================//
usize EntityManager::RegisterComponentType(usize ComponentHash)
{
	auto LastBit = BitMap_.end()->second;
	LastBit <<= 1;

#ifdef _DEBUG
	if (LastBit == std::numeric_limits<usize>::max() - 1)
	{
		throw std::exception("Unable to register component, all bits are currently in use\nConsider using more bits for components.");
	}
#endif

	BitMap_.insert({ ComponentHash, LastBit });

	return LastBit;
}
//===========================================================================//
Entity EntityManager::generateEntityID() const
{
	return static_cast<Entity>(EntityMap_.size());
}
//===========================================================================//

}
