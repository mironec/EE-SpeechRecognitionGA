#ifndef WAVE_H_
#define WAVE_H_

#include <fstream>
#include <cmath>
#include <cstdlib>

struct sample_mono16bit{
	signed short int value;
};

class Wave {
	unsigned long int samplesNumber;	//number of samples, should be subchunk2Length/blockAlign

	char chunkID[5];					//should be "RIFF"
	unsigned long int chunkLength;		//should be (32+dataChunkLength) or (fileSize-8) LE
	char chunkFormat[5];				//should be "WAVE"
	char subchunk1ID[5];				//should be "fmt "
	unsigned long int subchunk1Length;	//should be 16 LE
	unsigned short int audioFormat;		//should be 1 LE
	unsigned short int numOfChannels;	//should be 2 LE for stereo (1 LE for mono)
	unsigned long int sampleRate;		//samples per second in LE (44100Hz, mainly)
	unsigned long int byteRate;			//bytes per second in LE (sampleRate*numOfChannels*bitsPerSample/8)
	unsigned short int blockAlign;		//bytes per block in LE (numOfChannels*bitsPerSample/8)
	unsigned short int bitsPerSample;	//bits per sample in LE

	char subchunk2ID[5];				//should be "data"
	unsigned long int subchunk2Length;	//should be fileSize-44 LE, the length of the data in bytes (also numOfSamples*numOfChannels*bitsPerSample/8)
	unsigned char* data;				//actual data for the samples
	sample_mono16bit* dataUseful;		//used in this program
public:
	Wave();
	unsigned long int getSamplesNumber();
	short signed int* getData();
	void readFile(std::string name);
	void writeFileMono16Bit(std::string name);
	void convertToMono16Bit();
	void normalizeAmplitude();
	signed short int val(int x);
};

#endif
