#ifndef EVOLVE_H_
#define EVOLVE_H_

#include <iostream>
#include <fstream>
#include "Wave.h"

struct RECT{
	signed short int x1;
	signed short int x2;
	signed short int y1;
	signed short int y2;
};

struct PATTERN{
	RECT* rects;
	unsigned long int numRects;
};

struct POSSIBLE_FUNC{
	RECT* rects;
	unsigned long int numRects;
};

struct FUNC{
	POSSIBLE_FUNC* possibleFuncs;
	RECT* identifiers;
	unsigned long int numPossible;
	char* word;
	int wordLen;
	float cost;
};

class Evolve{
	unsigned long int numPopulation;
	unsigned long int numData;

	FUNC* population;
	Wave* sounds;
	std::string* words;
public:
	Evolve();
	float costFunction(FUNC* a);
	void evolvePop(int gens);
	void setPopulation(int size, std::string word);
	void setWaves(Wave* sounds);
	void setWords(std::string* words);
	void setDataSize(unsigned long int numData);
	void setFuncWord(std::string word);
	FUNC* bestFunc();
	float testFunc(FUNC* a);
	float testFunc(FUNC* a, Wave* sound, std::string word);
	float matchPossibleFunc(POSSIBLE_FUNC* func, Wave* sound);
	FUNC offspring(FUNC* f);
	FUNC* pop(int x);
	float matchRect(RECT* r, Wave* sound);
	void readFile(std::string filename);
	void writeFile(std::string filename);
};

#endif
