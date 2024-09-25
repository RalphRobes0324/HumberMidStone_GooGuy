#include "Build.h"


/// <summary>
/// Define the build
/// </summary>
/// <param name="_x"></param>
/// <param name="_y"></param>
/// <param name="_w"></param>
/// <param name="_h"></param>
Build::Build(int _x, int _y, int _w, int _h)
{
	x = _x;
	y = _y;
	width = _w;
	height = _h;

	rect = { x, y, width, height };
}

/// <summary>
/// Update the changes to the build
/// </summary>
void Build::Update() {
	rect = { x , y, width, height };
}

/// <summary>
/// Render the build
/// </summary>
/// <param name="renderer"></param>
void Build::Render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);
}

