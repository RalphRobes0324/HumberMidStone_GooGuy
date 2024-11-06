#ifndef SCENEA4_H
#define SCENEA4_H

#include <MMath.h>
#include "Scene.h"
#include "Build.h"
#include "Quest.h"
#include "DefineScenes.h"

using namespace MATH;
class SceneA4 : public Scene {
private:
	float xAxis;	// scene width, in game coords, set in constructor
	float yAxis;	// scene height, in game coords, set in constructor
	SDL_Window* window;		// an SDL window with a SDL renderer
	SDL_Renderer* renderer;	// the renderer associated with SDL window
	Matrix4 projectionMatrix;	// set in OnCreate()
    Matrix4 inverseProjection;	// set in OnCreate()

	Build platform1;
	Build triggerEvent;

	Vec3 newPos;


public:
	// This constructor may be different from what you've seen before
	// Notice the second parameter, and look in GameManager.cpp
	// to see how this constructor is called.
	SceneA4(SDL_Window* sdlWindow, GameManager* game_);
	~SceneA4();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
    void HandleEvents(const SDL_Event &event);
	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
    Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getInverseMatrix() { return inverseProjection; }

	bool RectsAreEqual(const SDL_Rect& rect1, const SDL_Rect& rect2);
};

#endif
