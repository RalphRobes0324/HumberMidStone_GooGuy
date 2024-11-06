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
		A2,
		A3,
		A4,
		A5,
		A6,
		A7,
		A8,
		B1,
		B2,
		B3,
		B4,
		B5,
		B6,
		C1,
		C2,
		C3,
		C4,
		D1,
		D2,
		D3,
		D4
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

