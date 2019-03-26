#include "AudioEngine.h"



AudioEngine::AudioEngine()
{
	ALboolean canEnumerate;
	canEnumerate = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (canEnumerate == AL_FALSE) {
		std::cout << "can't enumerate" << std::endl;
		//Default device is used.
	}
	else {
		//list audio devices.
	}
	
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
	Object o = scene.getObject(scene.getCamera(true).componentTransform.objID);

	setListenerPos(o.pos.x, o.pos.y, o.pos.z);
	glm::vec3 forward = o.GetForward();
	ALfloat listenerOri[] = { forward.x,forward.y,forward.z, 0.0f, 1.0f, 0.0f };
	alListenerfv(AL_ORIENTATION, listenerOri);

	//for (std::pair<std::string, ALuint> audioSource : audioSources) {
	//	if (AudioSource::checkState(audioSource.second) != AL_PLAYING) {
	//		audioSources.erase(audioSource.first);
	//		break;
	//	}
	//}
}



void AudioEngine::clean() {
	AudioStream::Clean();

	for (std::pair<std::string, ALuint> as : audioSources) {
		AudioSource::remove(as.second);
	}
	audioSources.clear();

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