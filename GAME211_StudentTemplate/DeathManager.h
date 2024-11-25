#pragma once

#include <SDL.h>
#include "GameManager.h"
#include "Body.h"
#include "DefineScenes.h"
#include "TemperatureManager.h"

class DeathManager
{
public:
	DeathManager(float _x, float _y, float _w, float _h, Vec4 _colour);
	DeathManager();
	~DeathManager();
	void Render(SDL_Renderer* renderer, GameManager* game);
	void OnTriggerEnter(GameManager* game);
	void Die(GameManager* game);
private:
	float x, y, width, height;
	Vec4 colour;
	SDL_FRect rect;
};

