#ifndef BOYERMOORE_H
#define BOYERMOORE_H 1

#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<random>
#include<fstream>
#include<ctime>

#include"DNA_set.h"

using namespace std;
#define NUM_OF_CHARS 256 //1����Ʈ���� ���� �� �ִ� ���� ����(�� ������ ���� ��ġ)
int* createBC(char* pat, int M);
void BoyerMoore(int SR_num, int l);


#endif