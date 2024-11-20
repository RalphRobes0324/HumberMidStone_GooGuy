#include "MainMenu.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
MainMenu::MainMenu(SDL_Window* sdlWindow_, GameManager* game_)
{
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
	buttonWidth = 200;
	buttonHeight = 50;

	// get window dimensions
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	// calculate horizontal center
	int centerX = (windowWidth / 2) - (buttonWidth / 2);

	// Initialize buttons with positions and textures
	playButton = new UI(renderer, "play.png", "play_hover.png", { centerX, 300, buttonWidth, buttonHeight });
	levelSelectButton = new UI(renderer, "levelSelect.png", "levelSelect_hover.png", { centerX, 375, buttonWidth, buttonHeight });
	optionsButton = new UI(renderer, "options.png", "options_hover.png", { centerX, 450, buttonWidth, buttonHeight });
	exitButton = new UI(renderer, "exit.png", "exit_hover.png", { centerX, 525, buttonWidth, buttonHeight });
}

MainMenu::~MainMenu(){
	delete playButton;
	delete levelSelectButton;
	delete optionsButton;
	delete exitButton;
}

bool MainMenu::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);


	game->GetSceneManager().SetCurrentScene(DefineScenes::MAIN_MENU);

	std::cout << "This is Main Menu\n";

	return true;
}

void MainMenu::OnDestroy() {}

void MainMenu::Update(const float deltaTime) {
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	int centerX = (windowWidth / 2) - (buttonWidth / 2);

	playButton->SetPosition(centerX, 300);
	levelSelectButton->SetPosition(centerX, 375);
	optionsButton->SetPosition(centerX, 450);
	exitButton->SetPosition(centerX, 525);
}

void MainMenu::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// render buttons
	playButton->Render();
	levelSelectButton->Render();
	optionsButton->Render();
	exitButton->Render();

	SDL_RenderPresent(renderer);
}

void MainMenu::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	game->SceneSwitching(event, DefineScenes::MENU);

	playButton->HandleEvent(event, [](){ 
		std::cout << "Play Button Clicked\n";
		// swap to Scene1
	});

	levelSelectButton->HandleEvent(event, [](){ 
		std::cout << "Level Select Button Clicked\n";
		// swap to LevelSelectMenu
	});

	optionsButton->HandleEvent(event, [](){ 
		std::cout << "Options Button Clicked\n";
		// swap to OptionMenu
	});

	exitButton->HandleEvent(event, [](){ 
		std::cout << "Exit Button Clicked\n";
		SDL_Quit();
		std::exit(0);
	});
}
