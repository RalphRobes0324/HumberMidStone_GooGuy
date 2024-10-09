#ifndef BUILD_H
#define BUILD_H

#include <SDL.h>
#include "GameManager.h"
#include "Body.h"


class Build {
public:
	Build(int _x, int _y, int _w, int _h, 
		bool _canMove, 
		bool _canDisappear, bool _isVisable, float _disappearTime,
		Vec4 _colour);

	void Render(SDL_Renderer* renderer, GameManager* game);

	void Update();
	void Update(float DeltaTime);

	SDL_Rect getPlatform() { return rect; } //added getter to get the platform so it can be used for checks of collision (Maya)

	SDL_Rect getPlatformInSDLspace(GameManager* game);
	
private:
	int x, y, width, height;
	SDL_Rect rect;

	bool canMove;
	bool canDisappear;
	bool isVisable;

	Vec4 colour;

	float disappearTime;
	float timer;
};

#endif

