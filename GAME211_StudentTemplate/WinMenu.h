#ifndef WINMENU_H
#define WINMENU_H

#include <MMath.h>
#include "Scene.h"
#include "Build.h"
#include "DefineScenes.h"
#include "UI.h"

using namespace MATH;
class WinMenu : public Scene {
	float xAxis;	// scene width, in game coords, set in constructor
	float yAxis;	// scene height, in game coords, set in constructor
	SDL_Window* window;		// an SDL window with a SDL renderer
	SDL_Renderer* renderer;	// the renderer associated with SDL window
	Matrix4 projectionMatrix;	// set in OnCreate()
	Matrix4 inverseProjection;	// set in OnCreate()

	//buttons 
	UI* exitButton;

	//button dimensions 
	int buttonWidth;
	int buttonHeight;

	SDL_Texture* backgroundTexture;
	SDL_Texture* youWinTexture;
	SDL_Rect youWinRect;

public:
	// This constructor may be different from what you've seen before
	// Notice the second parameter, and look in GameManager.cpp
	// to see how this constructor is called.
	WinMenu(SDL_Window* sdlWindow, GameManager* game_);
	~WinMenu();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& event);
	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
	Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getInverseMatrix() { return inverseProjection; }
};

#endif 