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
#define NUM_OF_CHARS 256 //1바이트에서 담을 수 있는 수의 개수(각 문자의 숫자 위치)
int* createBC(char* pat, int M);
void BoyerMoore(int SR_num, int l);


#endif