#include "AudioFile.h"



unsigned int AudioFile::generate(std::string filepath) {
	int bufferID = -1;
	if (filepath.find(".wav")) {
		bufferID = loadWAV(filepath.c_str());
	}
	return bufferID;
}


unsigned int AudioFile::loadWAV(const char* filename) {
	ALuint buffer;
	ALenum format;                          // The sound data format
	ALsizei freq;                           // The frequency of the sound data
	ALsizei size;                           // Data size


	FILE* soundFile = NULL;
	WAVE_Format wave_format;
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	unsigned char* data;
	try {
		fopen_s(&soundFile, filename, "rb");
		if (!soundFile) {
			std::cout << "Could not load sound file!" << std::endl;
			return -1;
		}

		std::fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

		if ((riff_header.chunkID[0] != 'R' ||
			riff_header.chunkID[1] != 'I' ||
			riff_header.chunkID[2] != 'F' ||
			riff_header.chunkID[3] != 'F') &&
			(riff_header.format[0] != 'W' ||
				riff_header.format[1] != 'A' ||
				riff_header.format[2] != 'V' ||
				riff_header.format[3] != 'E'))
			throw ("Invalid RIFF or WAVE Header");

		std::fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);

		if (wave_format.subChunkID[0] != 'f' ||
			wave_format.subChunkID[1] != 'm' ||
			wave_format.subChunkID[2] != 't' ||
			wave_format.subChunkID[3] != ' ')
			throw ("Invalid Wave Format");

		if (wave_format.subChunkSize > 16)
			fseek(soundFile, sizeof(short), SEEK_CUR);

		std::fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);

		if (wave_data.subChunkID[0] != 'd' ||
			wave_data.subChunkID[1] != 'a' ||
			wave_data.subChunkID[2] != 't' ||
			wave_data.subChunkID[3] != 'a')
			throw ("Invalid data header");

		data = new unsigned char[wave_data.subChunk2Size];

		if (!std::fread(data, wave_data.subChunk2Size, 1, soundFile))
			throw ("error loading WAVE data into struct!");

		size = wave_data.subChunk2Size;
		freq = wave_format.sampleRate;

		if (wave_format.numChannels == 1) {
			if (wave_format.bitsPerSample == 8)
				format = AL_FORMAT_MONO8;
			else if (wave_format.bitsPerSample == 16)
				format = AL_FORMAT_MONO16;
		}
		else if (wave_format.numChannels == 2) {
			if (wave_format.bitsPerSample == 8)
				format = AL_FORMAT_STEREO8;
			else if (wave_format.bitsPerSample == 16)
				format = AL_FORMAT_STEREO16;
		}

		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, (void*)data, size, freq);	

		ALCenum error;
		error = alGetError();
		if (error != AL_NO_ERROR) {
			std::cout << "Could not read sound, " << error << std::endl;
			std::fclose(soundFile);
			return -1;
		}
		else {
			std::cout << "Successfully read sound" << std::endl;
		}
		


		std::fclose(soundFile);
		return buffer;
	}
	catch (char* error) {
		if (soundFile != NULL)
			std::fclose(soundFile);
		return -1;
	}
}
