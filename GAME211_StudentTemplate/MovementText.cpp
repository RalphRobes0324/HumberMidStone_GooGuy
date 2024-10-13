#include "MovementText.h"
#include <iostream>

MovementText::MovementText(SDL_Renderer* renderer_, SDL_Window* window_)
    : 
    renderer(renderer_), 
    window(window_), 
    movementImage(nullptr),
    leftMovementImage(nullptr),
    rightMovementImage(nullptr)
{}

MovementText::~MovementText() {
    Cleanup();
}

bool MovementText::LoadImages(const char* movementImagePath, const char* leftMovementImagePath, const char* rightMovementImagePath) {
    // load movement image
    SDL_Surface* movementSurface = IMG_Load(movementImagePath);
    if (!movementSurface) {
        std::cerr << "Failed Loading Movement Image: " << IMG_GetError() << std::endl;
        return false;
    }
    movementImage = SDL_CreateTextureFromSurface(renderer, movementSurface);
    SDL_FreeSurface(movementSurface);

    // load left movement image
    SDL_Surface* leftMovementSurface = IMG_Load(leftMovementImagePath);
    if (!leftMovementSurface) {
        std::cerr << "Failed Loading Left Movement Image: " << IMG_GetError() << std::endl;
        return false;
    }
    leftMovementImage = SDL_CreateTextureFromSurface(renderer, leftMovementSurface);
    SDL_FreeSurface(leftMovementSurface);

    // load right movement image
    SDL_Surface* rightMovementSurface = IMG_Load(rightMovementImagePath);
    if (!rightMovementSurface) {
        std::cerr << "Failed Loading Right Movement Image: " << IMG_GetError() << std::endl;
        return false;
    }
    rightMovementImage = SDL_CreateTextureFromSurface(renderer, rightMovementSurface);
    SDL_FreeSurface(rightMovementSurface);

    return movementImage && leftMovementImage && rightMovementImage;
}

void MovementText::RenderMovement() {
    if (!movementImage) return;

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    int originalWidth, originalHeight;
    // get dimensions of image
    SDL_QueryTexture(movementImage, nullptr, nullptr, &originalWidth, &originalHeight);

    // resize image
    movementRect.w = 150;
    movementRect.h = 75;

    // bottom right corner
    movementRect.x = windowWidth - movementRect.w - 10;
    movementRect.y = windowHeight - movementRect.h - 10;

    // Render Movement Image
    SDL_RenderCopy(renderer, movementImage, nullptr, &movementRect);
}

void MovementText::RenderLeftMovement() {
    if (!leftMovementImage) return;

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    int originalWidth, originalHeight;
    // get dimensions of image
    SDL_QueryTexture(leftMovementImage, nullptr, nullptr, &originalWidth, &originalHeight);

    // resize image
    movementRect.w = 150;
    movementRect.h = 75;

    // bottom right corner
    movementRect.x = windowWidth - movementRect.w - 10;
    movementRect.y = windowHeight - movementRect.h - 10;

    // Render Movement Image
    SDL_RenderCopy(renderer, leftMovementImage, nullptr, &movementRect);
}

void MovementText::RenderRightMovement() {
    if (!rightMovementImage) return;

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    int originalWidth, originalHeight;
    // get dimensions of image
    SDL_QueryTexture(rightMovementImage, nullptr, nullptr, &originalWidth, &originalHeight);

    // resize image
    movementRect.w = 150;
    movementRect.h = 75;

    // bottom right corner
    movementRect.x = windowWidth - movementRect.w - 10;
    movementRect.y = windowHeight - movementRect.h - 10;

    // Render Movement Image
    SDL_RenderCopy(renderer, rightMovementImage, nullptr, &movementRect);
}

void MovementText::Cleanup() {
    if (movementImage) {
        SDL_DestroyTexture(movementImage);
        movementImage = nullptr;
    }
    if (leftMovementImage) {
        SDL_DestroyTexture(leftMovementImage);
        leftMovementImage = nullptr;
    }
    if (rightMovementImage) {
        SDL_DestroyTexture(rightMovementImage);
        rightMovementImage = nullptr;
    }
}