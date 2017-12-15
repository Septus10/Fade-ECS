#pragma once

#include <vector>
#include <memory>

#include "EntityManager.hpp"

namespace ECS {
    
class System
{
public:
    virtual ~System() = default;

    virtual void Tick(double DeltaTime, std::map<Entity, std::vector<Component*>> Components) = 0;

    virtual void FixedTick(double FixedDeltaTime, std::map<Entity, std::vector<Component*>> Components) = 0;

    virtual u64 GetRequiredComponents() const = 0;

protected:
    virtual void ProcessEntity(Entity EntityID, std::vector<Component*> Components) = 0;
};

class SystemManager
{
public:
    SystemManager(EntityManager* eMgr) :
        EntityManager_(eMgr)
    {  }

    template <typename T>
    inline void RegisterSystem()
    {
        Systems_.push_back(move(std::make_unique<T>()));
    }

    void TickSystems(double DeltaTime)
    {
        for (auto i = 0; i < Systems_.size(); i++)
        {
            auto ECM = EntityManager_->GetEntitiesWithComponents(Systems_[i]->GetRequiredComponents());
            Systems_[i]->Tick(DeltaTime, ECM);
        }
    }

    void FixedTickSystems(double FixedDeltaTime)
    {
        for (auto i = 0; i < Systems_.size(); i++)
        {
            auto ECM = EntityManager_->GetEntitiesWithComponents(Systems_[i]->GetRequiredComponents());
            Systems_[i]->FixedTick(FixedDeltaTime, ECM);
        }
    }

private:
    std::vector<std::unique_ptr<System>> Systems_;
    EntityManager* EntityManager_;

};

}
