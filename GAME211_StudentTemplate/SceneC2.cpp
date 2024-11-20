#include "SceneC2.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
SceneC2::SceneC2(SDL_Window* sdlWindow_, GameManager* game_) :
	platform1(3, 8, 4, 1, Vec4(255, 255, 255, 255)),
	bluePlatform(10.5f, 5.0f, 6.0f, 1.0f, true, false, 2.0f, Vec4(0, 0, 255, 255)),
	blueWall(18.5f, 14.0f, 1.0f, 10.0f, true, false, 2.0f, Vec4(0, 0, 255, 255)),
	redPlatform(21.5f, 5.0f, 6.0f, 1.0f, true, true, 2.0f, Vec4(255, 0, 0, 255)),
	triggerEvent(0.0f, 15.f, 1, 18.f, Vec4(255, 0, 255, 0)),
	triggerEvent2(25.0f, 15.f, 1, 18.f, Vec4(255, 0, 255, 255)),
	quest(SDL_GetRenderer(sdlWindow_)),
	jumpText(SDL_GetRenderer(sdlWindow_), sdlWindow_),
	movementText(SDL_GetRenderer(sdlWindow_), sdlWindow_)
{
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;

	if (!quest.LoadFont("Atop-R99O3.ttf", 24)) {
		std::cerr << "Failed to load quest font" << std::endl;
	}

	if (!jumpText.LoadImages("jump.png", "wall_jump.png")) {
		std::cerr << "Failed to load jump images" << std::endl;
	}

	if (!movementText.LoadImages("movement.png", "left_movement.png", "right_movement.png")) {
		std::cerr << "Failed to load jump images" << std::endl;
	}

	// Set Quests
	quest.AddQuest("Quest 1");

	float currentTemperature = TemperatureManager::Instance().GetTemperature();
	std::cout << "Current Temperature: " << currentTemperature << std::endl;
}

SceneC2::~SceneC2(){
}

bool SceneC2::OnCreate() {
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

	//Check last scene was
	if (game->GetSceneManager().GetLastScene() == DefineScenes::C1) {
		game->SetNewTriggerBox(triggerEvent.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}
	else if (game->GetSceneManager().GetLastScene() == DefineScenes::C3) {
		game->SetNewTriggerBox(triggerEvent2.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}
	else {
		game->getPlayer()->setPos(Vec3(5.f, 9.f, 0));
	}

	

	std::cout << "this is scene C2\n";

	return true;
}

void SceneC2::OnDestroy() {}

void SceneC2::Update(const float deltaTime) {

	// Update Temperature
	float currentTemperature = TemperatureManager::Instance().GetTemperature();

	// Update player
	game->getPlayer()->Update(deltaTime);
	game->getPlayer()->wallTouchRight = false;
	game->getPlayer()->wallTouchLeft = false;

	//set distination
	triggerEvent.OnTriggerEnter(game, DefineScenes::C1, DefineScenes::C2);
	triggerEvent2.OnTriggerEnter(game, DefineScenes::C3, DefineScenes::C2);

	bluePlatform.Update(deltaTime);
	blueWall.Update(deltaTime);
	redPlatform.Update(deltaTime);

	std::vector<SDL_FRect> builds = {
		platform1.getPlatform()
	};

	if (redPlatform.getVisibility() == true) {
		builds.push_back(redPlatform.getPlatform());
	}
	if (bluePlatform.getVisibility() == true) {
		builds.push_back(bluePlatform.getPlatform());
		builds.push_back(blueWall.getPlatform());
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

			// Check if player reached a certain platform
			if (RectsAreEqual(build, platform1.getPlatform())) {
				quest.UpdateQuest(1); // Touching platform 1
			}

			if (!RectsAreEqual(build, redPlatform.getPlatform())) {
				TemperatureManager::Instance().DecreaseTemperature(5.0f * deltaTime);
			}
		}

		if (!game->getPlayer()->isGrounded)
			TemperatureManager::Instance().DecreaseTemperature(1.0f * deltaTime);
	}
}

void SceneC2::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	platform1.Render(renderer, game);
	triggerEvent.Render(renderer, game);
	triggerEvent2.Render(renderer, game);

	bluePlatform.Render(renderer, game);
	blueWall.Render(renderer, game);
	redPlatform.Render(renderer, game);

	// render the player
	game->RenderPlayer(0.10f);

	// Render temperature meter
	TemperatureManager::Instance().RenderTemperature(renderer);

	// Render Quest
	quest.RenderCurrentQuest();

	// Determine which text to render based on player state
	// Jump Text
	if (game->getPlayer()->isGrounded && !game->getPlayer()->wallTouchLeft && !game->getPlayer()->wallTouchRight) {
		jumpText.RenderJump();
	}
	else if (game->getPlayer()->wallTouchLeft || game->getPlayer()->wallTouchRight) {
		jumpText.RenderWallJump();
	}

	// Movement Text
	if (!game->getPlayer()->wallTouchLeft && !game->getPlayer()->wallTouchRight) {
		movementText.RenderMovement();
	}
	else if (game->getPlayer()->wallTouchLeft) {
		movementText.RenderRightMovement();
	}
	else if (game->getPlayer()->wallTouchRight) {
		movementText.RenderLeftMovement();
	}

	SDL_RenderPresent(renderer);
}

void SceneC2::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	game->SceneSwitching(event, DefineScenes::C);
}

bool SceneC2::RectsAreEqual(const SDL_FRect& rect1, const SDL_FRect& rect2)
{
	return (rect1.x == rect2.x &&
		rect1.y == rect2.y &&
		rect1.w == rect2.w &&
		rect1.h == rect2.h);
}
