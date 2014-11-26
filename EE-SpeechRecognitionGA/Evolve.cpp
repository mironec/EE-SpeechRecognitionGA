#include "Evolve.h"

signed short int twoComplement(unsigned short int x){
	if(x>32767)
		return (signed short int)(x-65536);
	else
		return (signed short int)(x);
}

unsigned short int twoComplement(signed short int x){
	if(x<0)
		return (unsigned short int)(x+65536);
	else
		return (unsigned short int)(x);
}

unsigned long int CHANCE_NEW_POSSIBLE_FUNC = 100;
unsigned long int CHANCE_DEL_POSSIBLE_FUNC = 100;
unsigned long int CHANCE_NEW_RECT = 25;
unsigned long int CHANCE_RECT_CHANGE = 25;
unsigned long int CHANCE_ID_CHANGE = 100;

Evolve::Evolve(){
	numData=0;
	numPopulation=0;
	population = NULL;
	words = NULL;
	sounds = NULL;
}

void Evolve::evolvePop(int gens){
	if(population == NULL) return;
	//float* costs = new float[numPopulation];
	float maxCost;
	float minCost;
	int maxF;
	int minF;
	for(int g=0;g<gens;g++){
		maxCost=-1;
		minCost=2;
		for(unsigned int f=0;f<numPopulation;f++){
			population[f].cost=costFunction(&population[f]);
			if(population[f].cost>maxCost){maxCost=population[f].cost; maxF=f;}
			else if(population[f].cost<minCost){minCost=population[f].cost; minF=f;}
		}

		population[maxF] = offspring(&population[minF]);
	}
}

void Evolve::setPopulation(int size, std::string word){
	population = new FUNC[size];
	numPopulation=size;
	for(int p=0;p<size;p++){
		population[p].wordLen=word.length();
		for(int i=0;i<population[p].wordLen;i++){
			population[p].word[i]=word.at(i);
		}
		population[p].numPossible=1;
		population[p].identifiers=new RECT[1];
		population[p].possibleFuncs=new POSSIBLE_FUNC[1];
		population[p].identifiers[0].x1=0;
		population[p].identifiers[0].x2=100;
		population[p].identifiers[0].y1=-0x1FFF;
		population[p].identifiers[0].y2=0x1FFF;
		population[p].possibleFuncs[0].numRects=1;
		population[p].possibleFuncs[0].rects=new RECT[1];
		population[p].possibleFuncs[0].rects[0].x1=201;
		population[p].possibleFuncs[0].rects[0].x2=400;
		population[p].possibleFuncs[0].rects[0].y1=-0x1FFF;
		population[p].possibleFuncs[0].rects[0].y2=0x1FFF;
	}
}

FUNC Evolve::offspring(FUNC* f){
	FUNC n;
	n.wordLen=f->wordLen;
	n.word=new char[n.wordLen+1];
	for(int i=0;i<n.wordLen;i++){
		n.word[i]=f->word[i];
	}
	n.word[n.wordLen]='\0';
	n.numPossible=f->numPossible;
	if(rand()%CHANCE_NEW_POSSIBLE_FUNC==0){

	}
	if(f->numPossible>1 && rand()%CHANCE_DEL_POSSIBLE_FUNC==0){

	}
	n.identifiers=new RECT[n.numPossible];
	n.possibleFuncs=new POSSIBLE_FUNC[n.numPossible];
	for(unsigned int p=0;p<f->numPossible;p++){
		for(int i=0;i<4;i++){
			switch(i){
			case 0:
				n.identifiers[p].x1=f->identifiers[p].x1; break;
			case 1:
				n.identifiers[p].x2=f->identifiers[p].x2; break;
			case 2:
				n.identifiers[p].y1=f->identifiers[p].y1; break;
			case 3:
				n.identifiers[p].y2=f->identifiers[p].y2; break;
			}
			if(rand()%CHANCE_ID_CHANGE==0){
				switch(i){
				case 0:
					n.identifiers[p].x1+=0xFF/2-rand()%0xFF; break;
				case 1:
					n.identifiers[p].x2+=0xFF/2-rand()%0xFF; break;
				case 2:
					n.identifiers[p].y1+=0xFF/2-rand()%0xFF; break;
				case 3:
					n.identifiers[p].y2+=0xFF/2-rand()%0xFF; break;
				}
			}
		}

		n.possibleFuncs[p].numRects=f->possibleFuncs[p].numRects;
		if(rand()%CHANCE_NEW_RECT==0){
			n.possibleFuncs[p].numRects++;
			n.possibleFuncs[p].rects = new RECT[n.possibleFuncs[p].numRects];
			n.possibleFuncs[p].rects[n.possibleFuncs[p].numRects-1].x1=f->possibleFuncs[p].rects[f->possibleFuncs[p].numRects-1].x1;
			n.possibleFuncs[p].rects[n.possibleFuncs[p].numRects-1].x2=f->possibleFuncs[p].rects[f->possibleFuncs[p].numRects-1].x2;
			n.possibleFuncs[p].rects[n.possibleFuncs[p].numRects-1].y1=f->possibleFuncs[p].rects[f->possibleFuncs[p].numRects-1].y1;
			n.possibleFuncs[p].rects[n.possibleFuncs[p].numRects-1].y2=f->possibleFuncs[p].rects[f->possibleFuncs[p].numRects-1].y2;
		}
		else{
			n.possibleFuncs[p].rects = new RECT[n.possibleFuncs[p].numRects];
		}
		for(unsigned int r=0;r<f->possibleFuncs[p].numRects;r++){
			for(int i=0;i<4;i++){
				switch(i){
				case 0:
					n.possibleFuncs[p].rects[r].x1=f->possibleFuncs[p].rects[r].x1; break;
				case 1:
					n.possibleFuncs[p].rects[r].x2=f->possibleFuncs[p].rects[r].x2; break;
				case 2:
					n.possibleFuncs[p].rects[r].y1=f->possibleFuncs[p].rects[r].y1; break;
				case 3:
					n.possibleFuncs[p].rects[r].y2=f->possibleFuncs[p].rects[r].y2; break;
				}
				if(rand()%CHANCE_RECT_CHANGE==0){
					switch(i){
					case 0:
						n.possibleFuncs[p].rects[r].x1+=0xFF/2-rand()%0xFF; break;
					case 1:
						n.possibleFuncs[p].rects[r].x2+=0xFF/2-rand()%0xFF; break;
					case 2:
						n.possibleFuncs[p].rects[r].y1+=0xFF/2-rand()%0xFF; break;
					case 3:
						n.possibleFuncs[p].rects[r].y2+=0xFF/2-rand()%0xFF; break;
					}
				}
			}
		}
	}
	return n;
}

