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

	// big blob of components, saved in order of entity
	Pointer							Data_;
	usize							DataSize_;
	
	std::vector<usize>				EntityStartOffsets_;
	std::unordered_map
		<usize, std::vector<usize>> ComponentOffsets_;
	usize							LastOffset_;
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
		usize Hash = typeid(T).hash_code();
		for (auto& it : Array)
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
	T* StoreComponentAtOffset(const usize Size, const usize Offset)
	{
		if (Size + DataSize_ > MaxSize_)
		{
			Resize(DataSize_ + Size);
		}

		// if we're behind the final component
		if (Offset >= DataSize_)
		{
			// we've already resized the array if necessary, so we just grab the offset
			// no need to move around anything
			DataSize_ += Size;
			return new(Offset) T(); 
		}

		// if we get here it means that we're putting the component somewhere between other components
		// this is where it gets tricky, we'll have to move part of the data and update all offsets accordingly
			// Move the memory around
		void* StartAddress = reinterpret_cast<void*>(Offset + DataSize_);
		void* MoveAddress = reinterpret_cast<void*>(Offset + DataSize_ + Size);
		usize MoveSize = DataSize_ - Offset;
		memmove(MoveAddress, StartAddress, MoveSize);
			// Update all appropriate offsets
		for (auto& offset: EntityStartOffsets_)
		{
			if (offset >= Offset)
			{
				offset += Size;
			}
		}
		DataSize_ += Size;
		return new(Offset) T();
	}

	usize GetNextFreeOffsetOfEntity(Entity ID)
	{
		auto Size = EntityStartOffsets_.size();
		if (ID >= EntityStartOffsets_.size())
		{
			throw std::exception("Index out of range");
		}

		if (ID == Size - 1)
		{
			return DataSize_;
		}

		return EntityStartOffsets_[ID];
	}

public:
	ComponentArray()
	{
		Data_			= nullptr;
		MaxSize_		= 0;
		DataSize_		= 0;
		LastOffset_		= 0;
		NumComponents_	= 0;
	}

	ComponentArray(usize ReservedSize) :
		ComponentArray()
	{
		Resize(ReservedSize);
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

		usize ComponentSize		= sizeof(T);
		usize RequiredOffset	= GetNextFreeOffsetOfEntity(OwnerID);

		T* Comp = StoreComponentAtOffset<T>(ComponentSize, RequiredOffset);

		usize ParentOffset		= EntityStartOffsets_[OwnerID] - RequiredOffset;
		usize Hash				= typeid(T).hash_code();

		ComponentOffsets_[Hash][OwnerID] = ParentOffset;

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
	std::vector<T*> GetComponentsOfType()
	{
		const std::type_info& type_info = typeid(T);
		usize Hash = type_info.hash_code();
		std::vector<T*> Components;
		for (auto& Ent : EntityComponentOffsets_)
		{
			for (auto& it : Ent.second)
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
