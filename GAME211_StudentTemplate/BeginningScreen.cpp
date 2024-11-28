#include "BeginningScreen.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
BeginningScreen::BeginningScreen(SDL_Window* sdlWindow_, GameManager* game_)
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
	playButton = new UI(renderer, "play.png", "play_hover.png", { centerX, 400, buttonWidth, buttonHeight });
}

BeginningScreen::~BeginningScreen() {
	delete playButton;
}

bool BeginningScreen::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);


	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	backgroundTexture = IMG_LoadTexture(renderer, "OpeningImage.png");
	if (!backgroundTexture) {
		std::cerr << "Failed to load background image: " << IMG_GetError() << "\n";
		return false;
	}

	game->GetSceneManager().SetCurrentScene(DefineScenes::BEGINNING_SCREEN);

	std::cout << "This is Death Menu\n";

	return true;
}

void BeginningScreen::OnDestroy() {}

void BeginningScreen::Update(const float deltaTime) {
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	int centerX = (windowWidth / 2) - (buttonWidth / 2);

	playButton->SetPosition(650, 450);
}

void BeginningScreen::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr); // render bg

	playButton->Render();

	SDL_RenderPresent(renderer);
}

void BeginningScreen::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	game->SceneSwitching(event, DefineScenes::MENU);

	playButton->HandleEvent(event, [this]() {
		std::cout << "Play Button Clicked\n";
		// swap to Scene1
		game->SwitchScene(DefineScenes::A1);
		});
}
