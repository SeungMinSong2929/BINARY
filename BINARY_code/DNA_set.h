#ifndef DNASET_H
#define DNASET_H 1

#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<string>
#include<random>
#include<fstream>
#include<ctime>
#include<time.h>

using namespace std;

// reference DNA 관련 상수
#define PL 11		// 패턴 길이
#define PN 4194304	// 패턴의 개수
#define RL 61		// random 길이
#define RN			// random 개수
#define DL 10080	// DNA 길이 100800

// short read 관련 상수
#define ReadMin (PL+RL)		// 최소 read 길이
#define ReadMax 80			// 최대 read 길이
#define ReadN ((DL/ReadMin)*2)	// read 개수: (DNA길이/최소 read 개수)

// mismatch 관련 상수
#define mismatch (RL/10)	// mismatch 허용 개수: (ref DNA 생성 노이즈 /10)

struct dna {
	char gene;
	char check; // 방문 여부 T or F
};

extern struct dna ref[DL];
extern char pattern[PN][PL];
extern int pcnt; //패턴 생성
extern char shortreads[ReadMax]; // read 만들 때, 파일에 저장할 read 1개 배열
extern char my[DL]; // myDNA 파일에서 읽어서 저장

void reference(string filename); // reference 생성
void MYDNA(string filename);//dna 생성
void MakePattern(); // 패턴 생성 함수 호출
void PrintPermuWithRepet(char*arr, int C);// 패턴을 배열에 삽입
void swap(char*num1, char*num2); //swap 함수
void PermuWithRepet(char*set, char*arr, int setsize, int arrsize, int C, int i);// 중복순열 함수
void similarity(string MY, string REF); // 유사도 측정 
void make_SR(string filename); // short read 자르기

void getRF(struct dna *Ref); // reference.txt 파일에서 reference 얻기

void trivial(struct dna *ref, string filename);
//void KMP(struct dna *ref, string filename);
#endif
