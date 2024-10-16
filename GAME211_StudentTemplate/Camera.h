
#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
#include "GameManager.h"

class Camera {
public:
	Camera(int _screenWidth, int _screenHeight);
	void Update(float _targetX, float _targetY, GameManager* _game);

private:
	SDL_Rect gameCamera;
	int screenWidth;
	int screenHeight;
};


#endif