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
unsigned long int CHANCE_DEL_RECT = RAND_MAX;
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
	float* costs = new float[numPopulation];
	float totalCost;
	float maxCost;
	float minCost;
	for(int g=0;g<gens;g++){
		maxCost=-1;
		minCost=2;
		totalCost=0;
		for(unsigned int f=0;f<numPopulation;f++){
			if(population[f].cost==-1)
				population[f].cost=costFunction(&population[f]);
			if(population[f].cost>maxCost){maxCost=population[f].cost;}
			if(population[f].cost<minCost){minCost=population[f].cost;}
		}

		for(unsigned int f=0;f<numPopulation;f++){
			costs[f]=population[f].cost-minCost;
			totalCost+=costs[f];
		}

		int delId;
		if(totalCost==0){delId=0;}
		else{
			float del = ((float)rand()/(RAND_MAX))*totalCost;
			float aggCost = 0;
			for(unsigned int f=0;f<numPopulation;f++){
				aggCost += costs[f];
				if(aggCost>=del){delId=f; del=totalCost+1;}
			}
		}

		float minNewCost = 2;
		int minNewf;
		FUNC* popNew = new FUNC[numPopulation];
		for(unsigned int f=0;f<numPopulation;f++){
			popNew[f]=offspring(&population[f]);
			popNew[f].cost=costFunction(&popNew[f]);
			if(popNew[f].cost<minNewCost){minNewCost=popNew[f].cost; minNewf=f;}
		}
		population[delId] = popNew[minNewf];
	}
}

void Evolve::setPopulation(int size, std::string word){
	population = new FUNC[size];
	numPopulation=size;
	for(int p=0;p<size;p++){
		population[p].wordLen=word.length();
		population[p].word=new char[population[p].wordLen+1];
		for(int i=0;i<population[p].wordLen;i++){
			population[p].word[i]=word.at(i);
		}
		population[p].word[population[p].wordLen]='\0';
		population[p].numRects=1;
		population[p].rects=new RECT[1];
		population[p].rects[0].x1=201;
		population[p].rects[0].x2=400;
		population[p].rects[0].y1=-0x1FFF;
		population[p].rects[0].y2=0x1FFF;
		population[p].cost=-1;
	}
}

FUNC Evolve::offspring(FUNC* f){
	FUNC n;
	n.wordLen=f->wordLen;
	n.cost=-1;
	n.word=new char[n.wordLen+1];
	for(int i=0;i<n.wordLen;i++){
		n.word[i]=f->word[i];
	}
	n.word[n.wordLen]='\0';

	n.numRects=f->numRects;
	int remID = -1;
	unsigned int posId = 0;
	if(rand()%CHANCE_NEW_RECT==0){
		int newID = rand()%n.numRects;
		n.numRects++;
		n.rects = new RECT[n.numRects];
		if(rand()%2==0){
			n.rects[n.numRects-1].x1=rand()%0xFFFF;
			n.rects[n.numRects-1].y1=rand()%0xFFFF;
			if(rand()%2==0){
				n.rects[n.numRects-1].x2=rand()%0xFFFF;
				n.rects[n.numRects-1].y2=rand()%0xFFFF;
			}
			else{
				n.rects[n.numRects-1].x2=n.rects[n.numRects-1].x1+rand()%0xFF;
				n.rects[n.numRects-1].y2=n.rects[n.numRects-1].y2+rand()%0xFF;
			}
		}
		else{
			n.rects[n.numRects-1].x1=f->rects[newID].x1;
			n.rects[n.numRects-1].x2=f->rects[newID].x2;
			n.rects[n.numRects-1].y1=f->rects[newID].y1;
			n.rects[n.numRects-1].y2=f->rects[newID].y2;
		}
	}
	else if(n.numRects>1&&rand()%CHANCE_DEL_RECT==0){
		remID = rand()%n.numRects;
		n.numRects--;
		n.rects = new RECT[n.numRects+1];
	}
	else{
		n.rects = new RECT[n.numRects];
	}
	for(unsigned int r=0;r<f->numRects;r++){
		if((signed)r==remID) continue;
		for(int i=0;i<4;i++){
			switch(i){
			case 0:
				n.rects[posId].x1=f->rects[r].x1; break;
			case 1:
				n.rects[posId].x2=f->rects[r].x2; break;
			case 2:
				n.rects[posId].y1=f->rects[r].y1; break;
			case 3:
				n.rects[posId].y2=f->rects[r].y2; break;
			}
			if(rand()%CHANCE_RECT_CHANGE==0){
				if(rand()%2==0){
					switch(i){
					case 0:
						n.rects[posId].x1=rand()%0xFFFF; break;
					case 1:
						n.rects[posId].x2=rand()%0xFFFF; break;
					case 2:
						n.rects[posId].y1=rand()%0xFFFF; break;
					case 3:
						n.rects[posId].y2=rand()%0xFFFF; break;
					}
				}
				else{switch(i){
				case 0:
					n.rects[posId].x1+=0xFF/2-rand()%0xFF; break;
				case 1:
					n.rects[posId].x2+=0xFF/2-rand()%0xFF; break;
				case 2:
					n.rects[posId].y1+=0xFF/2-rand()%0xFF; break;
				case 3:
					n.rects[posId].y2+=0xFF/2-rand()%0xFF; break;
				}
				}
			}
		}
		posId++;
	}
	return n;
}

