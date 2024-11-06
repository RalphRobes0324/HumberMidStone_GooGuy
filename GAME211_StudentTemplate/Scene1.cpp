#include "Scene1.h"
#include <VMath.h>


// See notes about this constructor in Scene1.h.
Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_)
	:
	//init the build
	platform1(0, 2, 8, 2, Vec4(255, 255, 255, 255)),
	platform2(12, 2, 6, 2, Vec4(255, 255, 255, 255)),
	platform3(22, 2, 6, 2, Vec4(255, 255, 255, 255)),
	triggerEvent(24, 4, 1, 2, Vec4(0,255, 255, 255)),
	wall1(6, 10, 2, 9, Vec4(255, 255, 255, 255)),
	wall2(0, 10, 2, 9, Vec4(255, 255, 255, 255)),
	redPlatform(11, 10, 6, 1, true, true, 2.0f, Vec4(255, 0, 0, 255)),
	bluePlatform(20, 10, 6, 1, true, false, 2.0f, Vec4(0, 0, 255, 255)),
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
	quest.AddQuest("Quest 1: Escape the Test Tube");
	quest.AddQuest("Quest 2: Find a Way Out of the Experimentation Room");
	quest.AddQuest("Quest 3: Run from the Amalgamation");
	quest.AddQuest("Quest 4: Locate the Exit");
	quest.AddQuest("Quest 5: Leave the Lab");


	std::cout << "this is scene A1\n";
}

Scene1::~Scene1() {
	
}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f); 
	projectionMatrix = ndc * ortho;

	// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// Set player image to PacMan
	SDL_Surface* image;
	SDL_Texture* texture;

	image = IMG_Load("pacman.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	//set Player position when spawned into world
	if (game->GetSceneManager().GetLastScene() == DefineScenes::NONE) {
		game->getPlayer()->setPos(Vec3(3, 5, 0));
	}
	else if (game->GetSceneManager().GetLastScene() == DefineScenes::A2) {
		game->SetNewTriggerBox(triggerEvent.getPlatform());
		game->HandleSpawnPoint(.2f, 1.f);
		game->getPlayer()->setPos(game->GetPlayerNewPos());
	}
	
	

	return true;
}

void Scene1::OnDestroy() {
	jumpText.Cleanup();
}

void Scene1::Update(const float deltaTime) {


	//Update Camera *NOTE DOES NOT WORK. MUST ASK PROF FOR HELP*

	// Update player
	game->getPlayer()->Update(deltaTime);


	//Update the build
	redPlatform.Update(deltaTime);
	bluePlatform.Update(deltaTime);

	triggerEvent.OnTriggerEnter(game, DefineScenes::A2, DefineScenes::A1);

	std::vector<SDL_Rect> builds = {
		platform1.getPlatform(),
		platform2.getPlatform(),
		platform3.getPlatform(),
		wall1.getPlatform(),
		wall2.getPlatform()
	};
	if (redPlatform.getVisibility() == true)
		builds.push_back(redPlatform.getPlatform());
	if (bluePlatform.getVisibility() == true)
		builds.push_back(bluePlatform.getPlatform());

	if (game->getPlayer()->getAccel().y != 0.0f) {

		game->getPlayer()->isGrounded = false; //set isGrounded to true
		}

	//loop through platforms
	for (const SDL_Rect& build : builds) {
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
			else if (RectsAreEqual(build, platform2.getPlatform())) {
				quest.UpdateQuest(2); // Touching platform 2
			}
			else if (RectsAreEqual(build, platform3.getPlatform())) {
				quest.UpdateQuest(3); // Touching platform 3
			}
		}

	}
}

void Scene1::Render() {	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// Render the platforms
	platform1.Render(renderer, game);
	platform2.Render(renderer, game);
	platform3.Render(renderer, game);
	triggerEvent.Render(renderer, game);
	wall1.Render(renderer, game);
	wall2.Render(renderer, game);
	redPlatform.Render(renderer, game);
	bluePlatform.Render(renderer, game);


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

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
	game->SceneSwitching(event, DefineScenes::A);
}

bool Scene1::RectsAreEqual(const SDL_Rect& rect1, const SDL_Rect& rect2) {
	return (rect1.x == rect2.x &&
		rect1.y == rect2.y &&
		rect1.w == rect2.w &&
		rect1.h == rect2.h);
}