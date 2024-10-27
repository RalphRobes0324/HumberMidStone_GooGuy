
#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
#include "Scene.h"
#include <MMath.h>
#include "GameManager.h"

class Camera {
public:
	Camera();
	void Update(const float deltaTime, SDL_Window* _window, float _xAxis, float _yAxis, GameManager* _game);
	void OnCreate();
	Matrix4 GetProjectionMatrix();

private:
	SDL_Window* window;
	Matrix4 projectionMatrix;
	float xAxis, yAxis;
	float left; 
	float right, bottom, top;
	int w, h;

	Matrix4 ndc;
	Matrix4 orth;
	GameManager* game;



	
	
};


#endif