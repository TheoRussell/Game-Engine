#include "AudioSource.h"


int AudioSource::generate() {
	//Uses OpenAL to generate a source buffer.
	alGetError();
	ALuint sourceID;
	alGenSources((ALuint)1, &sourceID);
	if (alGetError() != AL_NO_ERROR) {
		std::cout << "Error generating audio source" << std::endl;
	}
	//The source has a position, velocity, pitch and gain.
	alSourcef(sourceID, AL_PITCH, 1.0f);
	alSourcef(sourceID, AL_GAIN, 1.0f);
	alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
	alSource3f(sourceID, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSourcei(sourceID, AL_LOOPING, AL_FALSE);
	return (int)sourceID;
}


void AudioSource::remove(unsigned int sourceID) {
	//Sources must be removed as there can only be a max of 32 being used at once.
	stopAudio(sourceID);
	alDeleteSources(1, &sourceID);
}

void AudioSource::playAudio(int sourceID, int bufferID) {
	//Plays the audio buffer bound to the source.
	stopAudio(sourceID);
	if (bufferID >= 0) {
		alGetError();
		alSourcei(sourceID, AL_BUFFER, bufferID);

		int error = alGetError();
		if (error != AL_NO_ERROR) {
			std::cout << "Error loading audio" << error << std::endl;
		}
		else {

			alSourcePlay(sourceID);
		}
	}
	else {
		//Occurs if no buffer is bound (-1).
		std::cout << "Invalid buffer." << std::endl;
	}


}

int AudioSource::checkState(int sourceID) {
	//Returns the state of the source. ie, (Playing, Paused, Stopped).
	ALint source_state;
	alGetSourcei(sourceID, AL_SOURCE_STATE, &source_state);
	return source_state;
}

void AudioSource::pauseAudio(int sourceID) {
	alSourcePause(sourceID);
}

void AudioSource::continueAudio(int sourceID) {
	alSourcePlay(sourceID);
}

void AudioSource::stopAudio(int sourceID) {
	alGetError();
	if (checkState(sourceID) == AL_PLAYING) {
		alSourceStop(sourceID);

		int error = alGetError();
		if (error != AL_NO_ERROR) {
			std::cout << "An error has occured trying to stop the audio. " << error << std::endl;
		}
	}

}

void AudioSource::setVelocity(int sourceID, float x, float y, float z) {
	alSource3f(sourceID, AL_VELOCITY, x, y, z);
}

void AudioSource::setPos(int sourceID, float x, float y, float z) {
	alSource3f(sourceID, AL_POSITION, x, y, z);
}

void AudioSource::setPitch(int sourceID, float pitch) {
	alSourcef(sourceID, AL_PITCH, pitch);
}

void AudioSource::setGain(int sourceID, float gain) {
	alSourcef(sourceID, AL_PITCH, gain);
}

void AudioSource::setLooping(int sourceID, bool doLoop) {
	if (doLoop) {
		alSourcei(sourceID, AL_LOOPING, AL_TRUE);
	}
	else {
		alSourcei(sourceID, AL_LOOPING, AL_FALSE);
	}
}

void AudioSource::setRolloff(int sourceID, float factor) {
	alSourcef(sourceID, AL_ROLLOFF_FACTOR, factor);
}

void AudioSource::setRefDistance(int sourceID, float factor) {
	alSourcef(sourceID, AL_REFERENCE_DISTANCE, factor);
}

void AudioSource::setMaxDistance(int sourceID, float factor) {
	alSourcef(sourceID, AL_MAX_DISTANCE, factor);
}




//A global variable, storing all audio buffers. (The only global variable in the code).
std::map<std::string, int> AudioStream::_audio_buffers_;


void AudioStream::Clean() {
	//Deletes all audio buffers.
	for (std::pair<std::string, int> af : _audio_buffers_) {
		alDeleteBuffers(1, (ALuint*)af.second);
	}
	_audio_buffers_.clear();
}

int AudioStream::GetBuffer(std::string name) {
	//Returns an audio buffer based on its string id in the dictionary (map).
	if (_audio_buffers_.find(name) != _audio_buffers_.end()) {
		return (int)_audio_buffers_.at(name);
	}
	return -1;
}

void AudioStream::AddBuffer(std::string name, int buffer) {
	//Pushes a new buffer to the audio buffer map.
	_audio_buffers_.insert(std::pair<std::string, int>(name, buffer));
}