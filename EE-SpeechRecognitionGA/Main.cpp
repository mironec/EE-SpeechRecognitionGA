#include <iostream>
#include <fstream>
#include "Wave.h"
using namespace std;

int main(int argc, char **argv) {
	Wave wave;
	wave.readFile("file.wav");

	wave.convertToMono16Bit();
	short int* data = wave.getData();
	for(unsigned int i=0;i<wave.getSamplesNumber();i+=100){
		cout << data[i] << " ";
		if(i%10000==0)
			cout << endl << i << ": ";
	}
	wave.normalizeAmplitude();
	delete[] data;
	data = wave.getData();
	cout << endl << endl << endl;
	for(unsigned int i=0;i<wave.getSamplesNumber();i+=100){
		cout << data[i] << " ";
		if(i%10000==0)
			cout << endl << i << ": ";
	}

	wave.writeFileMono16Bit("out.wav");

	return 0;
}
