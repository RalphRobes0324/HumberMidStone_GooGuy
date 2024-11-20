#include "SceneA5.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
SceneA5::SceneA5(SDL_Window* sdlWindow_, GameManager* game_) :
	Background(0.0f, 15.0f, 30.0f, 15.0f, Vec4(255, 255, 255, 255), "bookcase/book_bg.png"),
	platform1(0.0f, 2.0f, 30.0f, 2.5f, Vec4(255, 255, 255, 255), "bookcase/book_h1.png"),
	platform2(0.0f, 15.5f, 20.0f, 1.5f, Vec4(255, 255, 255, 255), "bookcase/book_h1.png"),
	platform3(8.75f, 4.5f, 8.0f, 2.65f, Vec4(255, 255, 255, 255), "bookcase/book_b5.png"),
	wall1(16.55f, 8.5f, 5.0f, 7.0f, Vec4(255, 255, 255, 255), "bookcase/book_b3.png"),
	wall2(21.5f, 11.0f, 3.0f, 9.5f, Vec4(255, 255, 255, 255), "bookcase/book_b2.png"),
	wall3(24.0f, 15.0f, 2.0f, 20.0f, Vec4(255, 255, 255, 255),"bookcase/book_v7.png"),
	triggerEvent(0.0f, 15.0f, 1.0f, 15.0f, Vec4(255, 0, 255, 0)),
	triggerEvent2(19.f, 17.0f, 5.5f, 1.0f, Vec4(255, 0, 255, 255))
{
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;

	std::cout << "this is scene A5\n";
}

SceneA5::~SceneA5(){
}

bool SceneA5::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// Set player image to PacMan

	SDL_Surface* image;
	SDL_Texture* texture;


	image = IMG_Load("pacman.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	//Load Textures
	Background.LoadTexture(renderer);
	platform1.LoadTexture(renderer);
	platform2.LoadTexture(renderer);
	platform3.LoadTexture(renderer);
	wall1.LoadTexture(renderer);
	wall2.LoadTexture(renderer);
	wall3.LoadTexture(renderer);

	if (game->GetSceneManager().GetLastScene() == DefineScenes::A4) {
		game->SetNewTriggerBox(triggerEvent.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}
	else if (game->GetSceneManager().GetLastScene() == DefineScenes::A8) {
		game->SetNewTriggerBox(triggerEvent2.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}
	else {
		game->getPlayer()->setPos(Vec3(3, 5, 0));
	}


	return true;
}

void SceneA5::OnDestroy() {
	Background.DestroyTexture();
	platform1.DestroyTexture();
	platform2.DestroyTexture();
	platform3.DestroyTexture();
	wall1.DestroyTexture();
	wall2.DestroyTexture();
	wall3.DestroyTexture();
}

void SceneA5::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);
	game->getPlayer()->wallTouchRight = false;
	game->getPlayer()->wallTouchLeft = false;

	triggerEvent.OnTriggerEnter(game, DefineScenes::A4, DefineScenes::A5);
	triggerEvent2.OnTriggerEnter(game, DefineScenes::A8, DefineScenes::A5);

	std::vector<SDL_FRect> builds = {
	platform1.getPlatform(),
	platform2.getPlatform(),
	platform3.getPlatform(),
	wall1.getPlatform(),
	wall2.getPlatform(),
	wall3.getPlatform()
	};

	if (game->getPlayer()->getAccel().y != 0.0f) {

		game->getPlayer()->isGrounded = false; //set isGrounded to true
	}

	//loop through platforms
	for (const SDL_FRect& build : builds) {
		//if the player has collided with the sides of one of the platforms
		if (game->getPlayer()->HasCollidedSide(build)) {
			//get the accel and vel of player and set the accel and vel to the current accel and vel other than x make it 0 to stop x motion when colliding
			Vec3 currentAccel = game->getPlayer()->getAccel();
			Vec3 currentVel = game->getPlayer()->getVel();
			game->getPlayer()->setAccel(Vec3(0.0f, currentAccel.y, currentAccel.z));
			game->getPlayer()->setVel(Vec3(0.0f, currentVel.y, currentVel.z));

			if (game->getPlayer()->getAccel().x > 0)
				game->getPlayer()->wallTouchLeft = true;
			else if (game->getPlayer()->getAccel().x < 0)
				game->getPlayer()->wallTouchRight = true;

		}

		//Check Collision
		if (game->getPlayer()->HasCollidedTop(build)) {
			//get the accel and vel of player and set the accel and vel to the current accel and vel other than y make it 0 to stop y motion when colliding
			Vec3 currentAccel = game->getPlayer()->getAccel();
			Vec3 currentVel = game->getPlayer()->getVel();
			game->getPlayer()->setAccel(Vec3(currentAccel.x, 0.0f, currentAccel.z));
			game->getPlayer()->setVel(Vec3(currentVel.x, 0.0f, currentVel.z));
			game->getPlayer()->isGrounded = true; //set isGrounded to true
		}

	}
}

void SceneA5::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	Background.Render(renderer, game);
	platform1.Render(renderer, game);
	platform2.Render(renderer, game);
	platform3.Render(renderer, game);
	wall3.Render(renderer, game);
	wall1.Render(renderer, game);
	wall2.Render(renderer, game);
	triggerEvent.Render(renderer, game);
	triggerEvent2.Render(renderer, game);

	// render the player
	game->RenderPlayer(0.10f);

	SDL_RenderPresent(renderer);
}

void SceneA5::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	game->SceneSwitching(event, DefineScenes::A);
}

bool SceneA5::RectsAreEqual(const SDL_FRect& rect1, const SDL_FRect& rect2)
{
	return (rect1.x == rect2.x &&
		rect1.y == rect2.y &&
		rect1.w == rect2.w &&
		rect1.h == rect2.h);
}
