#include "Build.h"


/// <summary>
/// Define the build
/// </summary>
/// <param name="_x"></param>
/// <param name="_y"></param>
/// <param name="_w"></param>
/// <param name="_h"></param>
Build::Build(int _x, int _y, int _w, int _h, 
	bool _canMove, 
	bool _canDisappear, bool _isVisible, float _disappearTime,
	Vec4 _colour)
{
	x = _x;
	y = _y;
	width = _w;
	height = _h;

	canMove = _canMove;
	canDisappear = _canDisappear;
	isVisible = _isVisible;
	disappearTime = _disappearTime;
	warningTime = _disappearTime / 2;
	
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
			if (alpha < 0) alpha = 0; // Clamp to avoid negative alpha
		}
		
		//Time is up, make platform disappear
		if (isVisible && timer >= disappearTime) {
			isVisible = false;// Platform disappear
			isWarning = false; //reset warning
			timer = 0.0f; // Reset the timer
			alpha = 255; //reset alpha

		}
		//Time is up, make platform reappear
		else if (!isVisible && timer >= disappearTime) {
			isVisible = true;// build  reappear
			timer = 0.0f; // Reset the timer
		}
	}
}


/// <summary>
/// Converts Screen space to physics space
/// </summary>
/// <param name="game"></param>
/// <returns></returns>
SDL_Rect Build::getPlatformInSDLspace(GameManager* game)
{
	//found windows w and h 1000x600

	SDL_Rect physicsRect; 

	float screenHeight = game->getSceneHeight();
	float screenWidth = game->getSceneWidth();

	physicsRect.x = x * screenWidth / 1000;
	physicsRect.y = (600 - y) * screenHeight / 600;
	physicsRect.w = width * screenWidth / 1000;
	physicsRect.h = (600 - height) * screenHeight / 600;

	return physicsRect;
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
	}
	else {
		SDL_SetRenderDrawColor(renderer, colour.x, colour.y, colour.z, colour.w);
		SDL_RenderFillRect(renderer, &sdlPlatform);
	}
}

