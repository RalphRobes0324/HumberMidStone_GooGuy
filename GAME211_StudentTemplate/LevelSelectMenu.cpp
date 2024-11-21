#include "LevelSelectMenu.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
LevelSelectMenu::LevelSelectMenu(SDL_Window* sdlWindow_, GameManager* game_)
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
	level1Button = new UI(renderer, "level1.png", "level1_hover.png", { centerX, 200, buttonWidth, buttonHeight });
	level2Button = new UI(renderer, "level2.png", "level2_hover.png", { centerX, 300, buttonWidth, buttonHeight });
	level3Button = new UI(renderer, "level3.png", "level3_hover.png", { centerX, 400, buttonWidth, buttonHeight });
	level4Button = new UI(renderer, "level4.png", "level4_hover.png", { centerX, 500, buttonWidth, buttonHeight });
	backButton = new UI(renderer, "back.png", "back_hover.png", { 10, windowHeight - buttonHeight - 10, buttonWidth, buttonHeight });
}

LevelSelectMenu::~LevelSelectMenu(){
	delete level1Button;
	delete level2Button;
	delete level3Button;
	delete level4Button;
	delete backButton;
}

bool LevelSelectMenu::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	backgroundTexture = IMG_LoadTexture(renderer, "Titlebg.png");
	if (!backgroundTexture) {
		std::cerr << "Failed to load background image: " << IMG_GetError() << "\n";
		return false;
	}

	menuTexture = IMG_LoadTexture(renderer, "levelSelect.png");
	if (!menuTexture) {
		std::cerr << "Failed to load menu image: " << IMG_GetError() << "\n";
		return false;
	}

	menuRect = { w / 2 - 300 / 2, 50, 300, 100 };

	game->GetSceneManager().SetCurrentScene(DefineScenes::LEVEL_SELECT_MENU);

	std::cout << "This is Level Select Menu\n";

	return true;
}

void LevelSelectMenu::OnDestroy() {}

void LevelSelectMenu::Update(const float deltaTime) {
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	int centerX = (windowWidth / 2) - (buttonWidth / 2);

	level1Button->SetPosition(centerX, 200);
	level2Button->SetPosition(centerX, 300);
	level3Button->SetPosition(centerX, 400);
	level4Button->SetPosition(centerX, 500);
	backButton->SetPosition(10, windowHeight - buttonHeight - 10);
}

void LevelSelectMenu::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr); // render bg
	SDL_RenderCopy(renderer, menuTexture, nullptr, &menuRect); // render menu

	// render buttons
	level1Button->Render();
	level2Button->Render();
	level3Button->Render();
	level4Button->Render();
	backButton->Render();

	SDL_RenderPresent(renderer);
}

void LevelSelectMenu::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	game->SceneSwitching(event, DefineScenes::MENU);

	level1Button->HandleEvent(event, [this]() {
		std::cout << "Level 1 Button Clicked\n";
		// swap to Leve1
		game->SwitchScene(DefineScenes::A1);
	});

	level2Button->HandleEvent(event, [this]() {
		std::cout << "Level 2 Button Clicked\n";
		// swap to Level2
		game->SwitchScene(DefineScenes::B1);
	});

	level3Button->HandleEvent(event, [this]() {
		std::cout << "Level 3 Button Clicked\n";
		// swap to Level3
		game->SwitchScene(DefineScenes::C1);
	});

	level4Button->HandleEvent(event, [this]() {
		std::cout << "Level 4 Button Clicked\n";
		// swap to Level4
		game->SwitchScene(DefineScenes::D1);
	});

	backButton->HandleEvent(event, [this]() {
		std::cout << "Back Button Clicked\n";
		// swap to MainMenu
		game->SwitchScene(DefineScenes::MAIN_MENU);
	});
}
