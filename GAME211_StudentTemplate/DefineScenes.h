#pragma once

class DefineScenes
{
public:
	enum GameScenes
	{
		NONE,
		MAIN_MENU,
		OPTION_MENU,
		A1,
		A2

	};


private:
	GameScenes currScene;
	GameScenes lastScene;

public:
	GameScenes GetCurrentScene() { return currScene; }
	GameScenes GetLastScene() { return lastScene; }

	void SetCurrentScene(GameScenes _currScene) { currScene = _currScene;  }
	void SetLastScene(GameScenes _lastScene) { lastScene = _lastScene;  }

};

