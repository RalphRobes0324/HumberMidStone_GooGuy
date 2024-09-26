#ifndef BUILD_H
#define BUILD_H

#include <SDL.h>


class Build {
public:
	Build(int _x, int _y, int _w, int _h);

	void Render(SDL_Renderer* renderer);

	void Update();

	SDL_Rect getPlatform() { return SDL_Rect(rect); }

private:
	int x, y, width, height;
	SDL_Rect rect;
};

#endif

