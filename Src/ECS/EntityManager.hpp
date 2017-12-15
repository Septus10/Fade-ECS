#pragma once

#include "Entity.hpp"
#include "ComponentArray.hpp"

#include "../Globals.h"

#include <iostream>
#include <vector>
#include <bitset>
#include <map>
#include <unordered_map>

namespace ECS {
 
class EntityManager
{
public:
	~EntityManager();

	Entity CreateEntity();

    template <typename T>
    T* AddComponent(const Entity EntityID)
    {
        if (EntityMap_.find(EntityID) == EntityMap_.end())
        {
            // error, can't find entity with ID
            std::cout << "Error: Can't find entity with id: " << EntityID << "\n";
            return nullptr;
        }
        EntityMap_[EntityID] |= static_cast<u64>(T::GetComponentFamily());

        auto& indices = IndexMap_[EntityID];
        indices.push_back(Components_.Size());
        return Components_.StoreNewComponent<T>(EntityID);
    }

	std::vector<Entity> GetEntitiesWithComponents(u64 Components);

	template <typename T>
	T* GetComponentFromEntity(Entity EntityID)
	{
		return Components_.GetComponentFromEntity<T>(EntityID);
	}

	template <typename T>
	std::vector<T*> GetComponentsOfType()
	{
		return Components_.GetComponentsOfType<T>();
    }

private:
	Entity generateEntityID() const;

private:
	ComponentArray<>									Components_;
    std::unordered_map<Entity, u64>                     EntityMap_;
    std::unordered_map<Entity, std::vector<usize>>      IndexMap_;

};

}
