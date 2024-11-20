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

	buttonWidth = 200;
	buttonHeight = 75;

	// get window dimensions
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	// calculate horizontal center
	int centerX = (windowWidth / 2) - (buttonWidth / 2);

	// Initialize buttons with positions and textures
	exitButton = new UI(renderer, "exit.png", "exit_hover.png", { centerX, 400, buttonWidth, buttonHeight });
}

DeathMenu::~DeathMenu(){
	delete exitButton;
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
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	int centerX = (windowWidth / 2) - (buttonWidth / 2);

	exitButton->SetPosition(centerX, 400);
}

void DeathMenu::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	exitButton->Render();

	SDL_RenderPresent(renderer);
}

void DeathMenu::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	game->SceneSwitching(event, DefineScenes::MENU);

	exitButton->HandleEvent(event, [this]() {
		std::cout << "Exit Button Clicked\n";
		// swap to MainMenu
		game->SwitchScene(DefineScenes::MAIN_MENU);
		});
}
