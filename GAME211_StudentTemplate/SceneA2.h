#ifndef SCENEA2_H
#define SCENEA2_H

#include <MMath.h>
#include "Scene.h"
#include "Build.h"
#include "Quest.h"
#include "DefineScenes.h"
#include "JumpText.h"
#include "MovementText.h"

using namespace MATH;
class SceneA2 : public Scene {
private:
	float xAxis;	// scene width, in game coords, set in constructor
	float yAxis;	// scene height, in game coords, set in constructor
	SDL_Window* window;		// an SDL window with a SDL renderer
	SDL_Renderer* renderer;	// the renderer associated with SDL window
	Matrix4 projectionMatrix;	// set in OnCreate()
    Matrix4 inverseProjection;	// set in OnCreate()

	Build platform1;
	Build platform2;
	Build platform3;
	Build platform4;
	Build wall;
	Build redPlatform1;
	Build redPlatform2;
	Build redPlatform3;
	Build bluePlatform1;
	Build bluePlatform2;
	Build triggerEvent;
	Build triggerEvent2;
	Build Background;

	Vec3 newPos;

	Quest quest;
	JumpText jumpText;
	MovementText movementText;


public:
	// This constructor may be different from what you've seen before
	// Notice the second parameter, and look in GameManager.cpp
	// to see how this constructor is called.
	SceneA2(SDL_Window* sdlWindow, GameManager* game_);
	~SceneA2();
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
