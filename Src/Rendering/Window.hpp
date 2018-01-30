#pragma once
#include "../Globals.h"

#include "SDL.h"

namespace Rendering {

class Window
{
public:
	~Window();

	bool Create(u32 X, u32 Y, u32 W, u32 H, u32 Flags);

	void SetPixel(u32 X, u32 Y, u32 Color);

	void SetRect(u32 X1, u32 Y1, u32 X2, u32 Y2, u32 Color);

	void ClearScreen();

	SDL_Surface*    GetScreenSurface() const;

	SDL_Window*     GetWindowHandle() const;
private:
	SDL_Window*		Window_ = nullptr;
	SDL_Surface*	ScreenSurface_ = nullptr;
};

}
