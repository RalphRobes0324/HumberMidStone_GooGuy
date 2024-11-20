#include "OptionMenu.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
OptionMenu::OptionMenu(SDL_Window* sdlWindow_, GameManager* game_)
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
	backButton = new UI(renderer, "back.png", "back_hover.png", { 10, windowHeight - buttonHeight - 10, buttonWidth, buttonHeight });
}

OptionMenu::~OptionMenu(){
	delete backButton;
}

bool OptionMenu::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);


	game->GetSceneManager().SetCurrentScene(DefineScenes::OPTION_MENU);

	std::cout << "This is Option Menu\n";

	return true;
}

void OptionMenu::OnDestroy() {}

void OptionMenu::Update(const float deltaTime) {
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	int centerX = (windowWidth / 2) - (buttonWidth / 2);

	backButton->SetPosition(10, windowHeight - buttonHeight - 10);
}

void OptionMenu::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	backButton->Render();

	SDL_RenderPresent(renderer);
}

void OptionMenu::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	game->SceneSwitching(event, DefineScenes::MENU);

	backButton->HandleEvent(event, [this]() {
		std::cout << "Back Button Clicked\n";
		// swap to MainMenu
		game->SwitchScene(DefineScenes::MAIN_MENU);
	});
}
