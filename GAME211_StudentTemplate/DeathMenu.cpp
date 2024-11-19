#include "DeathMenu.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
DeathMenu::DeathMenu(SDL_Window* sdlWindow_, GameManager* game_)
{
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
}

DeathMenu::~DeathMenu(){
}

bool DeathMenu::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);


	game->GetSceneManager().SetCurrentScene(DefineScenes::DEATH_MENU);

	std::cout << "This is Death Menu\n";

	return true;
}

void DeathMenu::OnDestroy() {}

void DeathMenu::Update(const float deltaTime) {

}

void DeathMenu::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);


	SDL_RenderPresent(renderer);
}

void DeathMenu::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	game->SceneSwitching(event, DefineScenes::MENU);
}
