#pragma once

#include <al.h>
#include <alc.h>
#include <iostream>
#include <map>

inline namespace AudioSource
{
	int generate();
	int checkState(int sourceID);
	void pauseAudio(int sourceID);
	void continueAudio(int sourceID);
	void stopAudio(int sourceID);
	void remove(unsigned int sourceID);
	void playAudio(int sourceID, int bufferID);
	void setVelocity(int sourceID, float x, float y, float z);
	void setPos(int sourceID, float x, float y, float z);
	void setPitch(int sourceID, float pitch);
	void setGain(int sourceID, float gain);
	void setLooping(int sourceID, bool doLoop);
	void setRolloff(int sourceID, float factor);
	void setRefDistance(int sourceID, float factor);
	void setMaxDistance(int sourceID, float factor);

};

inline namespace AudioStream {
	extern std::map<std::string, int> _audio_buffers_;

	void AddBuffer(std::string name, int buffer);
	int GetBuffer(std::string name);
	void Clean();
}


