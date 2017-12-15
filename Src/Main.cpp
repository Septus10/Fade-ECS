#include <iostream>

#include "Globals.h"

#include "ECS/EntityManager.hpp"
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
    eMgr.AddComponent<Transform>(ent);
	eMgr.AddComponent<Input>(ent);
	Velocity* vel = eMgr.AddComponent<Velocity>(ent);
	if (!vel)
	{
		return -1;
	}
	vel->VelX = 10.f;
	vel->VelY = 10.f;

    std::unique_ptr<ECS::SystemManager> sMgr = std::make_unique<ECS::SystemManager>(&eMgr);
    sMgr->RegisterSystem<Gameplay::MovementSystem>();
	sMgr->RegisterSystem<Gameplay::InputSystem>();
	Rendering::RenderSystem* rSystem = sMgr->RegisterSystem<Rendering::RenderSystem>();
	rSystem->SetWindowContext(&wnd);

    Timer timer, fixedTimer;
    timer.start();
    fixedTimer.start();
    const float FixedFpsSeconds = 1.f / FIXED_FPS;
	
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
			else if (e.type == SDL_KEYDOWN)
			{
				ECS::InputEvent ie;
				ie.Key = e.key.keysym.scancode;
				ie.Action = e.key.repeat ? ECS::IA_Repeat : ECS::IA_Pressed;
				sMgr->SendEvent(&ie);
			}
			else if (e.type == SDL_KEYUP)
			{
				ECS::InputEvent ie;
				ie.Key = e.key.keysym.scancode;
				ie.Action = ECS::IA_Released;
				sMgr->SendEvent(&ie);
			}
		}
        sMgr->TickSystems(timer.elapsed());
		timer.reset();
        if (fixedTimer.elapsed() > FixedFpsSeconds)
        {
            sMgr->FixedTickSystems(fixedTimer.elapsed());
			fixedTimer.reset();
        }
		wnd.ShowScreen();
    }

	return 0;
}