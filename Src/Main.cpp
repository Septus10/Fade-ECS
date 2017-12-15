#include <iostream>

#include "Globals.h"

#include "EntityManager.hpp"
#include "SystemManager.hpp"
#include "Component.hpp"
#include "Timer.hpp"

#include <memory>

#define FIXED_FPS 60

struct Transform : public ECS::Component
{
    Transform()
    {
        PosX = PosY = PosZ = 0.f;
        RotX = RotY = RotZ = 0.f;
        SclX = SclY = SclZ = 0.f;
    }

    static ECS::ComponentType GetComponentType()
    {
        return ECS::CT_Transform;
    }

    float PosX, PosY, PosZ;
    float RotX, RotY, RotZ;
    float SclX, SclY, SclZ;
};

struct Camera : public ECS::Component
{
    static ECS::ComponentType GetComponentType()
    {
        return ECS::CT_Camera;
    }
};

class MovementSystem : public ECS::System
{
public:
    virtual void Tick(double DeltaTime, std::map<ECS::Entity, std::vector<ECS::Component*>> Components) override
    {
        MovementTimer_ += DeltaTime;
        std::cout << "Move timer: " << MovementTimer_ << "\n";
        if (MovementTimer_ >= 1.f)
        {
            std::cout << "Movement one second tick\n";
            MovementTimer_= 0; 
        }
    }

    virtual void FixedTick(double FixedDeltaTime, std::map<ECS::Entity, std::vector<ECS::Component*>> Components) override
    {
    }

    virtual u64 GetRequiredComponents() const
    {
        return ECS::CT_Transform;
    }

private:
    double MovementTimer_ = 0.f;

protected:
    virtual void ProcessEntity(ECS::Entity EntityID, std::vector<ECS::Component*> Components) override
    {

    }
};

class CameraSystem : public ECS::System
{
public:
    virtual void Tick(double DeltaTime, std::map<ECS::Entity, std::vector<ECS::Component*>> Components) override
    {
        CameraTimer_ += DeltaTime;
        if (CameraTimer_ >= 1.f)
        {
            std::cout << "Camera one second tick\n";
            CameraTimer_ = 0;
        }
    }

    virtual void FixedTick(double FixedDeltaTime, std::map<ECS::Entity, std::vector<ECS::Component*>> Components) override
    {
        
    }

    virtual u64 GetRequiredComponents() const
    {
        return ECS::CT_Camera;
    }

private:
    double CameraTimer_ = 0.f;

protected:
    virtual void ProcessEntity(ECS::Entity EntityID, std::vector<ECS::Component*> Components) override
    {

    }
};

class CameraAndMovementSystem : public ECS::System
{
public:
    virtual void Tick(double DeltaTime, std::map<ECS::Entity, std::vector<ECS::Component*>> Components) override
    {
        CamMovTimer_ += DeltaTime;
        if (CamMovTimer_ >= 1.f)
        {
            std::cout << "Camera Move one second tick\n";
            CamMovTimer_ = 0;
        }
    }

    virtual void FixedTick(double FixedDeltaTime, std::map<ECS::Entity, std::vector<ECS::Component*>> Components) override
    {

    }

    virtual u64 GetRequiredComponents() const
    {
        return ECS::CT_Camera | ECS::CT_Transform;
    }

private:
    double CamMovTimer_ = 0.f;

protected:
    virtual void ProcessEntity(ECS::Entity EntityID, std::vector<ECS::Component*> Components) override
    {
        
    }
};

int main(int argc, char** argv)
{
    g_ExecutablePath = argv[0];

    ECS::EntityManager eMgr;
    ECS::Entity ent = eMgr.CreateEntity();
    eMgr.AddComponent<Transform>(ent);
    eMgr.AddComponent<Camera>(ent);

    std::unique_ptr<ECS::SystemManager> sMgr = std::make_unique<ECS::SystemManager>(&eMgr);
    sMgr->RegisterSystem<MovementSystem>();
    sMgr->RegisterSystem<CameraSystem>();
    sMgr->RegisterSystem<CameraAndMovementSystem>();

    Timer timer, fixedTimer;
    timer.start();
    fixedTimer.start();
    const float FixedFpsSeconds = 1.f / FIXED_FPS;

    while (g_ShouldRun)
    {
        sMgr->TickSystems(timer.elapsed());
        timer.reset();
        if (fixedTimer.elapsed() > FixedFpsSeconds)
        {
            sMgr->FixedTickSystems(fixedTimer.elapsed());
            fixedTimer.reset();
        }   
    }
}