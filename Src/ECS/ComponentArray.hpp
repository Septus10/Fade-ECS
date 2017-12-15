#pragma once

#include "../Globals.h"
#include "Entity.hpp"
#include "Components/ComponentFamilies.hpp"
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <iostream>

namespace ECS {
	
class Allocator
{
public:
	Pointer Allocate(usize Size)
	{
		Pointer ptr = std::malloc(Size);
		std::cout << "Allocated memory of size (" << Size << ") at address: " << ptr << "\n";
		return ptr;
	}

	void Free(Pointer Address)
	{
		if (Address)
		{
			std::cout << "Freeing memory at address: " << Address << "\n";
			return std::free(Address);			
		}		
	}
};

struct ComponentData
{
	ComponentFamily	Family_;
	usize			Offset_;
};

template <class Alloc = ECS::Allocator>
class ComponentArray
{
private:
	usize							MaxSize_;

	Pointer							Data_;
	usize							DataSize_;
	std::unordered_map
		<Entity, std::vector<ComponentData>>		EntityComponentOffsets_;
	usize							NumComponents_;
	Alloc							Alloc_;

private:
	void ResizeFromEmpty(usize NewSize)
	{
		Data_ = Alloc_.Allocate(NewSize);
		MaxSize_ = NewSize;
	}

	void ResizeFromContaining(usize NewSize)
	{
		Pointer Temp = Data_;
		Data_ = Alloc_.Allocate(NewSize);
		std::memcpy(Data_, Temp, DataSize_);
		MaxSize_ = NewSize;
		Alloc_.Free(Temp);
	}

	void Resize(usize NewSize)
	{
		if (Data_)
		{
			ResizeFromContaining(NewSize);
		}
		else
		{
			ResizeFromEmpty(NewSize);
		}		
	}

public:
	ComponentArray()
	{
		Data_			= nullptr;
		MaxSize_		= 0;
		DataSize_		= 0;
		NumComponents_	= 0;
	}
	
	~ComponentArray()
	{
		Alloc_.Free(Data_);
	}

	template <typename T>
	T* StoreNewComponent(Entity OwnerID)
	{
		if (T* Test = GetComponentFromEntity<T>(OwnerID))
		{
			return Test;
		}

		usize ComponentSize = sizeof(T);
		if (ComponentSize + DataSize_ > MaxSize_)
		{
			Resize(DataSize_ + ComponentSize);
		}

		usize IntAddress = reinterpret_cast<usize>(Data_) + DataSize_;
		Pointer Address = reinterpret_cast<void*>(IntAddress);
		T* Comp = static_cast<T*>(new(Address) T());
		std::cout << "Component created at address: " << Address << " with size (" << ComponentSize 
		<< ") next available memory at address: " << reinterpret_cast<void*>(IntAddress + ComponentSize) << "\n";
		
		ComponentData EntityCompData;
		EntityCompData.Family_	= T::GetComponentFamily();
		EntityCompData.Offset_ = DataSize_;
		if (EntityComponentOffsets_.find(OwnerID) == EntityComponentOffsets_.end())
		{
			EntityComponentOffsets_.insert(
				std::pair<Entity, std::vector<ComponentData>>(
					OwnerID, 
					std::vector<ComponentData>()
					)
				);
		}
		EntityComponentOffsets_[OwnerID].push_back(EntityCompData);

		DataSize_	+= ComponentSize;
		NumComponents_++;
		return Comp;
	}

	template <typename T>
	T* GetComponentFromEntity(Entity EntityID)
	{		
		ComponentFamily Family = T::GetComponentFamily();
		auto& ComponentIndices = EntityComponentOffsets_[EntityID];

		for (auto& it: ComponentIndices)
		{
			if (it.Family_ == Family)
			{
				usize IntAddress = reinterpret_cast<usize>(Data_) + it.Offset_;
				return reinterpret_cast<T*>(IntAddress);
			}			
		}		

		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetComponentsOfType()
	{
		ComponentFamily Family = T::GetComponentFamily();
		std::vector<T*> Components;
		for (auto& Ent: EntityComponentOffsets_)
		{
			for (auto& it: Ent.second)
			{
				if (it.Family_ == Family)
				{
					usize IntAddress = reinterpret_cast<usize>(Data_) + it.Offset_;
					T* Component = reinterpret_cast<T*>(IntAddress);
					Components.push_back(Component);
				}
			}			
		}

		return Components;
	}

	usize Size()
	{
		return NumComponents_;
	}
};

}
