#include "SceneA6.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
SceneA6::SceneA6(SDL_Window* sdlWindow_, GameManager* game_) :
	Background(0.0f, 15.0f, 30.0f, 15.0f, Vec4(255, 255, 255, 255), "bookcase/book_bg.png"),
	platform1(0.0f, 15.5f, 25.0f, 1.5f, Vec4(255, 255, 255, 255), "bookcase/book_h1.png"),
	platform2(5.0f, 2.0f, 6.0f, 2.5f, Vec4(255, 255, 255, 255), "bookcase/book_h1.png"),
	platform3(20.0f, 2.0f, 12.0f, 2.5f, Vec4(255, 255, 255, 255), "bookcase/book_h1.png"),
	platform4(11.0f, 5.0f, 8.0, 1.0f, Vec4(255, 255, 255, 255), "bookcase/book_h1.png"),
	wall(-0.75f, 15.0f, 1.5f, 20.0f, Vec4(255, 255, 255, 255), "bookcase/book_v7.png"),
	triggerEvent(0.0f, 0.0f, 20.f, 1.0f, Vec4(255, 0, 255, 255)),
	triggerEvent2(25.0f, 14.5f, 1.f, 15.0f, Vec4(255, 0, 255, 255))
{
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;

	std::cout << "this is scene A6\n";
}

SceneA6::~SceneA6(){
}

bool SceneA6::OnCreate() {
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
	platform4.LoadTexture(renderer);
	wall.LoadTexture(renderer);

	if (game->GetSceneManager().GetLastScene() == DefineScenes::A3) {
		game->SetNewTriggerBox(triggerEvent.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}	
	else if (game->GetSceneManager().GetLastScene() == DefineScenes::A7) {
		game->SetNewTriggerBox(triggerEvent2.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}
	else {
		game->getPlayer()->setPos(Vec3(8, 5, 0));
	}
	return true;
}

void SceneA6::OnDestroy() {
	Background.DestroyTexture();
	platform1.DestroyTexture();
	platform2.DestroyTexture();
	platform3.DestroyTexture();
	platform4.DestroyTexture();
	wall.DestroyTexture();
}

void SceneA6::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);

	triggerEvent.OnTriggerEnter(game, DefineScenes::A3, DefineScenes::A6);
	triggerEvent2.OnTriggerEnter(game, DefineScenes::A7, DefineScenes::A6);


	std::vector<SDL_FRect> builds = {
	platform1.getPlatform(),
	platform2.getPlatform(),
	platform3.getPlatform(),
	platform4.getPlatform(),
	wall.getPlatform()
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

void SceneA6::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	Background.Render(renderer, game);
	platform1.Render(renderer, game);
	platform2.Render(renderer, game);
	platform3.Render(renderer, game);
	platform4.Render(renderer, game);
	wall.Render(renderer, game);
	triggerEvent.Render(renderer, game);

	// render the player
	game->RenderPlayer(0.10f);

	SDL_RenderPresent(renderer);
}

void SceneA6::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	game->SceneSwitching(event, DefineScenes::A);
}

bool SceneA6::RectsAreEqual(const SDL_FRect& rect1, const SDL_FRect& rect2)
{
	return (rect1.x == rect2.x &&
		rect1.y == rect2.y &&
		rect1.w == rect2.w &&
		rect1.h == rect2.h);
}
