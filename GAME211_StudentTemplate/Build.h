#ifndef BUILD_H
#define BUILD_H

#include <SDL.h>
#include "GameManager.h"
#include "Body.h"


class Build {
public:
	Build(int _x, int _y, int _w, int _h, 
		bool _canMove, bool _canLoop, bool _isMoving, bool _moveForward, bool _moveUpward, float _waitTime,
		bool _canDisappear, bool _isVisible, float _disappearTime,
		Vec4 _colour);

	//Setting up Default Build
	Build(int _x, int _y, int _w, int _h, Vec4 _colour);

	//Setting up Disappearing Build
	Build(int _x, int _y, int _w, int _h, bool _canDisappear, bool _isVisible, float _disappearTime, Vec4 _colour);

	Build(int _x, int _y, int _w, int _h, 
		bool _canMove, bool _canLoop, bool _isMoving, bool _moveForward, bool _moveUpward, 
		float _waitTime, Vec3 _endPoint,
		Vec4 _colour);

	void Render(SDL_Renderer* renderer, GameManager* game);

	void Update();
	void Update(float DeltaTime);

	SDL_Rect getPlatform() { return rect; } //added getter to get the platform so it can be used for checks of collision (Maya)

	bool getVisibility() { return isVisible; } //added to return visibility for collision checks (Maya)
	
private:
	int x, y, width, height;
	SDL_Rect rect;

	//Type of Build
	bool canMove, canLoop;
	bool canDisappear;

	//Settings Visiability
	bool isVisible; //Display Build
	bool isWarning; //state if platform will warn the player
	float disappearTime; //length
	float warningTime; //Time to start warning the player
	int alpha; //Alpha of build
	int alphaEnds = 50; //Where alpha ends

	//Settings Movments
	bool isMoving;
	bool moveForward;
	bool moveUpward;
	float waitTime;
	Vec3 endPoint;
	Vec3 startPoint;

	Vec4 colour; //Build's colour
	

	
	float timer;
};

#endif

