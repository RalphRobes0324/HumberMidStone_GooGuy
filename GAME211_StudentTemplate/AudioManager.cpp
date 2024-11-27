#include "AudioManager.h"
#include "GameManager.h"
#include <iostream>
AudioManager* AudioManager::instance = nullptr;

AudioManager* AudioManager::Instance() {
	if (instance == nullptr) {
		instance = new AudioManager();
	}
	return instance;
}

void AudioManager::Release()
{
	delete instance;
	instance = nullptr;
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
/// Play Music
/// </summary>
/// <param name="id"> id of Music </param>
/// <param name="loops"> is Looping? -1 = Yes looping </param>
void AudioManager::PlayMusic(const std::string& id, int loops) {
	auto music = m_music.find(id);
	if (music != m_music.end()) {
		Mix_PlayMusic(music->second, loops);
	}
	else {
		std::cerr << "Music ID not found: " << id << std::endl;
	}
}


void AudioManager::PauseMusic()
{
	Mix_PauseMusic();
}

void AudioManager::ResumeMusic()
{
	Mix_ResumeMusic();
}

void AudioManager::StopMusic()
{
	if (Mix_PlayingMusic()) {
		Mix_HaltMusic();
	}
}

/// <summary>
/// Clear Music list
/// </summary>
void AudioManager::Clean() {
	for (auto& audio : m_sounds) {
		Mix_FreeChunk(audio.second);
	}
	m_sounds.clear();

	for (auto& music : m_music) {
		Mix_FreeMusic(music.second);
	}
	m_music.clear();
}