FUNC* Evolve::bestFunc(){
	float minCost=2;
	int minF;
	for(int i=0;i<numPopulation;i++){
		for(unsigned int f=0;f<numPopulation;f++){
			population[f].cost=costFunction(&population[f]);
			if(population[f].cost<minCost){minCost=population[f].cost; minF=f;}
		}
	}
	return &population[minF];
}

float Evolve::costFunction(FUNC* a){
	return testFunc(a);
}

void Evolve::setWaves(Wave* sounds){
	this->sounds = sounds;
}

void Evolve::setWords(std::string* words){
	this->words = words;
}

void Evolve::setDataSize(unsigned long int numData){
	this->numData = numData;
}

float Evolve::testFunc(FUNC* a){
	float ret = 0;
	for(unsigned int i=0;i<numData;i++){
		ret += std::abs(testFunc(a,&sounds[i],words[i])-(words[i]==a->word?1:0))/numData;
	}
	return ret;
}

float Evolve::testFunc(FUNC* a, Wave* sound, std::string word){
	float matchMax = -1;
	int pMax = -1;
	for(unsigned int p=0;p<a->numPossible;p++){
		float match = matchRect(&(a->identifiers[pMax]),sound);
		if(match>matchMax){matchMax=match; pMax=p;}
	}
	return matchPossibleFunc(&(a->possibleFuncs[pMax]),sound);
}

float Evolve::matchPossibleFunc(POSSIBLE_FUNC* func, Wave* sound){
	float totalMatch = 0;
	for(unsigned int r=0;r<func->numRects;r++){
		totalMatch += matchRect(&(func->rects[r]),sound)/func->numRects;
	}
	return totalMatch;
}

float Evolve::matchRect(RECT* r, Wave* sound){
	float totalMatch = 0;
	for(int x=r->x1;x<r->x2;x++){
		if(sound->val(x) >= r->y1 && sound->val(x) <= r->y2){
			totalMatch+=1.0f/(r->x2-r->x1+1);
		}
	}
	return totalMatch;
}

