
#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
#include <SDL_opengl.h>
#include "GameManager.h"

class Camera {
public:
	Camera(int _screenWidth, int _screenHeight);
	void Update(float _targetX, float _targetY, GameManager* _game);
	void Render(SDL_Renderer* renderer, GameManager* game);
private:
	SDL_Rect gameCamera;
	GameManager* game;
	int screenWidth;
	int screenHeight;

};


#endif