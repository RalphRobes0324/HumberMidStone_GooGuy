#include "AudioManager.h"
#include <iostream>

AudioManager AudioManager::Instance()
{
	static AudioManager instance;
	return instance;
}

AudioManager::AudioManager()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
}

AudioManager::~AudioManager()
{
	Clean();
	Mix_CloseAudio();
}
