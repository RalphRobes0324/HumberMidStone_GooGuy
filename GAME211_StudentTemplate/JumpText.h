#ifndef JUMPTEXT_H
#define JUMPTEXT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class JumpText {
private:
    std::string textToDisplay; // Text Displayed

    // SDL_ttf Rendering
    TTF_Font* jumpFont;
    SDL_Texture* jumpTexture;
    SDL_Rect jumpRect;
    SDL_Renderer* renderer;
    SDL_Window* window;

public:
    JumpText(SDL_Renderer* renderer_, SDL_Window* window_);
    ~JumpText();

    bool LoadFont(const char* fontPath, int fontSize);
    void SetText(const std::string& text);
    void Render();
    void Cleanup();
};

#endif