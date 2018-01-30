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
	inline bool DoesComponentExist(usize ComponentHash)
	{
		if (BitMap_.find(ComponentHash) != BitMap_.end())
		{
			return true;
		}
		return false;
	}

	usize RegisterComponentType(usize ComponentHash);

	template <typename T>
	bool AddComponentToEntity(const Entity EntityID)
	{
		if (EntityMap_.find(EntityID) == EntityMap_.end())
		{
			// error, can't find entity with ID
			std::cout << "Error: Can't find entity with id: " << EntityID << "\n";
			return false;
		}

		auto hash = typeid(T).hash_code();
		usize bit;
		if (!DoesComponentExist(hash))
		{
			bit = RegisterComponentType(hash);
		}
		else
		{
			bit = BitMap_[hash];
		}

		EntityMap_[EntityID] |= bit;
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

	std::vector<Entity> GetEntitiesWithComponents(std::vector<usize> Components);

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

	template <typename... Types>
	std::vector<std::tuple<Types*...>> GetComponentsOfTuple()
	{
		GetEntitiesWithComponents(typeid(Types).hash_code...);
		return std::vector<std::tuple<Types*...>>();
	}

private:
	Entity generateEntityID() const;

private:
	ComponentArray<>									Components_;
	std::map<usize, usize>								BitMap_;
	std::unordered_map<Entity, usize>					EntityMap_;
	std::unordered_map<Entity, std::vector<usize>>      IndexMap_;

};

}
