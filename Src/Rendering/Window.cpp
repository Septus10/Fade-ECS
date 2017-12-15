#include "Window.hpp"

#include <iostream>

namespace Rendering {

Window::~Window()
{
	SDL_DestroyWindow(Window_);
	SDL_DestroyRenderer(Renderer_);
	SDL_Quit();
}

bool Window::Create(u32 X, u32 Y, u32 W, u32 H, u32 Flags)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\nPress enter to continue...\n";
		std::cin.get();
		return false;
	}

	Window_ = SDL_CreateWindow("Fade ECS", X, Y, W, H, Flags);
	if (!Window_)
	{
		std::cout << "SDL window could not be created! SDL_Error: " << SDL_GetError() << "\nPress enter to continue...\n";
		std::cin.get();
		SDL_Quit();
		return false;
	}


	Renderer_ = SDL_CreateRenderer(Window_, -1, SDL_RENDERER_ACCELERATED);
	if (!Renderer_)
	{
		SDL_DestroyWindow(Window_);
		std::cout << "SDL renderer could not be created! SDL_Error: " << SDL_GetError() << "\nPress enter to continue...\n";
		SDL_Quit();
		return false;
	}
	ScreenSurface_ = SDL_GetWindowSurface(Window_);
	SDL_FillRect(ScreenSurface_, nullptr, SDL_MapRGB(ScreenSurface_->format, 0x1F, 0x1F, 0x1F));
	SDL_UpdateWindowSurface(Window_);

	return true;
}

void Window::SetPixel(u32 X, u32 Y, u32 Color)
{
	u32* Pixels = reinterpret_cast<u32*>(ScreenSurface_->pixels);
	Pixels[sizeof(u32) * (Y * ScreenSurface_->w + X)] = Color;
}

void Window::SetRect(u32 X1, u32 Y1, u32 X2, u32 Y2, u32 Color)
{
	u32* Pixels = reinterpret_cast<u32*>(ScreenSurface_->pixels);
	for (auto i = Y1; i < Y2; i++)
	{
		for (auto j = X1; j < X2; j++)
		{
			SetPixel(i, j, Color);
		}
	}
}

void Window::ClearScreen()
{
	SDL_FillRect(ScreenSurface_, nullptr, 0x1F1F1F);
}

void Window::ShowScreen()
{
	SDL_RenderClear(Renderer_);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Renderer_, ScreenSurface_);
	SDL_RenderCopy(Renderer_, tex, nullptr, nullptr);
	SDL_DestroyTexture(tex);
	SDL_RenderPresent(Renderer_);
}


}
