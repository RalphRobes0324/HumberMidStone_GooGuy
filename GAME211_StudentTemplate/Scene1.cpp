#include "Scene1.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_) 
	:
	//init the build
	platform1(100, 500, 300, 20),
	platform2 (500, 500, 300 , 20),
	platform3 (900, 500, 300, 20),
	wall1(55, 100, 20, 400),
	wall2(200, 50, 20, 300)
{

	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;

}

Scene1::~Scene1() {
	
}

bool Scene1::OnCreate() {
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

	return true;
}

void Scene1::OnDestroy() {}

void Scene1::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);

	//Update the build
	platform1.Update();
	platform2.Update();
	platform3.Update();
	wall1.Update();
	wall2.Update();

	//Testing collision code
	SDL_Rect testPlatform = platform2.getPlatform();

	testPlatform.x = 12.0f; //hard coded x and y for the platform to check the collision code is working
	testPlatform.y = 2.0f;
	
	Vec3 playerPos = game->getPlayer()->getPos(); //get player position for printing to console
	//print to console the location of the test platform and the player
	std::cout << "test platform: " << testPlatform.x << " " << testPlatform.y;
	std::cout << "player position: " << playerPos.x << " " << playerPos.y << " " << playerPos.z << " " << std::endl;

	//if the player is colliding with the test platform
	if (game->getPlayer()->HasCollidedWith(testPlatform)) {
		//get the accel and vel of player and set the accel and vel to the current accel and vel other than y make it 0 to stop y motion when colliding
		Vec3 currentAccel = game->getPlayer()->getAccel(); 
		Vec3 currentVel = game->getPlayer()->getVel();
		game->getPlayer()->setAccel(Vec3(currentAccel.x, 0.0f, currentAccel.z));
		game->getPlayer()->setVel(Vec3(currentVel.x, 0.0f, currentVel.z)) ;
		game->getPlayer()->isGrounded = true; //set isGrounded to true
	}
	else {

		game->getPlayer()->isGrounded = false; //if you aren't colliding set is grounded to false
	}
	
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// render the player
	game->RenderPlayer(0.10f);

	// Render the platforms
	platform1.Render(renderer);
	platform2.Render(renderer);
	platform3.Render(renderer);
	wall1.Render(renderer);
	wall2.Render(renderer);

	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
}
