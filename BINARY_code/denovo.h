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
	struct SHORT *back; // �ھ� ����Ǵ� SR �� �߰����� �κ��� ���� �� SR
	int b; // back�� �ε���
};

extern struct SHORT AllSR[ReadN]; // ��� short read ������ ������ �迭
extern bool first[ReadN]; // first�� ã�Ƴ��� �迭 

void denovo();
void getSR(struct SHORT *AllSR);
struct SHORT *findConnect(struct SHORT *AllSR);
void DE_NOVO(struct SHORT *First);
#endif