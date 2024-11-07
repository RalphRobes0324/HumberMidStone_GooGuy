#include "SceneA2.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
SceneA2::SceneA2(SDL_Window* sdlWindow_, GameManager* game_) :
	platform1(0.0f, 2.0f, 7.0f, 2.0f, Vec4(255, 255, 255, 255)),
	platform2(12.0f, 2.0f, 12.5f, 2.0f, Vec4(255, 255, 255, 255)),
	platform3(0.0f, 15.0f, 6.0f, 0.5f, Vec4(255, 255, 255, 255)),
	platform4(15.0f, 15.0f, 12.0f, 0.5f, Vec4(255, 255, 255, 255)),
	wall(24.5f, 15.0f, 0.5f, 20.0f, Vec4(255, 255, 255, 255)),
	triggerEvent(0.0f, 14.5f, 1.0f, 15.0f, Vec4(255, 0, 255, 0)),
	triggerEvent2(5.5f, 17.0f, 10.0f, 1.0f, Vec4(255, 0, 255, 255)),
	redPlatform1(6.5f, 9.5f, 6.0f, 1.0f, true, true, 2.0f, Vec4(255, 0, 0, 255)),
	redPlatform2(6.5f, 4.5f, 6.0f, 1.0f, true, true, 2.0f, Vec4(255, 0, 0, 255)),
	redPlatform3(6.5f, 13.5f, 6.0f, 1.0f, true, true, 2.0f, Vec4(255, 0, 0, 255)),
	bluePlatform1(15.0f, 7.0f, 6.0f, 1.0f, true, false, 2.0f, Vec4(0, 0, 255, 255)),
	bluePlatform2(15.0f, 12.0f, 6.0f, 1.0f, true, false, 2.0f, Vec4(0, 0, 255, 255))
{
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;

	//safe guard
	std::cout << "this is scene A2\n";
}

SceneA2::~SceneA2(){
}

bool SceneA2::OnCreate() {
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

	if (game->GetSceneManager().GetLastScene() == DefineScenes::A1) {

		game->SetNewTriggerBox(triggerEvent.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}
	else {
		game->getPlayer()->setPos(Vec3(2.5, 5, 0));
	}

	return true;
}

void SceneA2::OnDestroy() {}

void SceneA2::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);

	triggerEvent.OnTriggerEnter(game, DefineScenes::A1, DefineScenes::A2);
	triggerEvent2.OnTriggerEnter(game, DefineScenes::A4, DefineScenes::A2);

	//Update the build
	redPlatform1.Update(deltaTime);
	redPlatform2.Update(deltaTime);
	redPlatform3.Update(deltaTime);
	bluePlatform1.Update(deltaTime);
	bluePlatform2.Update(deltaTime);

	std::vector<SDL_FRect> builds = {
		platform1.getPlatform(),
		platform2.getPlatform(),
		platform3.getPlatform(),
		platform4.getPlatform(),
		wall.getPlatform()
	};

	if (redPlatform1.getVisibility() == true) {
		builds.push_back(redPlatform1.getPlatform());
		builds.push_back(redPlatform2.getPlatform());
		builds.push_back(redPlatform3.getPlatform());
	}
	if (bluePlatform1.getVisibility() == true) {
		builds.push_back(bluePlatform1.getPlatform());
		builds.push_back(bluePlatform2.getPlatform());
	}
	

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

void SceneA2::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	platform1.Render(renderer, game);
	platform2.Render(renderer, game);
	platform3.Render(renderer, game);
	platform4.Render(renderer, game);
	wall.Render(renderer, game);
	triggerEvent.Render(renderer, game);
	triggerEvent2.Render(renderer, game);

	redPlatform1.Render(renderer, game);
	redPlatform2.Render(renderer, game);
	redPlatform3.Render(renderer, game);
	bluePlatform1.Render(renderer, game);
	bluePlatform2.Render(renderer, game);
	// render the player
	game->RenderPlayer(0.10f);

	SDL_RenderPresent(renderer);
}

void SceneA2::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	game->SceneSwitching(event, DefineScenes::A);
}

bool SceneA2::RectsAreEqual(const SDL_FRect& rect1, const SDL_FRect& rect2)
{
	return (rect1.x == rect2.x &&
		rect1.y == rect2.y &&
		rect1.w == rect2.w &&
		rect1.h == rect2.h);
}
