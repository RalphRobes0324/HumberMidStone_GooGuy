#pragma once

#include <SDL_mixer.h>
#include <string>
#include <unordered_map>


class GameManager;

class AudioManager
{

private:
	static AudioManager* instance;
	AudioManager();
	~AudioManager();
	std::unordered_map<std::string, Mix_Chunk*> m_sounds;
	std::unordered_map<std::string, Mix_Music*> m_music;

public:
	static AudioManager* Instance();
	static void Release();
	bool LoadSound(const std::string& id, const std::string& path);
	bool LoadMusic(const std::string& id, const std::string& path);
	void PlaySound(const std::string& id, int loop = 0);
	void PlayMusic(const std::string& id, int loops = -1);


	void PauseMusic();
	void ResumeMusic();
	void StopMusic();

	void Clean();





};

