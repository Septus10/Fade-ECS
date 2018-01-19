#include <iostream>

#include "Globals.h"

#include "ECS/EntityComponentManager.hpp"
#include "ECS/SystemManager.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/Components/Camera.hpp"
#include "Timer.hpp"

#include <SDL.h>
#include <memory>

#include "Gameplay/MovementSystem.hpp"
#include "Gameplay/InputSystem.hpp"
#include "Rendering/RenderSystem.hpp"
#include "Rendering/Window.hpp"
#include "ECS/Components/Input.hpp"
#include <typeindex>


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

#define FIXED_FPS 60

c8*		g_ExecutablePath;
bool	g_ShouldRun	= true;

#undef main
int main(int argc, char** argv)
{
	g_ExecutablePath = argv[0];

	Rendering::Window wnd;
	wnd.Create(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	ECS::EntityManager eMgr;
	ECS::Entity ent = eMgr.CreateEntity();
	Transform* trans = eMgr.AddComponent<Transform>(ent);
	trans->PosX = 100;
	trans->PosY = 100;
	eMgr.AddSingletonComponent<Input>(ent);
	Velocity* vel = eMgr.AddComponent<Velocity>(ent);
	if (!vel)
	{
		return -1;
	}
	vel->VelX = 10.f;
	vel->VelY = 10.f;

	ECS::SystemManager systemMgr(&eMgr);
	systemMgr.RegisterSystem<Gameplay::MovementSystem>();
	systemMgr.RegisterSystem<Gameplay::InputSystem>();
	auto renderSystem = systemMgr.RegisterSystem<Rendering::RenderSystem>();
	renderSystem->SetWindowContext(&wnd);

	Timer timer, fixedTimer;
	timer.start();

    fixedTimer.start();
    const float FixedFpsSeconds = 1.f / FIXED_FPS;
	
	double time = 0.f;
    while (g_ShouldRun)
    {	
		wnd.ClearScreen();
		SDL_Event e{ 0 };
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				g_ShouldRun = false;
				break;
			}
			
			if (e.type == SDL_KEYDOWN)
			{
				ECS::InputEvent ie;
				ie.Key = e.key.keysym.scancode;
				ie.Action = e.key.repeat ? ECS::IA_Repeat : ECS::IA_Pressed;
				systemMgr.SendEvent(ie);
			}
			else if (e.type == SDL_KEYUP)
			{
				ECS::InputEvent ie;
				ie.Key = e.key.keysym.scancode;
				ie.Action = ECS::IA_Released;
				systemMgr.SendEvent(ie);
			}
		}

        systemMgr.PreTickSystems();
        systemMgr.TickSystems(timer.elapsed());
		timer.reset();
        if (fixedTimer.elapsed() > FixedFpsSeconds)
        {
            systemMgr.FixedTickSystems(fixedTimer.elapsed());
			fixedTimer.reset();
        }
    }

	return 0;
}