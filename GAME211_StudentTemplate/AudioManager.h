#pragma once

#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

class AudioManager
{

private:
	static AudioManager* instance;
	AudioManager();
	~AudioManager();
	std::unordered_map<std::string, Mix_Chunk*> m_sounds;
	std::unordered_map<std::string, Mix_Music*> m_music;
	void FreeCurrrentMusic();

public:
	static AudioManager* Instance();
	bool LoadSound(const std::string& id, const std::string& path);
	bool LoadMusic(const std::string& id, const std::string& path);
	void PlaySound(const std::string& id, int loop = 0);
	void PlayMusic(const std::string& id, int loops = -1);


	void PauseMusic();
	void ResumeMusic();
	void StopMusic();

	void Clean();


	bool muteAudio = false;




};

