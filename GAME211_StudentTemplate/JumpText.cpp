#include "JumpText.h"
#include <iostream>

JumpText::JumpText(SDL_Renderer* renderer_, SDL_Window* window_)
    :
    renderer(renderer_),
    window(window_),
    jumpImage(nullptr),
    wallJumpImage(nullptr)
{}

JumpText::~JumpText() {
    Cleanup();
}

bool JumpText::LoadImages(const char* jumpImagePath, const char* wallJumpImagePath) {
    // load jump image
    SDL_Surface* jumpSurface = IMG_Load(jumpImagePath);
    if (!jumpSurface) {
        std::cerr << "Failed Loading Jump Image: " << IMG_GetError() << std::endl;
        return false;
    }
    jumpImage = SDL_CreateTextureFromSurface(renderer, jumpSurface);
    SDL_FreeSurface(jumpSurface);

    // load wall jump image
    SDL_Surface* wallJumpSurface = IMG_Load(wallJumpImagePath);
    if (!wallJumpSurface) {
        std::cerr << "Failed Loading Wall Jump Image: " << IMG_GetError() << std::endl;
        return false;
    }
    wallJumpImage = SDL_CreateTextureFromSurface(renderer, wallJumpSurface);
    SDL_FreeSurface(wallJumpSurface);

    return jumpImage && wallJumpImage;
}

void JumpText::RenderJump() {
    if (!jumpImage) return;

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    int originalWidth, originalHeight;
    // get dimensions of image
    SDL_QueryTexture(jumpImage, nullptr, nullptr, &originalWidth, &originalHeight);
    
    // resize image
    jumpRect.w = 150;
    jumpRect.h = 75;

    // bottom right corner
    jumpRect.x = windowWidth - jumpRect.w - 160;
    jumpRect.y = windowHeight - jumpRect.h - 10;

    // Render Jump Image
    SDL_RenderCopy(renderer, jumpImage, nullptr, &jumpRect);
}

void JumpText::RenderWallJump() {
    if (!wallJumpImage) return;

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    int originalWidth, originalHeight;
    // get dimensions of image
    SDL_QueryTexture(wallJumpImage, nullptr, nullptr, &originalWidth, &originalHeight);

    // resize image
    jumpRect.w = 150;
    jumpRect.h = 75;

    // bottom right corner
    jumpRect.x = windowWidth - jumpRect.w - 160;
    jumpRect.y = windowHeight - jumpRect.h - 10;

    // Render Wall Jump Image
    SDL_RenderCopy(renderer, wallJumpImage, nullptr, &jumpRect);
}

void JumpText::Cleanup() {
    if (jumpImage) {
        SDL_DestroyTexture(jumpImage);
        jumpImage = nullptr;
    }
    if (wallJumpImage) {
        SDL_DestroyTexture(wallJumpImage);
        wallJumpImage = nullptr;
    }
}