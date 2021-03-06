#include "Wave.h"
using namespace std;

Wave::Wave(){
	samplesNumber = 0;
	chunkLength = -1;
	subchunk1Length = 0;
	audioFormat = 0;
	numOfChannels = 0;
	sampleRate = 0;
	byteRate = 0;
	blockAlign = 0;
	bitsPerSample = 0;

	subchunk2Length = 0;
	data = NULL;
	dataUseful = NULL;
}

unsigned long int Wave::getSamplesNumber(){
	return samplesNumber;
}

short int* Wave::getData(){
	short int* dat;
	dat = new short int[samplesNumber];
	for(unsigned int i=0;i<samplesNumber;i++){
		dat[i]=dataUseful[i].value;
	}
	return dat;
}

void Wave::readFile(string name){
	ifstream in (name.c_str(), ios::in | ios::binary);
	if(!in.good()) return;

	for(int i=0;i<4;i++){
		chunkID[i]=(char)in.get();				//big-endian
	}
	chunkID[4]='\0';
	chunkLength=0;
	for(int i=0;i<4;i++){
		chunkLength+=((in.get())<<(i*8));		//little-endian
	}
	for(int i=0;i<4;i++){
		chunkFormat[i]=(char)in.get();			//big-endian
	}
	chunkFormat[4]='\0';
	for(int i=0;i<4;i++){
		subchunk1ID[i]=(char)in.get();			//big-endian
	}
	subchunk1ID[4]='\0';
	subchunk1Length=0;
	for(int i=0;i<4;i++){
		subchunk1Length+=((in.get())<<(i*8));	//little-endian
	}
	audioFormat=0;
	for(int i=0;i<2;i++){
		audioFormat+=((in.get())<<(i*8));		//little-endian
	}
	numOfChannels=0;
	for(int i=0;i<2;i++){
		numOfChannels+=((in.get())<<(i*8));		//little-endian
	}
	sampleRate=0;
	for(int i=0;i<4;i++){
		sampleRate+=((in.get())<<(i*8));		//little-endian
	}
	byteRate=0;
	for(int i=0;i<4;i++){
		byteRate+=((in.get())<<(i*8));			//little-endian
	}
	blockAlign=0;
	for(int i=0;i<2;i++){
		blockAlign+=((in.get())<<(i*8));		//little-endian
	}
	bitsPerSample=0;
	for(int i=0;i<2;i++){
		bitsPerSample+=((in.get())<<(i*8));		//little-endian
	}
	for(int i=0;i<4;i++){
		subchunk2ID[i]=(char)in.get();			//big-endian
	}
	subchunk2ID[4]='\0';
	subchunk2Length=0;
	for(int i=0;i<4;i++){
		subchunk2Length+=((in.get())<<(i*8));	//little-endian
	}
	///////////////////////////////////////////////////////DATA///////////////////////////////////////////////////////////
	samplesNumber=subchunk2Length/blockAlign;
	data = (unsigned char*)malloc(samplesNumber*numOfChannels*bitsPerSample/8*sizeof(unsigned char));
	char* dataRaw = new char[subchunk2Length];
	in.read(dataRaw,subchunk2Length);
	data = (unsigned char*)dataRaw;

	in.close();
}

void Wave::convertToMono16Bit(){
	if(bitsPerSample!=16) return;
	if(numOfChannels!=1&&numOfChannels!=2) return;

	dataUseful = new sample_mono16bit[samplesNumber];
	for(unsigned int i=0;i<samplesNumber;i++){
		if(numOfChannels==2){
			unsigned char* sample = &data[i*blockAlign];
			long int val1 = sample[0]+(sample[1]<<8);		//left channel
			long int val2 = sample[2]+(sample[3]<<8);		//right channel
			if(val1>32767){									//negative, because two's complement
				val1 = val1-65536;
			}
			if(val2>32767){									//negative, because two's complement
				val2 = val2-65536;
			}
			dataUseful[i].value=(signed short int)(val1/2+val2/2);
		}
		else if(numOfChannels==1){
			unsigned char* sample = &data[i*blockAlign];
			long int val = sample[0]+(sample[1]<<8);		//little-endian
			if(val>32767){									//negative, because two's complement
				val = val-65536;
			}
			dataUseful[i].value=(signed short int)val;
		}
	}

	numOfChannels=1;
	blockAlign=numOfChannels*bitsPerSample/8;
	byteRate=numOfChannels*sampleRate*bitsPerSample/8;
	subchunk2Length=samplesNumber*numOfChannels*bitsPerSample/8;
	chunkLength=subchunk2Length+subchunk1Length+16;
}

void Wave::writeFileMono16Bit(string name){
	if(numOfChannels!=1) return;
	if(bitsPerSample!=16) return;
	ofstream out (name.c_str(), ios::out | ios::binary);
	if(!out.good()) return;

	for(int i=0;i<4;i++){
		out.put(chunkID[i]);					//big-endian
	}
	for(int i=0;i<4;i++){
		out.put(chunkLength>>(i*8)&0xFF);		//little-endian
	}
	for(int i=0;i<4;i++){
		out.put(chunkFormat[i]);				//big-endian
	}
	for(int i=0;i<4;i++){
		out.put(subchunk1ID[i]);				//big-endian
	}
	for(int i=0;i<4;i++){
		out.put(subchunk1Length>>(i*8)&0xFF);	//little-endian
	}
	for(int i=0;i<2;i++){
		out.put(audioFormat>>(i*8)&0xFF);		//little-endian
	}
	for(int i=0;i<2;i++){
		out.put(numOfChannels>>(i*8)&0xFF);		//little-endian
	}
	for(int i=0;i<4;i++){
		out.put(sampleRate>>(i*8)&0xFF);		//little-endian
	}
	for(int i=0;i<4;i++){
		out.put(byteRate>>(i*8)&0xFF);			//little-endian
	}
	for(int i=0;i<2;i++){
		out.put(blockAlign>>(i*8)&0xFF);		//little-endian
	}
	for(int i=0;i<2;i++){
		out.put(bitsPerSample>>(i*8)&0xFF);		//little-endian
	}
	for(int i=0;i<4;i++){
		out.put(subchunk2ID[i]);				//big-endian
	}
	for(int i=0;i<4;i++){
		out.put(subchunk2Length>>(i*8)&0xFF);	//little-endian
	}
	///////////////////////////////////////////////////////DATA///////////////////////////////////////////////////////////
	char* dat = new char[subchunk2Length];
	for(unsigned int i=0;i<samplesNumber;i++){
		short int val = dataUseful[i].value;
		/*unsigned short int valu;
		if(val<0)
			valu=val+0xFFFF;
		else
			valu=val;*/
		for(unsigned int x=0;x<blockAlign;x++){
			dat[i*blockAlign+x]=(val>>(x*8))&0xFF;
		}
	}
	out.write(dat,subchunk2Length);

	out.close();
}

void Wave::normalizeAmplitude(){
	unsigned short int max=0;
	unsigned long int totalMax = 2<<(bitsPerSample-2);
	for(unsigned int i=0;i<samplesNumber;i++){
		unsigned short int val = abs(dataUseful[i].value);
		if(val>max){max=val;}
	}
	for(unsigned int i=0;i<samplesNumber;i++){
		dataUseful[i].value=(signed short int)(
				(float)(dataUseful[i].value)*(totalMax/(float)max)
				);
	}
}

signed short int Wave::val(int x){
	if(x>=samplesNumber) return 0;
	return dataUseful[x].value;
}
