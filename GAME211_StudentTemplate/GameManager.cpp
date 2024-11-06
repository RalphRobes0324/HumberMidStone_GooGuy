#include "GameManager.h"
#include "Scene1.h"
#include "SceneA2.h"
#include "SceneA3.h"
#include "SceneA4.h"
#include "SceneA5.h"
#include "SceneA6.h"
#include "SceneA7.h"
#include "SceneA8.h"
#include "SceneB1.h"
#include "SceneB2.h"
#include "SceneB3.h"
#include "SceneB4.h"
#include "SceneB5.h"
#include "SceneB6.h"
#include "CopyBaseScene.h"

GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
    player = nullptr;
}

bool GameManager::OnCreate() {
    // My display is 1920 x 1080 but the following seems to work best to fill the screen.
    //const int SCREEN_WIDTH = 1540;
    //const int SCREEN_HEIGHT = 860;

    // Use 1000x600 for less than full screen
    const int SCREEN_WIDTH = 1000;
    const int SCREEN_HEIGHT = 600;

    windowPtr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (windowPtr == nullptr) {
		OnDestroy();
		return false;
	}
	if (windowPtr->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		OnDestroy();
		return false;
	}

    // select scene for specific assignment

    currentScene = new Scene1(windowPtr->GetSDL_Window(), this);
    
    // create player
    float mass = 1.0f;
    float radius = 0.5f;
    float orientation = 0.0f;
    float rotation = 0.0f;
    float angular = 0.0f;
    Vec3 position(0.5f * currentScene->getxAxis(), 0.5f * currentScene->getyAxis(), 0.0f);
    Vec3 velocity(0.0f, 0.0f, 0.0f);
    Vec3 acceleration(0.0f, 0.0f, 0.0f);

    player = new PlayerBody
    (
        position,
        velocity,
        acceleration,
        mass,
        radius,
        orientation,
        rotation,
        angular,
        this
    );
    if ( player->OnCreate() == false ) {
        OnDestroy();
        return false;
    }

    // need to create Player before validating scene
    if (!ValidateCurrentScene()) {
        OnDestroy();
        return false;
    }

    // create user define event 
    changeScene = SDL_RegisterEvents(20);
    if (changeScene == ((Uint32) - 1)) {
        OnDestroy();
        return false;
    }

    sceneManager.SetCurrentScene(DefineScenes::A1);
    sceneManager.SetLastScene(DefineScenes::NONE); //DONT FORGET CHANGE WHEN MAIN MENU IS BUILT

	return true;
}


/// Here's the whole game loop
void GameManager::Run() {
    
	timer->Start();
    
	while (isRunning) {
        
        handleEvents();
		timer->UpdateFrameTicks();
        currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();

		/// Keep the event loop running at a proper rate
		SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
	}
}

Uint32 GameManager::GetChangeScene()
{
    return changeScene;
}


/// <summary>
/// Setting spawn point
/// </summary>
/// <param name="offset">Adjust offset on x-axis</param>
/// <param name="topOffset">Adjust offset on y-axis</param>
void GameManager::HandleSpawnPoint(const float offset, const float topOffset)
{
    Vec3 playerPos = oldPlayerPos;

    float radius = player->getRadius();
    Vec3 newPosition;

    float overlapRight = (oldSpawn.x + oldSpawn.w) - (playerPos.x - radius);
    float overlapLeft = (playerPos.x + radius) - oldSpawn.x;
    float overlapTop = (oldSpawn.y + oldSpawn.h) - (playerPos.y - radius);
    float overlapBottom = (playerPos.y + radius) - oldSpawn.y;

    float minHorizontalOverlap = std::min(overlapLeft, overlapRight);
    float minVerticalOverlap = std::min(overlapTop, overlapBottom);

    if (minHorizontalOverlap < 0.5f && minVerticalOverlap < 0.90f) {
        if (overlapLeft < overlapRight)
        {
            // Coming from the left, spawn on the right side of the new trigger box
            newPosition.x = newSpawn.x + newSpawn.w + radius + offset;
            newPosition.y = playerPos.y;
            //std::cout << "Coming from left side\n";
        }
        else 
        {
            newPosition.x = newSpawn.x - radius - offset;
            newPosition.y = playerPos.y;
            //std::cout << "Coming from right side\n";
        }
    }
    else
    {
        if (minVerticalOverlap > 0.75) 
        {

            newPosition.y = newSpawn.y - radius - topOffset;
            newPosition.x = newSpawn.x + offset;
            //std::cout << "Coming from top side\n";
            
  
        }
        else
        {
            newPosition.y = newSpawn.y + newSpawn.h + radius + topOffset;
            newPosition.x = newSpawn.x + offset;
            //std::cout << "Coming from bottom side\n";
        }
    }


    SetPlayerNewPos(newPosition);
}

