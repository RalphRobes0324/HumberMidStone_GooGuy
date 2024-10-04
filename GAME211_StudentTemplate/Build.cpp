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
/// Converts Screen space to physics space
/// </summary>
/// <param name="game"></param>
/// <returns></returns>
SDL_Rect Build::getPlatformInSDLspace(GameManager* game)
{
	//found windows w and h 1000x600

	SDL_Rect physicsRect; 

	float screenHeight = game->getSceneHeight();
	float screenWidth = game->getSceneWidth();

	physicsRect.x = x * screenWidth / 1000;
	physicsRect.y = (600 - y) * screenHeight / 600;
	physicsRect.w = width * screenWidth / 1000;
	physicsRect.h = (600 - height) * screenHeight / 600;

	return physicsRect;
}

/// <summary>
/// Render the build
/// </summary>
/// <param name="renderer"></param>
void Build::Render(SDL_Renderer* renderer, GameManager* game) {

	//Convert sdl space so it can render in sdl space
	SDL_Rect sdlPlatform;
	sdlPlatform.x = (1000 * rect.x) / game->getSceneWidth(); //converting physical coordinates to sdl coordinates on x-axis
	sdlPlatform.y = (600 * (game->getSceneHeight() - rect.y)) / game->getSceneHeight();//converting physical coordinates to sdl coordinates on y-axis
	sdlPlatform.w = (1000 * rect.w) / game->getSceneWidth();
	sdlPlatform.h = (600 * rect.h) / game->getSceneHeight();

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderFillRect(renderer, &sdlPlatform);
}

