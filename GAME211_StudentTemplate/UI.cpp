#include "UI.h"
#include <iostream>

UI::UI(SDL_Renderer* renderer_, const std::string& imagePath, const std::string& hoverImagePath, SDL_Rect rect)
    : renderer(renderer_), buttonRect(rect) {
    // Load default button textures
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    buttonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // load on hover button textures
    surface = IMG_Load(hoverImagePath.c_str());
    hoverTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

UI::~UI() {
    SDL_DestroyTexture(buttonTexture);
    SDL_DestroyTexture(hoverTexture);
}

void UI::Render() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Render hover texture if hovered, otherwise render normal texture
    if (IsHovered(mouseX, mouseY)) {
        SDL_RenderCopy(renderer, hoverTexture, nullptr, &buttonRect);
    } else {
        SDL_RenderCopy(renderer, buttonTexture, nullptr, &buttonRect);
    }
}

bool UI::IsHovered(int mouseX, int mouseY) {
    return mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
        mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h;
}

void UI::HandleEvent(const SDL_Event& event, std::function<void()> onClick) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        if (IsHovered(mouseX, mouseY) && event.button.button == SDL_BUTTON_LEFT) {
            onClick();
        }
    }
}

void UI::SetPosition(int x, int y) {
    buttonRect.x = x;
    buttonRect.y = y;
}