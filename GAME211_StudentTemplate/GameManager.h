#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <SDL.h>
#include <iostream>
#include "Window.h"
#include "Timer.h"
#include "Scene.h"
#include "PlayerBody.h"
#include "DefineScenes.h"
#include "Build.h"
#include <string.h>

class AudioManager;



class GameManager {
private:
	/// These are called "forward declarations" The idea is that a pointer is 
	/// really just an unsigned int, so to create a pointer to it you really only
	/// need to tell the compiler that there is a class called Window and I need
	/// a pointer to it, thus the "class Window*" means trust me, there is a class 
	/// called Window, I just need a pointer to it.

	/// If that was too much for your brain, just #include "Window.h" and declare
	/// Window *ptr and don't use the word "class"  This is a second semester C++
	/// topic anyway
	class Window *windowPtr;
	class Timer *timer;
	bool isRunning;
	class Scene *currentScene;

	// This might be unfamiliar
    class PlayerBody *player;

	void handleEvents();
	void LoadScene(int i);
	bool ValidateCurrentScene();
	Uint32 changeScene;

	Vec3 newPlayerPos;
	Vec3 oldPlayerPos;

	SDL_FRect newSpawn;
	SDL_FRect oldSpawn;


	DefineScenes sceneManager;


	

	bool isMuted = false;

public:
	AudioManager* audioManager;

	GameManager();
	~GameManager();
	bool OnCreate();
	void OnDestroy();

	DefineScenes& GetSceneManager() { return sceneManager; }


	// These might be unfamiliar
	float getSceneHeight();
	float getSceneWidth();
	Matrix4 getProjectionMatrix();
    PlayerBody* getPlayer(){ return player; }
	void RenderPlayer(float scale = 1.0f);
	SDL_Renderer* getRenderer();

	void Run();

	Uint32 GetChangeScene();

	void SetPlayerNewPos(Vec3 newPos) { newPlayerPos = newPos; }
	Vec3 GetPlayerNewPos() { return newPlayerPos;  }

	void SetPlayerOldPos(Vec3 old) { oldPlayerPos = old; }
	Vec3 GetPlayerOldPos() { return oldPlayerPos; }

	SDL_FRect GetOldTriggerBox() const { return oldSpawn; }
	void SetOldTriggerBox(const SDL_FRect& _rect) { oldSpawn = _rect; }

	SDL_FRect GetNewTriggerBox() const { return newSpawn; }
	void SetNewTriggerBox(const SDL_FRect& _rect) { newSpawn = _rect; } //set new destination
	void HandleSpawnPoint(const float offset, const float topOffset);

	void SceneSwitching(SDL_Event event, DefineScenes::TypeOfScenes sceneType);
	void SwitchScene(DefineScenes::TypeOfScenes sceneType, int num);
	void SwitchScene(DefineScenes::GameScenes scenes);

	bool GetMute() const;
	void SetMute(bool _isMuted);

	void PlayerSFX(const std::string& audioName);
};
#endif