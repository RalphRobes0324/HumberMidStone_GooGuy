#include "SceneA2.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
SceneA2::SceneA2(SDL_Window* sdlWindow_, GameManager* game_) :
	Background(0.0f, 15.0f, 30.0f, 15.0f, Vec4(255, 255, 255, 255), "bookcase/book_bg.png"),
	platform1(0.0f, 2.0f, 7.0f, 2.5f, Vec4(255, 255, 255, 255), "bookcase/book_h1.png"),
	platform2(12.0f, 2.0f, 12.5f, 2.5f, Vec4(255, 255, 255, 255), "bookcase/book_h1.png"),
	platform3(0.0f, 15.5f, 6.0f, 1.5f, Vec4(255, 255, 255, 255), "bookcase/book_h1.png"),
	platform4(15.0f, 15.5f, 12.0f, 1.5f, Vec4(255, 255, 255, 255), "bookcase/book_h1.png"),
	wall(24.0f, 15.0f, 2.0f, 20.0f, Vec4(255, 255, 255, 255), "bookcase/book_v7.png"),
	triggerEvent(0.0f, 14.5f, 1.0f, 15.0f, Vec4(255, 0, 255, 0)),
	triggerEvent2(5.5f, 17.0f, 10.0f, 1.0f, Vec4(255, 0, 255, 255)),
	deathTriggerEvent(-20.f, -2.f, 100.f, 1.f, Vec4(0,0,255,255)),
	redPlatform1(6.5f, 9.5f, 6.0f, 1.0f, true, true, 2.0f, Vec4(255, 0, 0, 255), "bookcase/book_h1.png"),
	redPlatform2(6.5f, 4.5f, 6.0f, 1.0f, true, true, 2.0f, Vec4(255, 0, 0, 255), "bookcase/book_h1.png"),
	redPlatform3(6.5f, 13.5f, 6.0f, 1.0f, true, true, 2.0f, Vec4(255, 0, 0, 255), "bookcase/book_h1.png"),
	bluePlatform1(15.0f, 7.0f, 6.0f, 1.0f, true, false, 2.0f, Vec4(0, 0, 255, 255), "bookcase/book_h1.png"),
	bluePlatform2(15.0f, 12.0f, 6.0f, 1.0f, true, false, 2.0f, Vec4(0, 0, 255, 255), "bookcase/book_h1.png"),
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

	//Load Textures
	wall.LoadTexture(renderer);
	platform1.LoadTexture(renderer);
	platform2.LoadTexture(renderer);
	platform3.LoadTexture(renderer);
	platform4.LoadTexture(renderer);
	redPlatform1.LoadTexture(renderer);
	redPlatform2.LoadTexture(renderer);
	redPlatform3.LoadTexture(renderer);
	bluePlatform1.LoadTexture(renderer);
	bluePlatform2.LoadTexture(renderer);
	Background.LoadTexture(renderer);

	if (game->GetSceneManager().GetLastScene() == DefineScenes::A1) {

		game->SetNewTriggerBox(triggerEvent.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}
	else if (game->GetSceneManager().GetLastScene() == DefineScenes::A4) {
		game->SetNewTriggerBox(triggerEvent2.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}
	else {
		game->getPlayer()->setPos(Vec3(2.5, 5, 0));
	}

	return true;
}

void SceneA2::OnDestroy() {
	wall.DestroyTexture();
	platform1.DestroyTexture();
	platform2.DestroyTexture();
	platform3.DestroyTexture();
	platform4.DestroyTexture();
	redPlatform1.DestroyTexture();
	redPlatform2.DestroyTexture();
	redPlatform3.DestroyTexture();
	bluePlatform1.DestroyTexture();
	bluePlatform2.DestroyTexture();
	Background.DestroyTexture();
}

void SceneA2::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);
	game->getPlayer()->wallTouchRight = false;
	game->getPlayer()->wallTouchLeft = false;

	triggerEvent.OnTriggerEnter(game, DefineScenes::A1, DefineScenes::A2);
	triggerEvent2.OnTriggerEnter(game, DefineScenes::A4, DefineScenes::A2);
	deathTriggerEvent.OnTriggerEnter(game);

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

void SceneA2::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	Background.Render(renderer, game);
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

	deathTriggerEvent.Render(renderer, game);

	// render the player
	game->RenderPlayer(0.10f);

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
