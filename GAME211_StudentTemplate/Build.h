#ifndef BUILD_H
#define BUILD_H

#include <SDL.h>
#include "GameManager.h"
#include "Body.h"
#include "DefineScenes.h"


class Build {
public:
	//Setting up Default Build
	Build(float _x, float _y, float _w, float _h, Vec4 _colour);
	Build(float _x, float _y, float _w, float _h, Vec4 _colour, const std::string& _texturePath);

	//Setting up Disappearing Build
	Build(float _x, float _y, float _w, float _h, bool _canDisappear, bool _isVisible, float _disappearTime, Vec4 _colour);
	Build(float _x, float _y, float _w, float _h, bool _canDisappear, bool _isVisible, float _disappearTime, Vec4 _colour, const std::string& _texturePath);

	~Build();

	void LoadTexture(SDL_Renderer* renderer);

	void DestroyTexture();
	
	void Render(SDL_Renderer* renderer, GameManager* game);

	void Update();
	void Update(float DeltaTime);

	SDL_FRect getPlatform() { return rect; }//added getter to get the platform so it can be used for checks of collision (Maya)

	bool getVisibility() { return isVisible; } //added to return visibility for collision checks (Maya)
	
	void OnTriggerEnter(GameManager* game, DefineScenes::GameScenes newScene, DefineScenes::GameScenes lastScene);

	void OnTriggerStay(float DeltaTime, GameManager* game);

	bool isPlayerInTriggerBox(GameManager* game);

	// Fan Functions
	void SetFanDirection(float _direction) { fanDirection = _direction; }
	float GetFanDirection() const { return fanDirection; }
	void SetFanPower(float _power) { fanPower = _power; }
	float GetFanPower() const { return fanPower; }
	void SetEffectiveRange(float range) { effectiveRange = range; }
	float GetEffectiveRange() const { return effectiveRange; }
	void Update(float DeltaTime, GameManager* game);
	void UpdateTexture(SDL_Renderer* renderer, const std::string& newTexturePath);

private:
	float x, y, width, height;
	SDL_FRect rect;

	//Type of Build
	bool canDisappear;

	//Settings Visiability
	bool isVisible; //Display Build
	bool isWarning; //state if platform will warn the player
	float disappearTime; //length
	float warningTime; //Time to start warning the player
	int alpha; //Alpha of build
	int alphaEnds = 50; //Where alpha ends

	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = nullptr;
	std::string texturePath;

	Vec4 colour; //Build's colour
	
	float timer;


	float stateTimer = 1.5f;

	// Fan Info
	bool fanDirection;
	float fanPower;
	float effectiveRange;
};

#endif

