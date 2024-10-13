#ifndef MOVEMENTTEXT_H
#define MOVEMENTTEXT_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class MovementText {
private:
    SDL_Texture* movementImage;
    SDL_Texture* leftMovementImage;
    SDL_Texture* rightMovementImage;

    SDL_Rect movementRect;
    SDL_Renderer* renderer;
    SDL_Window* window;

public:
    MovementText(SDL_Renderer* renderer_, SDL_Window* window_);
    ~MovementText();

    bool LoadImages(const char* movementImagePath, const char* leftMovementImagePath, const char* rightMovementImagePath);
    void RenderMovement();
    void RenderLeftMovement();
    void RenderRightMovement();
    void Cleanup();
};

#endif