FUNC* Evolve::bestFunc(){
	float minCost=2;
	int minF;
	for(unsigned int f=0;f<numPopulation;f++){
		population[f].cost=costFunction(&population[f]);
		if(population[f].cost<minCost){minCost=population[f].cost; minF=f;}
	}
	return &population[minF];
}

float Evolve::costFunction(FUNC* a){
	float ret = 0;
	for(unsigned int i=0;i<numData;i++){
		ret += std::abs(matchFunc(a,&sounds[i])-(words[i].compare(a->word)==0?1:0))/numData;
	}
	return ret;
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

float Evolve::matchFunc(FUNC* func, Wave* sound){
	float totalMatch = 0;
	for(unsigned int r=0;r<func->numRects;r++){
		if(func->rects[r].x1>=func->rects[r].x2){
			short int mid = func->rects[r].x2;
			func->rects[r].x2=func->rects[r].x1;
			func->rects[r].x1=mid;
		}
		if(func->rects[r].y1>=func->rects[r].y2){
			short int mid = func->rects[r].y2;
			func->rects[r].y2=func->rects[r].y1;
			func->rects[r].y1=mid;
		}

		totalMatch += matchRect(&(func->rects[r]),sound)/func->numRects;
	}
	return totalMatch;
}

float Evolve::matchRect(RECT* r, Wave* sound){
	float totalMatch = 0;
	for(int x=r->x1;x<r->x2;x++){
		if(x<0) continue;
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
		population[o].cost=-1;
		long int rects = 0;
		for(int i=0;i<4;i++){
			rects=rects<<8;
			rects+=data[pos++];
		}
		population[o].numRects=rects;
		population[o].rects=new RECT[rects];
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
					population[o].rects[r].x1=valu; break;
				case 1:
					population[o].rects[r].x2=valu; break;
				case 2:
					population[o].rects[r].y1=valu; break;
				case 3:
					population[o].rects[r].y2=valu; break;
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
		len+=4;
		len+=population[p].wordLen;
		len+=4+8*population[p].numRects;
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
			data[pos++]=(population[p].numRects>>(24-i*8))&0xFF;
		}
		for(unsigned int r=0;r<population[p].numRects;r++){
			for(int d=0;d<4;d++){
				signed short int val;
				unsigned short int valu;
				switch(d){
				case 0:
					val = population[p].rects[r].x1; break;
				case 1:
					val = population[p].rects[r].x2; break;
				case 2:
					val = population[p].rects[r].y1; break;
				case 3:
					val = population[p].rects[r].y2; break;
				}
				valu = twoComplement(val);
				for(int i=0;i<2;i++){
					data[pos++]=(valu>>(8-i*8))&0xFF;
				}
			}
		}
	}

	std::ofstream out(filename.c_str(), std::ios::binary | std::ios::out);
	char* dat = (char*)data;
	out.write(dat,len);
	out.close();
}

void Evolve::writeFileBestAlg(std::string filename){
	if(population == NULL) return;
	FUNC* f = bestFunc();

	unsigned int len = 0;
	len+=8;
	len+=f->wordLen;
	len+=4+8*f->numRects;

	unsigned char* data = new unsigned char[len];

	int pos = 0;
	for(int i=0;i<4;i++){
		data[pos++]=(1>>(24-i*8))&0xFF;
	}
	int wlen = f->wordLen;
	for(int i=0;i<4;i++){
		data[pos++]=(wlen>>(24-i*8))&0xFF;
	}
	for(int i=0;i<wlen;i++){
		data[pos++]=f->word[i];
	}
	for(int i=0;i<4;i++){
		data[pos++]=(f->numRects>>(24-i*8))&0xFF;
	}
	for(unsigned int r=0;r<f->numRects;r++){
		for(int d=0;d<4;d++){
			signed short int val;
			unsigned short int valu;
			switch(d){
			case 0:
				val = f->rects[r].x1; break;
			case 1:
				val = f->rects[r].x2; break;
			case 2:
				val = f->rects[r].y1; break;
			case 3:
				val = f->rects[r].y2; break;
			}
			valu = twoComplement(val);
			for(int i=0;i<2;i++){
				data[pos++]=(valu>>(8-i*8))&0xFF;
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
