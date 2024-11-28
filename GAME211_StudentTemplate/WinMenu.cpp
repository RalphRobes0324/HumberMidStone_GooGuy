#include "WinMenu.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
WinMenu::WinMenu(SDL_Window* sdlWindow_, GameManager* game_)
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

WinMenu::~WinMenu() {
	delete exitButton;
}

bool WinMenu::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);


	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	backgroundTexture = IMG_LoadTexture(renderer, "YouWin.png");
	if (!backgroundTexture) {
		std::cerr << "Failed to load background image: " << IMG_GetError() << "\n";
		return false;
	}

	youWinTexture = IMG_LoadTexture(renderer, "you_win.png");
	if (!youWinTexture) {
		std::cerr << "Failed to load game over image: " << IMG_GetError() << "\n";
		return false;
	}

	youWinRect = { w / 2 - 500 / 2, 50, 500, 200 };

	game->GetSceneManager().SetCurrentScene(DefineScenes::DEATH_MENU);

	std::cout << "This is Death Menu\n";

	return true;
}

void WinMenu::OnDestroy() {}

void WinMenu::Update(const float deltaTime) {
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	int centerX = (windowWidth / 2) - (buttonWidth / 2);

	exitButton->SetPosition(500, 450);
}

void WinMenu::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr); // render bg
	SDL_RenderCopy(renderer, youWinTexture, nullptr, &youWinRect); // render game over

	exitButton->Render();

	SDL_RenderPresent(renderer);
}

void WinMenu::HandleEvents(const SDL_Event& event)
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
