#pragma once
#include <al.h>
#include <alc.h>
#include <fstream>
#include <iostream>

inline namespace AudioFile
{

	struct RIFF_Header {
		char chunkID[4];
		long chunkSize;
		char format[4];
	};

	struct WAVE_Format {
		char subChunkID[4];
		long subChunkSize;
		short audioFormat;
		short numChannels;
		long sampleRate;
		long byteRate;
		short blockAlign;
		short bitsPerSample;
	};

	struct WAVE_Data {
		char subChunkID[4];
		long subChunk2Size;
	};


	unsigned int generate(std::string filepath);
	unsigned int loadWAV(const char * filename);
};

