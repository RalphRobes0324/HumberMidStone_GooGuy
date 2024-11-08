#ifndef SCENEA3_H
#define SCENEA3_H

#include <MMath.h>
#include "Scene.h"
#include "Build.h"
#include "Quest.h"
#include "DefineScenes.h"

using namespace MATH;
class SceneA3 : public Scene {
private:
	float xAxis;	// scene width, in game coords, set in constructor
	float yAxis;	// scene height, in game coords, set in constructor
	SDL_Window* window;		// an SDL window with a SDL renderer
	SDL_Renderer* renderer;	// the renderer associated with SDL window
	Matrix4 projectionMatrix;	// set in OnCreate()
    Matrix4 inverseProjection;	// set in OnCreate()

	Build platform1, platform2, platform3;
	Build wall;
	Build redPlatform1, redPlatform2;
	Build bluePlatform;
	Build triggerEvent;
	Build triggerEvent2;

	Vec3 newPos;


public:
	// This constructor may be different from what you've seen before
	// Notice the second parameter, and look in GameManager.cpp
	// to see how this constructor is called.
	SceneA3(SDL_Window* sdlWindow, GameManager* game_);
	~SceneA3();
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

	bool RectsAreEqual(const SDL_FRect& rect1, const SDL_FRect& rect2);
};

#endif
