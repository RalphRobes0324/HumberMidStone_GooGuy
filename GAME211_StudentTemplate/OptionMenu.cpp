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
	buttonWidth = 150;
	buttonHeight = 150;

	// get window dimensions
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	// calculate horizontal center
	int centerX = (windowWidth / 2) - (buttonWidth / 2);

	// get muted state from game manager
	isMuted = game->GetMute();

	// Initialize buttons with positions and textures
	backButton = new UI(renderer, "back.png", "back_hover.png", { 10, windowHeight - buttonHeight - 10, 200, 75 });
	volumeButton = 
		new UI(renderer, isMuted ? "volOFF.png" : "volON.png", 
			isMuted ? "volOFF_hover.png" : "volON_hover.png",
			{ centerX, 300, buttonWidth, buttonHeight });
}

OptionMenu::~OptionMenu(){
	delete backButton;
	delete volumeButton;
}

bool OptionMenu::OnCreate() {
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

	optionsTexture = IMG_LoadTexture(renderer, "options.png");
	if (!optionsTexture) {
		std::cerr << "Failed to load options image: " << IMG_GetError() << "\n";
		return false;
	}

	optionsRect = { w / 2 - 300 / 2, 50, 300, 100 };

	game->GetSceneManager().SetCurrentScene(DefineScenes::OPTION_MENU);

	std::cout << "This is Option Menu\n";

	return true;
}

void OptionMenu::OnDestroy() {}

void OptionMenu::Update(const float deltaTime) {
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	int centerX = (windowWidth / 2) - (buttonWidth / 2);

	backButton->SetPosition(10, windowHeight - 85);
	volumeButton->SetPosition(centerX, 300);
}

void OptionMenu::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr); // render bg
	SDL_RenderCopy(renderer, optionsTexture, nullptr, &optionsRect); // render options

	backButton->Render();
	volumeButton->Render();

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
	volumeButton->HandleEvent(event, [this]() {
		std::cout << "Volume Button Clicked\n";
		// update button textures baed on mute state
		if (isMuted)
		{
			volumeButton->UpdateTextures("volON.png", "volON_hover.png");
		}
		else {
			volumeButton->UpdateTextures("volOFF.png", "volOFF_hover.png");
		}
		// toggle mute/unmute
		isMuted = !isMuted;
		game->SetMute(isMuted);
	});
}
