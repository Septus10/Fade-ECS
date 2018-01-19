#pragma once

#include "../Globals.h"
#include "Entity.hpp"
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
	usize			TypeHash_;
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
    std::vector<ComponentData>      SingletonComponentOffsets_;
        
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

    template <typename T>
    inline T* GetComponentFromComponentDataArray(std::vector<ComponentData> Array)
	{
		const std::type_info& type_info = typeid(T);
        usize Hash = type_info.hash_code();
        for (auto& it: Array)
        {
            if (it.TypeHash_ == Hash)
            {
                usize IntAddress = reinterpret_cast<usize>(Data_) + it.Offset_;
                return reinterpret_cast<T*>(IntAddress);
            }
        }

        return nullptr;
	}

    template <typename T>
    T* StoreComponentInMemory(const usize Size, ComponentData& CompData)
	{
        if (Size + DataSize_ > MaxSize_)
        {
            Resize(DataSize_ + Size);
        }

        usize IntAddress = reinterpret_cast<usize>(Data_) + DataSize_;
        Pointer Address = reinterpret_cast<void*>(IntAddress);
        T* Comp =  static_cast<T*>(new(Address) T());

        CompData.TypeHash_ = typeid(T).hash_code();
        CompData.Offset_ = DataSize_;

        DataSize_	    += Size;
        NumComponents_  ++;

        return Comp;
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

        if (T* Test = GetSingletonComponent<T>())
        {
            return Test;
        }

		usize ComponentSize = sizeof(T);
        ComponentData EntityCompData;
		T* Comp = StoreComponentInMemory<T>(ComponentSize, EntityCompData);
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
		return Comp;
	}

    template <typename T>
    T* StoreNewSingletonComponent(const Entity EntityID)
    {
        if (T* Test = GetSingletonComponent<T>())
        {
            return Test;
        }

        usize ComponentSize = sizeof(T);
        ComponentData CompData;
        T* Comp = StoreComponentInMemory<T>(ComponentSize, CompData);
        SingletonComponentOffsets_.push_back(CompData);
        return Comp;
    }

	template <typename T>
	T* GetComponentFromEntity(Entity EntityID)
	{		
		auto& ComponentIndices = EntityComponentOffsets_[EntityID];
		T* Comp = GetComponentFromComponentDataArray<T>(ComponentIndices);
        if (!Comp)
        {
            Comp = GetSingletonComponent<T>();
        }
        return Comp;
	}

    template <typename T>
    T* GetSingletonComponent()
	{
	    return GetComponentFromComponentDataArray<T>(SingletonComponentOffsets_);
	}

	template <typename T>
	std::vector<T*> GetComponentsOfType()
	{
		const std::type_info& type_info = typeid(T);
		usize Hash = type_info.hash_code();
		std::vector<T*> Components;
		for (auto& Ent: EntityComponentOffsets_)
		{
			for (auto& it: Ent.second)
			{
				if (it.TypeHash_ == Hash)
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