void GameManager::SceneSwitching(SDL_Event event, DefineScenes::TypeOfScenes sceneType)
{
    if (event.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = SDL_GetKeyFromScancode(event.key.keysym.scancode);

        if (key >= SDLK_0 && key <= SDLK_9)
        {
            int numberPressed = key - SDLK_0;
            SwitchScene(sceneType, numberPressed);
        }
    }
}

void GameManager::SwitchScene(DefineScenes::TypeOfScenes sceneType, int num)
{
    SDL_Event event;
    if (sceneType == DefineScenes::A) {
        if (num == 1) {
            GetSceneManager().SetCurrentScene(DefineScenes::A1);
            SDL_memset(&event, 0, sizeof(event));
            event.type = GetChangeScene();
            event.user.code = 1;
            event.user.data1 = nullptr;
            event.user.data2 = nullptr;
            SDL_PushEvent(&event);
        }
        else if (num == 2) {
            GetSceneManager().SetCurrentScene(DefineScenes::A2);
            SDL_memset(&event, 0, sizeof(event));
            event.type = GetChangeScene();
            event.user.code = 1;
            event.user.data1 = nullptr;
            event.user.data2 = nullptr;
            SDL_PushEvent(&event);
        }
        else if (num == 3) {
            GetSceneManager().SetCurrentScene(DefineScenes::A3);
            SDL_memset(&event, 0, sizeof(event));
            event.type = GetChangeScene();
            event.user.code = 1;
            event.user.data1 = nullptr;
            event.user.data2 = nullptr;
            SDL_PushEvent(&event);
        }
        else if (num == 4) {
            GetSceneManager().SetCurrentScene(DefineScenes::A4);
            SDL_memset(&event, 0, sizeof(event));
            event.type = GetChangeScene();
            event.user.code = 1;
            event.user.data1 = nullptr;
            event.user.data2 = nullptr;
            SDL_PushEvent(&event);
        }
        else if (num == 5) {
            GetSceneManager().SetCurrentScene(DefineScenes::A5);
            SDL_memset(&event, 0, sizeof(event));
            event.type = GetChangeScene();
            event.user.code = 1;
            event.user.data1 = nullptr;
            event.user.data2 = nullptr;
            SDL_PushEvent(&event);
        }
        else if (num == 6) {
            GetSceneManager().SetCurrentScene(DefineScenes::A6);
            SDL_memset(&event, 0, sizeof(event));
            event.type = GetChangeScene();
            event.user.code = 1;
            event.user.data1 = nullptr;
            event.user.data2 = nullptr;
            SDL_PushEvent(&event);
        }
        else if (num == 7) {
            GetSceneManager().SetCurrentScene(DefineScenes::A7);
            SDL_memset(&event, 0, sizeof(event));
            event.type = GetChangeScene();
            event.user.code = 1;
            event.user.data1 = nullptr;
            event.user.data2 = nullptr;
            SDL_PushEvent(&event);
        }
        else if (num == 8) {
            GetSceneManager().SetCurrentScene(DefineScenes::A8);
            SDL_memset(&event, 0, sizeof(event));
            event.type = GetChangeScene();
            event.user.code = 1;
            event.user.data1 = nullptr;
            event.user.data2 = nullptr;
            SDL_PushEvent(&event);
        }
    }
    else if (sceneType == DefineScenes::B) {

    }
}

