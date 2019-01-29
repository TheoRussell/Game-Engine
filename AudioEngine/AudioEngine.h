#pragma once

#define _audio_h_

#include "AudioFile.h"
#include "AudioSource.h"
#include "Scene.h"

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();
	void playSound(std::string name, std::string sound);
	unsigned int newSource(std::string name);
	void update(Scene & scene);
	void clean();
	void setListenerPos(float x, float y, float z);
	void setListenerVelocity(float x, float y, float z);
private:
	std::map < std::string, ALuint > audioBuffers;
	std::map <std::string ,ALuint> audioSources;


	std::vector<unsigned int> sourceID;

	ALCdevice * device;
	ALCcontext * context;
};

