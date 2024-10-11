#include "JumpText.h"
#include <iostream>

JumpText::JumpText(SDL_Renderer* renderer_, SDL_Window* window_)
    :
    renderer(renderer_),
    window(window_),
    jumpFont(nullptr),
    jumpTexture(nullptr)
{}

JumpText::~JumpText() {
    Cleanup();
}

bool JumpText::LoadFont(const char* fontPath, int fontSize) {
    // Initialize SDL_ttf only if it hasn't been initialized yet
    if (TTF_WasInit() == 0) {
        if (TTF_Init() == -1) {
            std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
            return false;
        }
    }

    // Load the font
    jumpFont = TTF_OpenFont(fontPath, fontSize);
    if (jumpFont == nullptr) {
        std::cerr << "Failed to load Jump font: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void JumpText::SetText(const std::string& text) {
    textToDisplay = text;
}

void JumpText::Render() {
    if (textToDisplay.empty()) return;

    SDL_Color textColor = { 0, 255, 0, 255 }; // Green text
    SDL_Surface* textSurface = TTF_RenderText_Solid(jumpFont, textToDisplay.c_str(), textColor);

    if (!textSurface) {
        std::cerr << "Failed to create text surface for '" << textToDisplay << "': " << TTF_GetError() << std::endl;
        return;
    }

    jumpTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!jumpTexture) {
        std::cerr << "Failed to create texture for '" << textToDisplay << "': " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return;
    }

    // Get the window size
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // Set the jumpRect position to the bottom right corner
    jumpRect = { windowWidth - textSurface->w - 10, windowHeight - textSurface->h - 10, textSurface->w, textSurface->h };

    SDL_FreeSurface(textSurface); // Free the surface after creating the texture

    // Render the jump text
    SDL_RenderCopy(renderer, jumpTexture, nullptr, &jumpRect);
}

void JumpText::Cleanup() {
    if (jumpTexture) {
        SDL_DestroyTexture(jumpTexture);
        jumpTexture = nullptr;
    }
    if (jumpFont) {
        TTF_CloseFont(jumpFont);
        jumpFont = nullptr;
    }
}