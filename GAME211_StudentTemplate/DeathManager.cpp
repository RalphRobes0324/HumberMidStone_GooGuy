#include "DeathManager.h"

DeathManager::DeathManager(float _x, float _y, float _w, float _h, Vec4 _colour)
{
	x = _x;
	y = _y;
	width = _w;
	height = _h;
	colour = _colour;
	rect = { x , y, width, height };
}

DeathManager::DeathManager()
{
}

DeathManager::~DeathManager()
{
}

void DeathManager::OnTriggerEnter(GameManager* game)
{
	Vec3 pos = game->getPlayer()->getPos();
	float radius = game->getPlayer()->getRadius();
	if ((pos.x - radius) > (rect.x + rect.w) || ((pos.x + radius) < rect.x) // x positions
		||
		((pos.y + radius) < (rect.y - rect.h)) || ((pos.y - radius) > rect.y))
	{
		return;
	}
	Die(game);
}

void DeathManager::Die(GameManager* game)
{
	game->GetSceneManager().SetCurrentScene(DefineScenes::DEATH_MENU);
	game->GetSceneManager().SetLastScene(DefineScenes::NONE);
	SDL_Event event;
	SDL_memset(&event, 0, sizeof(event));
	event.type = game->GetChangeScene();
	event.user.code = 1;
	event.user.data1 = nullptr;
	event.user.data2 = nullptr;
	SDL_PushEvent(&event);
}

void DeathManager::Render(SDL_Renderer* renderer, GameManager* game)
{
	SDL_Rect sdlPlatform;
	sdlPlatform.x = (1000 * rect.x) / game->getSceneWidth(); //converting physical coordinates to sdl coordinates on x-axis
	sdlPlatform.y = (600 * (game->getSceneHeight() - rect.y)) / game->getSceneHeight();//converting physical coordinates to sdl coordinates on y-axis
	sdlPlatform.w = (1000 * rect.w) / game->getSceneWidth();
	sdlPlatform.h = (600 * rect.h) / game->getSceneHeight();

	SDL_SetRenderDrawColor(renderer, colour.x, colour.y, colour.z, colour.w);
	SDL_RenderFillRect(renderer, &sdlPlatform);
}
