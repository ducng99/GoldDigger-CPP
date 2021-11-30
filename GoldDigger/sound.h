#ifndef __SOUND_H__
#define __SOUND_H__

#include <fmod.hpp>
#include <map>
#include <string>

class SoundManager
{
public:
	static SoundManager& GetInstance();
	static void DestroyInstance();
	~SoundManager();

	void Initialise();

	FMOD::Sound* GetSound(const char* pcFilename);
	void PlaySound(FMOD::Sound* sound);
private:
	SoundManager(const SoundManager& sound);
	SoundManager& operator=(const SoundManager& sound);

	SoundManager();

private:
	static SoundManager* sm_pInstance;
	FMOD::Channel* m_pChannel;
	FMOD::System* m_pFMODSystem;

	std::map<std::string, FMOD::Sound*> m_pLoadedSounds;
};

#endif // !__SOUND_H__