void GameManager::handleEvents() 
{
    SDL_Event event;

    // Let's add mouse movement and position
    // https://wiki.libsdl.org/SDL_GetMouseState

    SDL_PumpEvents();  // make sure we have the latest mouse state.

    //https://www.youtube.com/watch?v=SYrRMr4BaD4&list=PLM7LHX-clszBIGsrh7_3B2Pi74AhMpKhj&index=3

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false; 
        }
        else if (event.type == changeScene) {
            //switch scene
            currentScene->OnDestroy();
            delete currentScene;
            if (sceneManager.GetCurrentScene() == DefineScenes::A1) {
                currentScene = new Scene1(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::A2) {
                currentScene = new SceneA2(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::A3) {
                currentScene = new SceneA3(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::A4) {
                currentScene = new SceneA4(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::A5) {
                currentScene = new SceneA5(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::A6) {
                currentScene = new SceneA6(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::A7) {
                currentScene = new SceneA7(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::A8) {
                currentScene = new SceneA8(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::B1) {
                currentScene = new SceneB1(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::B2) {
                currentScene = new SceneB2(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::B3) {
                currentScene = new SceneB3(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::B4) {
                currentScene = new SceneB4(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::B5) {
                currentScene = new SceneB5(windowPtr->GetSDL_Window(), this);
            }
            else if (sceneManager.GetCurrentScene() == DefineScenes::B6) {
                currentScene = new SceneB6(windowPtr->GetSDL_Window(), this);
            }

            if (!currentScene->OnCreate()) {
                OnDestroy();
                isRunning = false;
            }
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                isRunning = false;
                break;
            case SDL_SCANCODE_Q:
                isRunning = false;
                break;
            case SDL_SCANCODE_DELETE:
                isRunning = false;
                break;
            case SDL_SCANCODE_1:
                //LoadScene(2);
                break;
            default:
                break;
            }
        }
        currentScene->HandleEvents(event);
    }
}

GameManager::~GameManager() {}

void GameManager::OnDestroy(){
	if (windowPtr) delete windowPtr;
	if (timer) delete timer;
	if (currentScene) delete currentScene;
}

// This might be unfamiliar
float GameManager::getSceneHeight() { return currentScene->getyAxis(); }

// This might be unfamiliar
float GameManager::getSceneWidth() { return currentScene->getxAxis(); }

// This might be unfamiliar
Matrix4 GameManager::getProjectionMatrix()
{ return currentScene->getProjectionMatrix(); }

// This might be unfamiliar
SDL_Renderer* GameManager::getRenderer()
{
    // [TODO] might be missing some SDL error checking
    SDL_Window* window = currentScene->getWindow();
    SDL_Renderer* renderer = SDL_GetRenderer(window);
    return renderer;
}

// This might be unfamiliar
void GameManager::RenderPlayer(float scale)
{
    player->Render(scale);
}

void GameManager::LoadScene( int i )
{
    // cleanup of current scene before loading another one
    currentScene->OnDestroy();
    delete currentScene;

    switch ( i )
    {
        case 1:
            currentScene = new Scene1( windowPtr->GetSDL_Window(), this);
            break;
        default:
            currentScene = new Scene1( windowPtr->GetSDL_Window(), this );
            break;
    }

    // using ValidateCurrentScene() to safely run OnCreate
    if (!ValidateCurrentScene())
    {
        isRunning = false;
    }
}

bool GameManager::ValidateCurrentScene()
{
    if (currentScene == nullptr) {
        return false;
    }
    if (currentScene->OnCreate() == false) {
        return false;
    }
    return true;
}
