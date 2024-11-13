#include "SceneC4.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
SceneC4::SceneC4(SDL_Window* sdlWindow_, GameManager* game_) :
	platform1(9, 4, 7.5, 1, Vec4(255, 255, 255, 255)),
	platform2(20, 7, 8, 1, Vec4(255, 255, 255, 255)),
	bluePlatform1(-3, 5.0f, 6.0f, 1.0f, true, true, 2.0f, Vec4(0, 0, 255, 255)),
	bluePlatform2(11.0f, 7, 3.0f, 1.0f, true, true, 2.0f, Vec4(0, 0, 255, 255)),
	triggerEvent(0, 4, 1, 2, Vec4(0, 255, 255, 255)),
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

SceneC4::~SceneC4(){
}

bool SceneC4::OnCreate() {
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
	//if (game->GetSceneManager().GetLastScene() == DefineScenes::A1) {
	//	game->SetNewTriggerBox(triggerEvent.getPlatform());
	//	game->HandleSpawnPoint(.2f, 1.f);
	//	game->getPlayer()->setPos(game->GetPlayerNewPos());
	//}

	game->getPlayer()->setPos(Vec3(2.5, 5, 0));

	std::cout << "this is scene C4\n";

	return true;
}

void SceneC4::OnDestroy() {}

void SceneC4::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);

	//set distination
	//triggerEvent.OnTriggerEnter(game, DefineScenes::A1, DefineScenes::A2);

	bluePlatform1.Update(deltaTime);
	bluePlatform2.Update(deltaTime);

	std::vector<SDL_FRect> builds = {
		platform1.getPlatform(),
		platform2.getPlatform()
	};

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

			// Check if player reached a certain platform
			if (RectsAreEqual(build, platform1.getPlatform())) {
				quest.UpdateQuest(1); // Touching platform 1
			}
		}

	}
}

void SceneC4::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	platform1.Render(renderer, game);
	platform2.Render(renderer, game);
	//triggerEvent.Render(renderer, game);

	bluePlatform1.Render(renderer, game);
	bluePlatform2.Render(renderer, game);

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

void SceneC4::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	game->SceneSwitching(event, DefineScenes::C);
}

bool SceneC4::RectsAreEqual(const SDL_FRect& rect1, const SDL_FRect& rect2)
{
	return (rect1.x == rect2.x &&
		rect1.y == rect2.y &&
		rect1.w == rect2.w &&
		rect1.h == rect2.h);
}
