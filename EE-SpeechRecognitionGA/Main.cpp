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

	srand(time(0));

	Evolve e;
	e.setWaves(waves);
	e.setWords(&words[0]);
	e.setDataSize(len);
	e.readFile("popGen40000");
	e.evolvePop(10000);
	cout << e.testFunc(e.bestFunc());
	e.writeFile("popGen50000");

	return 0;
}
