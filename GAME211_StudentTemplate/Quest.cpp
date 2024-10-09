#include "Quest.h"
#include <iostream>

Quest::Quest(SDL_Renderer* renderer_)
    :
    renderer(renderer_),
    questFont(nullptr),
    questTexture(nullptr),
    currentQuestIndex(0),
    highestQuestIndex(0)
{}

Quest::~Quest() {
    Cleanup();
}

bool Quest::LoadFont(const char* fontPath, int fontSize) {
    // Initialize SDL_ttf only if it hasn't been initialized yet
    if (TTF_WasInit() == 0) {
        if (TTF_Init() == -1) {
            std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
            return false;
        }
    }

    // Load the font
    questFont = TTF_OpenFont(fontPath, fontSize);
    if (questFont == nullptr) {
        std::cerr << "Failed to Load Quest Font: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void Quest::AddQuest(const std::string& quest) {
    quests.push_back(quest);
}

void Quest::RenderCurrentQuest() {
    if (quests.empty()) return;

    SDL_Color questColor = { 255, 255, 255, 255 };
    SDL_Surface* questSurface = TTF_RenderText_Solid(questFont, quests[currentQuestIndex].c_str(), questColor);
    if (questSurface) {
        questTexture = SDL_CreateTextureFromSurface(renderer, questSurface);
        questRect = { 10, 10, questSurface->w, questSurface->h };
        SDL_FreeSurface(questSurface);
    }

    if (questTexture) {
        SDL_RenderCopy(renderer, questTexture, nullptr, &questRect);
    }
}

void Quest::UpdateQuest(int platformNumber) {
    if (platformNumber < 1 || platformNumber > quests.size()) return;

    if (platformNumber - 1 > highestQuestIndex) {
        highestQuestIndex = platformNumber - 1;
        currentQuestIndex = highestQuestIndex;
    }
}

void Quest::Cleanup() {
    if (questFont) {
        TTF_CloseFont(questFont);
        questFont = nullptr;
    }
    if (questTexture) {
        SDL_DestroyTexture(questTexture);
        questTexture = nullptr;
    }
    if (TTF_WasInit()) {
        TTF_Quit();
    }
}