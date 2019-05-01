#include "AudioEngine.h"



AudioEngine::AudioEngine()
{
	///Setting up OpenAL.
	ALboolean canEnumerate;
	canEnumerate = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (canEnumerate == AL_FALSE) {
		std::cout << "can't enumerate" << std::endl;
		//Default device is used.
	}
	else {
		//list audio devices.
	}
	
	//Creating a io stream to the device.
	device = alcOpenDevice(NULL);
	if (!device) {
		std::cout << "ERROR: Can't connect to device (AUDIOENGINE)" << std::endl;
	}


	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		std::cout << "ERROR: Can't create context (AUDIOENGINE)" << std::endl;
	}

	//Setting basic values for openal to initialise.
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, listenerOri);

	AudioStream::AddBuffer("test", (ALuint)AudioFile::generate("src\\sounds\\test.wav"));
	AudioStream::AddBuffer("car", (ALuint)AudioFile::generate("src\\sounds\\boom.wav"));

}

AudioEngine::~AudioEngine()
{
	clean();
}


unsigned int AudioEngine::newSource(std::string name) {
	try {
		audioSources.insert(std::pair<std::string, ALuint>(name, (ALuint)AudioSource::generate()));
	}
	catch (std::exception ex) {

	}
	if (audioSources.size() > 0) {
		return audioSources.size() - 1;
	}
	return 0;
}

void AudioEngine::update(Scene &scene) {
	//Fetching the user's position.
	Object o = scene.getObject(scene.getCamera(true).componentTransform.objID);
	//Updating OpenAL to know where the player is in 3D space.
	setListenerPos(o.pos.x, o.pos.y, o.pos.z);
	//Update direction of player.
	glm::vec3 forward = o.GetForward();
	ALfloat listenerOri[] = { forward.x,forward.y,forward.z, 0.0f, 1.0f, 0.0f };
	alListenerfv(AL_ORIENTATION, listenerOri);
}



void AudioEngine::clean() {
	//Removing all the audio buffers and sources.
	AudioStream::Clean();

	for (std::pair<std::string, ALuint> as : audioSources) {
		AudioSource::remove(as.second);
	}
	audioSources.clear();
	//Closing the OpenAL context.
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void AudioEngine::setListenerPos(float x, float y, float z) {
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
}

void AudioEngine::setListenerVelocity(float x, float y, float z) {
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
}