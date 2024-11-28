#include "SceneD5.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
SceneD5::SceneD5(SDL_Window* sdlWindow_, GameManager* game_) :
	Background(0.0f, 15.0f, 30.0f, 15.0f, Vec4(255, 255, 255, 255), "greenhouse/gh_bg.png"),
	platform1(0.0f, 2.0f, 8.0f, 2.0f, Vec4(255, 255, 255, 255), "greenhouse/h1.png"),
	platform2(4.5f, 13.0f, 8.0f, 1.0f, Vec4(255, 255, 255, 255), "greenhouse/h1.png"),
	platform3(10.0f, 5.0f, 8.0f, 1.0f, Vec4(255, 255, 255, 255), "greenhouse/h1.png"),
	venusFlyTrap(19.5f, 6.5f, 4.5f, 3.5f, Vec4(255, 255, 255, 255), "greenhouse/open.png"),
	stem(21.25f, 4.5f, 1.5f, 6.5f, Vec4(255, 255, 255, 255), "greenhouse/stem.png"),
	wall(0.0f, 15.0f, 2.0f, 20.0f, Vec4(255, 255, 255, 255), "greenhouse/v1.png"),
	blueWall(18.0f, 12.0f, 1.0f, 6.0f, true, false, 2.0f, Vec4(0, 0, 255, 255), "greenhouse/v1_b.png"),
	redPlatform(19.5f, 13.0f, 6.0f, 1.0f, true, true, 2.0f, Vec4(255, 0, 0, 255), "greenhouse/h1_r.png"),
	triggerEvent(2, 17, 23, 1, Vec4(255, 0, 255, 255)),
	triggerEvent2(25, 15, 1, 18, Vec4(255, 0, 255, 255)),
	venusTrapTrigger(19.5f, 6.f, 4.f, 1.f, Vec4(255, 0, 255, 255)),
	venusPlatform(19.5f, 5.f, 4.5f, 3.5f, Vec4(255, 255, 0, 255)),
	deathTriggerEvent(-20.f, -2.f, 100.f, 1.f, Vec4(255, 0, 255, 255)),
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

SceneD5::~SceneD5(){
}

bool SceneD5::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	//Load Textures
	Background.LoadTexture(renderer);
	platform1.LoadTexture(renderer);
	platform2.LoadTexture(renderer);
	platform3.LoadTexture(renderer);
	wall.LoadTexture(renderer);
	venusFlyTrap.LoadTexture(renderer);
	stem.LoadTexture(renderer);
	redPlatform.LoadTexture(renderer);
	blueWall.LoadTexture(renderer);

	//Check last scene was
	if (game->GetSceneManager().GetLastScene() == DefineScenes::D2) {
		game->SetNewTriggerBox(triggerEvent.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}
	else if (game->GetSceneManager().GetLastScene() == DefineScenes::D6) {
		game->SetNewTriggerBox(triggerEvent2.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}
	else {
		game->getPlayer()->setPos(Vec3(4, 5, 0));
	}

	std::cout << "this is scene D5\n";

	return true;
}

void SceneD5::OnDestroy() {
	//Destroy Texture
	Background.DestroyTexture();
	platform1.DestroyTexture();
	platform2.DestroyTexture();
	platform3.DestroyTexture();
	blueWall.DestroyTexture();
	stem.DestroyTexture();
	redPlatform.DestroyTexture();
	wall.DestroyTexture();
	venusFlyTrap.DestroyTexture();
}

void SceneD5::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);
	game->getPlayer()->wallTouchRight = false;
	game->getPlayer()->wallTouchLeft = false;

	deathTriggerEvent.OnTriggerEnter(game);
	venusTrapTrigger.OnTriggerStay(deltaTime, game, renderer, venusFlyTrap);

	//set distination
	triggerEvent.OnTriggerEnter(game, DefineScenes::D2, DefineScenes::D5);
	triggerEvent2.OnTriggerEnter(game, DefineScenes::D6, DefineScenes::D5);

	//update the build 
	redPlatform.Update(deltaTime);
	blueWall.Update(deltaTime);

	std::vector<SDL_FRect> builds = {
	platform1.getPlatform(),
	platform2.getPlatform(),
	platform3.getPlatform(),
	wall.getPlatform(),
	venusPlatform.getPlatform()
	};

	if (redPlatform.getVisibility() == true)
		builds.push_back(redPlatform.getPlatform());

	if (blueWall.getVisibility() == true)
		builds.push_back(blueWall.getPlatform());

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
		}
		if (game->getPlayer()->HasCollidedBottom(build) && !(game->getPlayer()->HasCollidedTop(build) || game->getPlayer()->HasCollidedSide(build))) {
			//get the accel and vel of player and set the accel and vel to the current accel and vel other than y make it 0 to stop y motion when colliding
			Vec3 currentAccel = game->getPlayer()->getAccel();
			Vec3 currentVel = game->getPlayer()->getVel();
			game->getPlayer()->setAccel(Vec3(currentAccel.x, -currentAccel.y, currentAccel.z));
			game->getPlayer()->setVel(Vec3(currentVel.x, -currentVel.y, currentVel.z));
			game->getPlayer()->isGrounded = true; //set isGrounded to true
		}

	}
}

void SceneD5::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	Background.Render(renderer, game);
	platform1.Render(renderer, game);
	platform2.Render(renderer, game);
	platform3.Render(renderer, game);
	stem.Render(renderer, game);
	
	wall.Render(renderer, game);
	blueWall.Render(renderer, game);
	redPlatform.Render(renderer, game);
	triggerEvent.Render(renderer, game);
	triggerEvent2.Render(renderer, game);

	if(!venusTrapTrigger.trapTriggered)
		venusFlyTrap.Render(renderer, game);

	// render the player
	game->RenderPlayer(0.10f);

	if (venusTrapTrigger.trapTriggered)
		venusFlyTrap.Render(renderer, game);

	

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

void SceneD5::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	if (!venusTrapTrigger.trapTriggered) 
		game->getPlayer()->HandleEvents(event);

	game->SceneSwitching(event, DefineScenes::D);
}

bool SceneD5::RectsAreEqual(const SDL_FRect& rect1, const SDL_FRect& rect2)
{
	return (rect1.x == rect2.x &&
		rect1.y == rect2.y &&
		rect1.w == rect2.w &&
		rect1.h == rect2.h);
}
