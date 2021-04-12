#ifndef DENOVO_H
#define DENOVO_H 1

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<random>
#include<fstream>
#include<ctime>
#include<vector>

#include"DNA_set.h"

using namespace std;

struct SHORT {
	vector<char> gene;
	struct SHORT *back; // 뒤어 연결되는 SR 중 추가적인 부분이 가장 긴 SR
	int b; // back의 인덱스
};

extern struct SHORT AllSR[ReadN]; // 모든 short read 정보를 저장할 배열
extern bool first[ReadN]; // first를 찾아내는 배열 

void denovo();
void getSR(struct SHORT *AllSR);
struct SHORT *findConnect(struct SHORT *AllSR);
void DE_NOVO(struct SHORT *First);
#endif