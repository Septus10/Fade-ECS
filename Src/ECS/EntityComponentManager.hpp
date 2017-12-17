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
private:
    template <typename T>
    bool AddComponentToEntity(const Entity EntityID)
    {
        if (EntityMap_.find(EntityID) == EntityMap_.end())
        {
            // error, can't find entity with ID
            std::cout << "Error: Can't find entity with id: " << EntityID << "\n";
            return false;
        }

        EntityMap_[EntityID] |= static_cast<u64>(T::GetComponentFamily());
        return true;
    }

public:
	~EntityManager();

	Entity CreateEntity();

    template <typename T>
    T* AddComponent(const Entity EntityID)
    {       
        if (!AddComponentToEntity<T>(EntityID))
        {
            return nullptr;
        }
        auto& indices = IndexMap_[EntityID];
        indices.push_back(Components_.Size());
        return Components_.StoreNewComponent<T>(EntityID);
    }

    template <typename T>
    T* AddSingletonComponent(const Entity EntityID)
    {
        if (!AddComponentToEntity<T>(EntityID))
        {
            return nullptr;
        }

        return Components_.StoreNewSingletonComponent<T>(EntityID);
    }

	std::vector<Entity> GetEntitiesWithComponents(u64 Components);

	template <typename T>
	T* GetComponentFromEntity(const Entity EntityID)
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