void Evolve::readFile(std::string filename){
	if(population != NULL) return;

	std::ifstream in(filename.c_str(), std::ios::binary | std::ios::in);

	std::streampos begin,end;
	begin = in.tellg();
	in.seekg (0, std::ios::end);
	end = in.tellg();
	char* dat = new char[end-begin];
	in.seekg(0,std::ios::beg);
	in.read(dat,end-begin);

	in.close();

	unsigned char* data = (unsigned char*)dat;
	long long int pos = 0;
	long int pop = 0;
	for(int i=0;i<4;i++){
		pop=pop<<8;
		pop+=data[pos++];
	}
	numPopulation=pop;
	population = new FUNC[pop];
	for(int o=0;o<pop;o++){
		long int wordLen = 0;
		for(int i=0;i<4;i++){
			wordLen=wordLen<<8;
			wordLen+=data[pos++];
		}
		population[o].wordLen=wordLen;
		population[o].word=new char[wordLen+1];
		for(int i=0;i<wordLen;i++){
			population[o].word[i]=data[pos++];
		}
		population[o].word[wordLen]='\0';
		long int poss = 0;
		for(int i=0;i<4;i++){
			poss=poss<<8;
			poss+=data[pos++];
		}
		population[o].numPossible = poss;
		population[o].possibleFuncs = new POSSIBLE_FUNC[poss];
		population[o].identifiers = new RECT[poss];
		for(int p=0;p<poss;p++){
			for(int r=0;r<4;r++){
				unsigned short int val = 0;
				for(int i=0;i<2;i++){
					val = val << 8;
					val += data[pos++];
				}
				signed short int valu = twoComplement(val);
				switch(r){
				case 0:
					population[o].identifiers[p].x1=valu; break;
				case 1:
					population[o].identifiers[p].x2=valu; break;
				case 2:
					population[o].identifiers[p].y1=valu; break;
				case 3:
					population[o].identifiers[p].y2=valu; break;
				}
			}
			long int rects = 0;
			for(int i=0;i<4;i++){
				rects=rects<<8;
				rects+=data[pos++];
			}
			population[o].possibleFuncs[p].numRects=rects;
			population[o].possibleFuncs[p].rects=new RECT[rects];
			for(int r=0;r<rects;r++){
				for(int d=0;d<4;d++){
					unsigned short int val = 0;
					for(int i=0;i<2;i++){
						val = val << 8;
						val += data[pos++];
					}
					signed short int valu = twoComplement(val);
					switch(d){
					case 0:
						population[o].possibleFuncs[p].rects[r].x1=valu; break;
					case 1:
						population[o].possibleFuncs[p].rects[r].x2=valu; break;
					case 2:
						population[o].possibleFuncs[p].rects[r].y1=valu; break;
					case 3:
						population[o].possibleFuncs[p].rects[r].y2=valu; break;
					}
				}
			}
		}
	}
}

void Evolve::writeFile(std::string filename){
	if(population == NULL) return;

	unsigned int len = 0;
	len+=4;
	for(unsigned int p=0;p<numPopulation;p++){
		len+=8;
		len+=population[p].wordLen;
		for(unsigned int poss=0;poss<population[p].numPossible;poss++){
			len+=12+8*population[p].possibleFuncs[poss].numRects;
		}
	}

	unsigned char* data = new unsigned char[len];

	int pos = 0;
	for(int i=0;i<4;i++){
		data[pos++]=(numPopulation>>(24-i*8))&0xFF;
	}
	for(unsigned int p=0;p<numPopulation;p++){
		int wlen = population[p].wordLen;
		for(int i=0;i<4;i++){
			data[pos++]=(wlen>>(24-i*8))&0xFF;
		}
		for(int i=0;i<wlen;i++){
			data[pos++]=population[p].word[i];
		}
		for(int i=0;i<4;i++){
			data[pos++]=(population[p].numPossible>>(24-i*8))&0xFF;
		}
		for(unsigned int poss=0;poss<population[p].numPossible;poss++){
			for(int r=0;r<4;r++){
				signed short int val;
				unsigned short int valu;
				switch(r){
				case 0:
					val = population[p].identifiers[poss].x1; break;
				case 1:
					val = population[p].identifiers[poss].x2; break;
				case 2:
					val = population[p].identifiers[poss].y1; break;
				case 3:
					val = population[p].identifiers[poss].y2; break;
				}
				valu = twoComplement(val);
				for(int i=0;i<2;i++){
					data[pos++]=(valu>>(8-i*8))&0xFF;
				}
			}
			for(int i=0;i<4;i++){
				data[pos++]=(population[p].possibleFuncs[poss].numRects>>(24-i*8))&0xFF;
			}
			for(unsigned int r=0;r<population[p].possibleFuncs[poss].numRects;r++){
				for(int d=0;d<4;d++){
					signed short int val;
					unsigned short int valu;
					switch(d){
					case 0:
						val = population[p].possibleFuncs[poss].rects[r].x1; break;
					case 1:
						val = population[p].possibleFuncs[poss].rects[r].x2; break;
					case 2:
						val = population[p].possibleFuncs[poss].rects[r].y1; break;
					case 3:
						val = population[p].possibleFuncs[poss].rects[r].y2; break;
					}
					valu = twoComplement(val);
					for(int i=0;i<2;i++){
						data[pos++]=(valu>>(8-i*8))&0xFF;
					}
				}
			}
		}
	}

	std::ofstream out(filename.c_str(), std::ios::binary | std::ios::out);
	char* dat = (char*)data;
	out.write(dat,len);
	out.close();
}

FUNC* Evolve::pop(int x){
	return &population[x];
}
