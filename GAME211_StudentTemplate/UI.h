#ifndef UI_H
#define UI_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class UI {
private:
	SDL_Rect buttonRect;		// button size/position
	SDL_Texture* buttonTexture; // default button
	SDL_Texture* hoverTexture;	// button on hover
	SDL_Renderer* renderer;		// button renderer

public:
	UI(SDL_Renderer* renderer_, const std::string& imagePath, const std::string& hoverImagePath, SDL_Rect rect);
	~UI();

	void Render();
	bool IsHovered(int mouseX, int mouseY);
	void HandleEvent(const SDL_Event& event, void (*onClick)());
};

#endif