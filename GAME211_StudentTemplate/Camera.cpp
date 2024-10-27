#include "Camera.h"



Camera::Camera()
{

}

void Camera::OnCreate()
{

}

void Camera::Update(const float deltaTime, SDL_Window* _window, float _xAxis, float _yAxis, GameManager* _game)
{
	window = _window;
	xAxis = _xAxis;
	yAxis = _yAxis;
	game = _game;

	SDL_GetWindowSize(window, &w, &h);
	ndc = MMath::viewportNDC(w, h);

	left = 0.0f;
	right = xAxis;
	bottom = 0.0f; 
	top = yAxis;

	left = game->getPlayer()->getPos().x - xAxis / 2.0f;
	right = game->getPlayer()->getPos().x + xAxis / 2.0f;
	bottom = game->getPlayer()->getPos().y - yAxis / 2.0f;
	top = game->getPlayer()->getPos().y + yAxis / 2.0f;

	orth = MMath::orthographic(left, right, bottom, top, 0.0f, 1.0f);
	projectionMatrix = ndc * orth;
}

Matrix4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}


