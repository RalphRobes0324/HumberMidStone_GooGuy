#pragma once

class DefineScenes
{
public:

	enum TypeOfScenes {
		A,
		B,
		C,
		D,
		MENU
	};
	enum GameScenes
	{
		NONE,
		MAIN_MENU,
		OPTION_MENU,
		WIN_MENU,
		DEATH_MENU,
		BEGINNING_SCREEN,
		LEVEL_SELECT_MENU,
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
		D4,
		D5,
		D6
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

