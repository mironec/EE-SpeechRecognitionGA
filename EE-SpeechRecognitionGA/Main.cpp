#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include "Wave.h"
#include "Evolve.h"
using namespace std;

Wave* waves;
vector<string> fileNames;
vector<string> words;
int len = 0;

int main(int argc, char **argv) {

	/*Wave * wave = new Wave[20];
	wave[0].readFile("Audio/Test/Jeden1.wav");
	wave[1].readFile("Audio/Test/Jeden2.wav");
	wave[2].readFile("Audio/Test/Dva1.wav");
	wave[3].readFile("Audio/Test/Dva2.wav");
	wave[4].readFile("Audio/Test/Tri1.wav");
	wave[5].readFile("Audio/Test/Tri2.wav");
	wave[6].readFile("Audio/Test/Styri1.wav");
	wave[7].readFile("Audio/Test/Styri2.wav");
	wave[8].readFile("Audio/Test/Pet1.wav");
	wave[9].readFile("Audio/Test/Pet2.wav");
	wave[10].readFile("Audio/Test/Sest1.wav");
	wave[11].readFile("Audio/Test/Sest2.wav");
	wave[12].readFile("Audio/Test/Sedem1.wav");
	wave[13].readFile("Audio/Test/Sedem2.wav");
	wave[14].readFile("Audio/Test/Osem1.wav");
	wave[15].readFile("Audio/Test/Osem2.wav");
	wave[16].readFile("Audio/Test/Devet1.wav");
	wave[17].readFile("Audio/Test/Devet2.wav");
	wave[18].readFile("Audio/Test/Desat1.wav");
	wave[19].readFile("Audio/Test/Desat2.wav");
	for(int i=0;i<20;i++){
		wave[i].convertToMono16Bit();
		wave[i].normalizeAmplitude();
	}
	wave[0].writeFileMono16Bit("Audio/Test/jedenN1.wav");
	wave[1].writeFileMono16Bit("Audio/Test/jedenN2.wav");
	wave[2].writeFileMono16Bit("Audio/Test/dvaN1.wav");
	wave[3].writeFileMono16Bit("Audio/Test/dvaN2.wav");
	wave[4].writeFileMono16Bit("Audio/Test/triN1.wav");
	wave[5].writeFileMono16Bit("Audio/Test/triN2.wav");
	wave[6].writeFileMono16Bit("Audio/Test/styriN1.wav");
	wave[7].writeFileMono16Bit("Audio/Test/styriN2.wav");
	wave[8].writeFileMono16Bit("Audio/Test/petN1.wav");
	wave[9].writeFileMono16Bit("Audio/Test/petN2.wav");
	wave[10].writeFileMono16Bit("Audio/Test/sestN1.wav");
	wave[11].writeFileMono16Bit("Audio/Test/sestN2.wav");
	wave[12].writeFileMono16Bit("Audio/Test/sedemN1.wav");
	wave[13].writeFileMono16Bit("Audio/Test/sedemN2.wav");
	wave[14].writeFileMono16Bit("Audio/Test/osemN1.wav");
	wave[15].writeFileMono16Bit("Audio/Test/osemN2.wav");
	wave[16].writeFileMono16Bit("Audio/Test/devetN1.wav");
	wave[17].writeFileMono16Bit("Audio/Test/devetN2.wav");
	wave[18].writeFileMono16Bit("Audio/Test/desatN1.wav");
	wave[19].writeFileMono16Bit("Audio/Test/desatN2.wav");*/

	string wordLowerCase;
	string wordNormalCase;
	string lowNum;
	string highNum;
	if(argc==5){
		wordLowerCase=argv[1];
		wordNormalCase=argv[2];
		lowNum=argv[3];
		highNum=argv[4];
	}
	else{
		wordLowerCase="desat";
		wordNormalCase="Desat";
		lowNum="5000";
		highNum="6000";
	}

	srand(time(0));

	ifstream config ("config.txt");

	while(config.good()){
		string s;
		getline(config,s);
		string fileName;
		string word;
		int pos = s.find(';');
		if(pos==-1){break;}
		fileName=s.substr(0,pos);
		word=s.substr(pos+1);
		if(word!=wordLowerCase&&rand()%4!=0) continue;
		fileNames.push_back(fileName);
		words.push_back(word);
		len++;
	}
	config.close();
	waves = new Wave[len];
	for(int i=0;i<len;i++){
		waves[i].readFile(fileNames[i]);
		waves[i].convertToMono16Bit();
	}

	Evolve e;
	e.setWaves(waves);
	e.setWords(&words[0]);
	e.setDataSize(len);
	string read = "Algo/"; read+=wordNormalCase; read+='/'; read+=lowNum; read+=".alg";
	e.readFile(read);
	FUNC* f = e.bestFunc();
	cout << e.costFunction(f) << endl;
	for(int i=0;i<100;i++){
		e.evolvePop(10);
		FUNC* f = e.bestFunc();
		cout << i << ": " << e.costFunction(f) << endl;
	}
	string write = "Algo/"; write+=wordNormalCase; write+='/'; write+=highNum; write+=".alg";
	e.writeFile(write);
	string writeBest = "Algo/"; writeBest+=wordNormalCase; writeBest+="/Best.alg";
	e.writeFileBestAlg(writeBest);

	/*Evolve* e = new Evolve[10];
	for(int i=0;i<10;i++){
		e[i].setWaves(waves);
		e[i].setWords(&words[0]);
		e[i].setDataSize(len);
	}
	e[0].readFile("Algo/Jeden/Best.alg");
	e[1].readFile("Algo/Dva/Best.alg");
	e[2].readFile("Algo/Tri/Best.alg");
	e[3].readFile("Algo/Styri/Best.alg");
	e[4].readFile("Algo/Pet/Best.alg");
	e[5].readFile("Algo/Sest/Best.alg");
	e[6].readFile("Algo/Sedem/Best.alg");
	e[7].readFile("Algo/Osem/Best.alg");
	e[8].readFile("Algo/Devet/Best.alg");
	e[9].readFile("Algo/Desat/Best.alg");

	ofstream file("out.txt");
	for(int k=0;k<len;k++){
	for(int i=0;i<10;i++){
		FUNC* f = e[i].pop(0);
		file << words[k] << " - " << (i+1) << ": " << e[i].testFunc(f,&waves[k]) << endl;
	}
	}
	file.close();*/

	return 0;
}
