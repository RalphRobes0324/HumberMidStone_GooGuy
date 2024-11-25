#include "SceneC1.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
SceneC1::SceneC1(SDL_Window* sdlWindow_, GameManager* game_) :
	Background(0.0f, 15.0f, 30.0f, 15.0f, Vec4(255, 255, 255, 255), "freezer/freezer_bg_2.png"),
	platform1(-0.5f, 1.0f, 10.5f, 8.0f, Vec4(255, 255, 255, 255), "freezer/freezer_acunit_heat.png"),
	platform2(5.0f, 2.0f, 5.0f, 1.0f, Vec4(255, 255, 255, 255), "freezer/freezer_h2.png"),
	platform3(15.0f, 5.0f, 6.0f, 1.0f, Vec4(255, 255, 255, 255), "freezer/freezer_h2.png"),
	redPlatform(17.0f, 8.0f, 2.0f, 1.0f, true, true, 2.0f, Vec4(255, 0, 0, 255), "freezer/freezer_h5.png"),
	triggerEvent(25, 15.f, 1.0f, 18.0f, Vec4(255, 0, 255, 255)),
	deathTriggerEvent(-20.f, -2.f, 100.f, 1.f, Vec4(0, 0, 255, 255)),
	deathTriggerEvent2(-2.0f, 14.5f, 1.0f, 15.0f, Vec4(0, 0, 255, 255)),
	Overlay(0.0f, 15.0f, 30.0f, 15.0f, Vec4(255, 255, 255, 255), "freezer/freezer_bg_overlay2.png"),
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
}

SceneC1::~SceneC1(){
}

bool SceneC1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	isOverlayUpdated = false;

	//Load Textures
	Background.LoadTexture(renderer);
	platform1.LoadTexture(renderer);
	platform2.LoadTexture(renderer);
	platform3.LoadTexture(renderer);
	redPlatform.LoadTexture(renderer);
	Overlay.LoadTexture(renderer);

	//Check last scene was * DONT DELETE THIS*
	if (game->GetSceneManager().GetLastScene() == DefineScenes::C2) {
		game->SetNewTriggerBox(triggerEvent.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}
	else {
		game->getPlayer()->setPos(Vec3(2.5, 5, 0));
	}

	

	std::cout << "this is scene C1\n";

	return true;
}

void SceneC1::OnDestroy() {
	//Destroy Texture
	Background.DestroyTexture();
	platform1.DestroyTexture();
	platform2.DestroyTexture();
	platform3.DestroyTexture();
	redPlatform.DestroyTexture();
	Overlay.DestroyTexture();
}

void SceneC1::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);
	game->getPlayer()->wallTouchRight = false;
	game->getPlayer()->wallTouchLeft = false;

	redPlatform.Update(deltaTime);

	//set distination
	triggerEvent.OnTriggerEnter(game, DefineScenes::C2, DefineScenes::C1);

	deathTriggerEvent.OnTriggerEnter(game);
	deathTriggerEvent2.OnTriggerEnter(game);

	if (TemperatureManager::Instance().GetTemperature() <= 50) {
		if (!isOverlayUpdated) {
			Overlay.UpdateTexture(renderer, "freezer/freezer_bg_overlay1.png");
			isOverlayUpdated = true;
		}
	}

	std::vector<SDL_FRect> builds = {
	platform1.getPlatform(),
	platform2.getPlatform(),
	platform3.getPlatform()
	};
	if (redPlatform.getVisibility() == true)
		builds.push_back(redPlatform.getPlatform());

	if (game->getPlayer()->getAccel().y != 0.0f) {

		game->getPlayer()->isGrounded = false; //set isGrounded to true
	}

	TemperatureManager::Instance().SetHotPlatform(false);

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

			// Check if on hot platform or not
			if (RectsAreEqual(build, platform1.getPlatform()) || RectsAreEqual(build, redPlatform.getPlatform())) {
				TemperatureManager::Instance().SetHotPlatform(true);
			}
		}
	}

	if (!game->getPlayer()->isGrounded || !TemperatureManager::Instance().GetHotPlatform())
		TemperatureManager::Instance().DecreaseTemperature(2.0f * deltaTime, game);
}

void SceneC1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	Background.Render(renderer, game);
	platform1.Render(renderer, game);
	platform2.Render(renderer, game);
	platform3.Render(renderer, game);
	redPlatform.Render(renderer, game);
	triggerEvent.Render(renderer, game);
	Overlay.Render(renderer, game);

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

void SceneC1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event); 
	game->SceneSwitching(event, DefineScenes::C);
}

bool SceneC1::RectsAreEqual(const SDL_FRect& rect1, const SDL_FRect& rect2)
{
	return (rect1.x == rect2.x &&
		rect1.y == rect2.y &&
		rect1.w == rect2.w &&
		rect1.h == rect2.h);
}
