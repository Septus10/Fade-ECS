#pragma once

#include "Entity.hpp"
#include "Component.hpp"

#include <vector>
#include <bitset>
#include <map>
#include <unordered_map>

namespace ECS {
 
class EntityManager
{
public:
    ~EntityManager()
    {
        for (auto& it : Components_)
        {
            delete it;
        }

        Components_.clear();
    }

    Entity CreateEntity()
    {
        Entity ID = generateEntityID();
        EntityMap_[ID] = 0;
        IndexMap_[ID] = std::vector<usize>();
        return ID;
    }

    template <typename T>
    void AddComponent(const Entity EntityID)
    {
        if (EntityMap_.find(EntityID) == EntityMap_.end())
        {
            // error, can't find entity with ID
            std::cout << "Error: Can't find entity with id: " << EntityID << "\n";
            return;
        }
        EntityMap_[EntityID] |= static_cast<u64>(T::GetComponentType());

        auto& indices = IndexMap_[EntityID];
        indices.push_back(Components_.size());
        Components_.push_back(new T());
    }

    std::map<Entity, std::vector<Component*>> GetEntitiesWithComponents(u64 RequiredComponents)
    {
        std::map<Entity, std::vector<Component*>> ECM;
        for (auto& it: EntityMap_)
        {
            if (RequiredComponents & it.second) // does this entity contain these components
            {
                std::vector<Component*> CV;
                auto& IndexMap = IndexMap_[it.first];
                for (auto& index: IndexMap)
                {
                    CV.push_back(Components_[index]);
                }

                ECM[it.first] = CV;
            }
        }
        return ECM;
    }

private:
    Entity generateEntityID() const
    {
        return static_cast<Entity>(EntityMap_.size());
    }

private:
    std::vector<Component*>                             Components_;
    std::unordered_map<Entity, u64>                     EntityMap_;
    std::unordered_map<Entity, std::vector<usize>>      IndexMap_;

};

}
