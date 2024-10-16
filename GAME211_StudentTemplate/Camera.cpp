#include "Camera.h"

Camera::Camera(int _screenWidth, int _screenHeight) :
	screenWidth(_screenWidth), screenHeight(_screenHeight)
{
	gameCamera = { 0, 0, screenWidth, screenHeight };
}

void Camera::Update(float _targetX, float _targetY, GameManager* _game)
{
	//Physics space to sdl space
	int x  = (1000 * _targetX ) / _game->getSceneWidth(); //converting physical coordinates to sdl coordinates on x-axis
	int y = (600 * (_game->getSceneHeight() - _targetY)) / _game->getSceneHeight();//converting physical coordinates to sdl coordinates on y-axis

	//Center the camera
	gameCamera.x = x - screenWidth / 2;
	gameCamera.y = y - screenHeight / 2;
}

void Camera::Render(SDL_Renderer* renderer, GameManager* game)
{

}


