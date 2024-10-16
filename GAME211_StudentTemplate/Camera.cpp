#include "Camera.h"

Camera::Camera(int _screenWidth, int _screenHeight) :
	screenWidth(_screenWidth), screenHeight(_screenHeight)
{
	gameCamera = { 0, 0, screenWidth, screenHeight };
}

void Camera::Update(float _targetX, float _targetY, GameManager* _game)
{

}


