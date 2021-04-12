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

// reference DNA ���� ���
#define PL 11		// ���� ����
#define PN 4194304	// ������ ����
#define RL 61		// random ����
#define RN			// random ����
#define DL 10080	// DNA ���� 100800

// short read ���� ���
#define ReadMin (PL+RL)		// �ּ� read ����
#define ReadMax 80			// �ִ� read ����
#define ReadN ((DL/ReadMin)*2)	// read ����: (DNA����/�ּ� read ����)

// mismatch ���� ���
#define mismatch (RL/10)	// mismatch ��� ����: (ref DNA ���� ������ /10)

struct dna {
	char gene;
	char check; // �湮 ���� T or F
};

extern struct dna ref[DL];
extern char pattern[PN][PL];
extern int pcnt; //���� ����
extern char shortreads[ReadMax]; // read ���� ��, ���Ͽ� ������ read 1�� �迭
extern char my[DL]; // myDNA ���Ͽ��� �о ����

void reference(string filename); // reference ����
void MYDNA(string filename);//dna ����
void MakePattern(); // ���� ���� �Լ� ȣ��
void PrintPermuWithRepet(char*arr, int C);// ������ �迭�� ����
void swap(char*num1, char*num2); //swap �Լ�
void PermuWithRepet(char*set, char*arr, int setsize, int arrsize, int C, int i);// �ߺ����� �Լ�
void similarity(string MY, string REF); // ���絵 ���� 
void make_SR(string filename); // short read �ڸ���

void getRF(struct dna *Ref); // reference.txt ���Ͽ��� reference ���

void trivial(struct dna *ref, string filename);
//void KMP(struct dna *ref, string filename);
#endif
