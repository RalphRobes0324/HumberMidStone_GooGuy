#ifndef QUEST_H
#define QUEST_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

class Quest {
private:
    int currentQuestIndex; // sets the current quest index
    int highestQuestIndex; // track the latest quest
    std::vector<std::string> quests; // vector of strings to hold the quests

    // SDL_ttf Rendering
    TTF_Font* questFont;
    SDL_Texture* questTexture;
    SDL_Rect questRect;
    SDL_Renderer* renderer;

public:
    Quest(SDL_Renderer* renderer);
    ~Quest();

    bool LoadFont(const char* fontPath, int fontSize);
    void AddQuest(const std::string& quest);
    void RenderCurrentQuest();

    void UpdateQuest(int platformNumber);

    // Cleanup resources
    void Cleanup();
};

#endif