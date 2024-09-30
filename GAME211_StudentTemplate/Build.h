#ifndef BUILD_H
#define BUILD_H

#include <SDL.h>
#include "GameManager.h"
#include "Body.h"


class Build {
public:
	Build(int _x, int _y, int _w, int _h);

	void Render(SDL_Renderer* renderer);

	void Update();

	SDL_Rect getPlatform() { return rect; } //added getter to get the platform so it can be used for checks of collision (Maya)

	SDL_Rect getPlatformInPhysicsSpace(GameManager* game);
	
private:
	int x, y, width, height;
	SDL_Rect rect;

	bool isTrigger;
};

#endif

