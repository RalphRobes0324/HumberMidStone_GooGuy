#ifndef JUMPTEXT_H
#define JUMPTEXT_H

#include <SDL.h>
#include <string>
#include <SDL_image.h>

class JumpText {
private:
    std::string textToDisplay; // Text Displayed

    // SDL_ttf Rendering
    SDL_Texture* jumpImage;
    SDL_Texture* wallJumpImage;

    SDL_Rect jumpRect;
    SDL_Renderer* renderer;
    SDL_Window* window;

public:
    JumpText(SDL_Renderer* renderer_, SDL_Window* window_);
    ~JumpText();

    bool LoadImages(const char* jumpImagePath, const char* wallJumpImagePath);
    void RenderJump();
    void RenderWallJump();
    void Cleanup();
};

#endif