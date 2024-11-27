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

/// <summary>
/// Load SFX
/// </summary>
/// <param name="id"> Name of ID that will be called upon </param>
/// <param name="path"> Destination of WAV file </param>
/// <returns></returns>
bool AudioManager::LoadSound(const std::string& id, const std::string& path)
{
	Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
	if (!sound) {
		std::cerr << "Failed to load sound: " << path << " SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	m_sounds[id] = sound;
	return true;
}


/// <summary>
/// Load Music
/// </summary>
/// <param name="id"> Name of ID that will be called upon </param>
/// <param name="path"> Destination of MP3 file </param>
/// <returns></returns>
bool AudioManager::LoadMusic(const std::string& id, const std::string& path)
{
	Mix_Music* music = Mix_LoadMUS(path.c_str());
	if (!music) {
		std::cerr << "Failed to load music: " << path << " SDL_mixer Error: " << Mix_GetError() << std::endl;
		return false;
	}
	m_music[id] = music;
	return true;
}

/// <summary>
/// Play SFX
/// </summary>
/// <param name="id"> id of SFX </param>
/// <param name="loop"> is Looping? 0 = No Looping </param>
void AudioManager::PlaySound(const std::string& id, int loop)
{
	auto audio = m_sounds.find(id);
	if (audio != m_sounds.end()) {
		Mix_PlayChannel(-1, audio->second, loop);
	}
	else {
		std::cerr << "Sound ID not found: " << id << std::endl;
	}
}

/// <summary>
/// Player Music
/// </summary>
/// <param name="id"> id of Music </param>
/// <param name="loops"> is Looping? -1 = Yes looping </param>
void AudioManager::PlayMusic(const std::string& id, int loops) {
	auto it = m_music.find(id);
	if (it != m_music.end()) {
		Mix_PlayMusic(it->second, loops);
	}
	else {
		std::cerr << "Music ID not found: " << id << std::endl;
	}
}
