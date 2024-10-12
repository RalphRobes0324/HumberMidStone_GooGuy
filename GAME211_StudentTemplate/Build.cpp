#include "Build.h"

/// <summary>
/// Define the default build
/// </summary>
/// <param name="_x"></param>
/// <param name="_y"></param>
/// <param name="_w"></param>
/// <param name="_h"></param>
/// <param name="_colour"></param>
Build::Build(int _x, int _y, int _w, int _h, Vec4 _colour)
{
	x = _x;
	y = _y;
	width = _w;
	height = _h;
	colour = _colour;

	rect = { x, y, width, height };
}

/// <summary>
/// Init disappearing platform
/// </summary>
/// <param name="_x"></param>
/// <param name="_y"></param>
/// <param name="_w"></param>
/// <param name="_h"></param>
/// <param name="_canDisappear"></param>
/// <param name="_isVisible"></param>
/// <param name="_disappearTime"></param>
/// <param name="_colour"></param>
Build::Build(int _x, int _y, int _w, int _h, bool _canDisappear, bool _isVisible, float _disappearTime, Vec4 _colour)
{
	x = _x;
	y = _y;
	width = _w;
	height = _h;

	canDisappear = _canDisappear;
	isVisible = _isVisible;
	disappearTime = _disappearTime;
	warningTime = _disappearTime / 2;

	colour = _colour;
	alpha = colour.w;
	rect = { x, y, width, height };
}

Build::Build(int _x, int _y, int _w, int _h, 
	bool _canMove, bool _canLoop, bool _isMoving, 
	bool _moveForward, bool _moveUpward, 
	float _speed, float _waitTime, 
	Vec3 _endPoint, 
	Vec4 _colour)
{
	x = _x;
	y = _y;
	width = _w;
	height = _h;

	canMove = _canMove;
	canLoop = _canLoop;

	isMoving = _canLoop;
	moveForward = _moveForward;
	moveUpward = _moveUpward;

	waitTime = _waitTime;
	speed = _speed;
	startPoint = Vec3(x, y, 0.0f);
	endPoint = _endPoint;

	colour = _colour;
	rect = { x, y, width, height };
}

/// <summary>
/// Update the changes to the build
/// </summary>
void Build::Update() {
	rect = { x , y, width, height };
}

void Build::Update(float DeltaTime)
{
	//check build can disappear
	if (canDisappear) {
		timer += DeltaTime;

		if (isVisible && timer >= (disappearTime - warningTime)) {
			isWarning = true;
		}

		//Handle warning system on the platforms if warning begins
		if (isWarning) {
			float progress = (disappearTime - timer) / warningTime;
			alpha = static_cast<int>(255 * progress); // Reduce alpha to fade out
			if (alpha < alphaEnds) alpha = alphaEnds; // Clamp to avoid negative alpha
		}
		
		//Time is up, make platform disappear
		if (isVisible && timer >= disappearTime) {
			isVisible = false;// Platform disappear
			isWarning = false; //reset warning
			timer = 0.0f; // Reset the timer
			alpha = colour.w; //reset alpha

		}
		//Time is up, make platform reappear
		else if (!isVisible && timer >= disappearTime) {
			isVisible = true;// build  reappear
			timer = 0.0f; // Reset the timer
		}
	}

	else if (canMove) {
		if (canLoop) {
			if (!isMoving) {
				timer += DeltaTime;
				if (timer >= waitTime) {
					isMoving = true;
					timer = 0.0f;
					moveForward = !moveForward; // Switch direction after stopping
				}
			}
			else {
				if (moveForward) {
					// Move towards the end point
					if (rect.x < endPoint.x) rect.x += speed * DeltaTime;

					// Check if reached the destination
					if (rect.x >= endPoint.x) {
						rect.x = endPoint.x;
						isMoving = false;
					}
				}
				else {
					//Move towards start point
					if (rect.x > startPoint.x) rect.x -= speed * DeltaTime;
					// Check if reached the starting point
					if (rect.x <= startPoint.x) {
						rect.x = startPoint.x; // Snap to the starting point
						isMoving = false;
					}

				}
			}
		}
	}

}



/// <summary>
/// Render the build
/// </summary>
/// <param name="renderer"></param>
void Build::Render(SDL_Renderer* renderer, GameManager* game) {

	// Enable alpha blending
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//Convert physics space so it can render in sdl space
	SDL_Rect sdlPlatform;
	sdlPlatform.x = (1000 * rect.x) / game->getSceneWidth(); //converting physical coordinates to sdl coordinates on x-axis
	sdlPlatform.y = (600 * (game->getSceneHeight() - rect.y)) / game->getSceneHeight();//converting physical coordinates to sdl coordinates on y-axis
	sdlPlatform.w = (1000 * rect.w) / game->getSceneWidth();
	sdlPlatform.h = (600 * rect.h) / game->getSceneHeight();

	//Check Type of build
	if (canDisappear) {
		//Check visable
		if (isVisible) {
			SDL_SetRenderDrawColor(renderer, colour.x, colour.y, colour.z, alpha);
			SDL_RenderFillRect(renderer, &sdlPlatform);
		}
		else if (!isVisible) {
			SDL_SetRenderDrawColor(renderer, colour.x, colour.y, colour.z, alphaEnds);
			SDL_RenderFillRect(renderer, &sdlPlatform);
		}
	}
	else {
		SDL_SetRenderDrawColor(renderer, colour.x, colour.y, colour.z, colour.w);
		SDL_RenderFillRect(renderer, &sdlPlatform);
	}
}

