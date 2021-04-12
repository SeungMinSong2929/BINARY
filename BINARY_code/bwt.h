#ifndef BWT_H
#define BWT_H 1

#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<string>
#include<random>
#include<fstream>
#include<ctime>
#include<time.h>


using namespace std;

struct bwt_string {
	string dna_st;
	char first_char;
	int last_index;//마지막 문자의 원래 T에서의 
};

extern struct bwt_string * bwt_sub;// fill the rest에서 사용할 pre suffix string
void find(int i, int &first, int A_cnt, int C_cnt, int G_cnt);
void swap(struct bwt_string *a, struct bwt_string *b);
void make_heap_tree(bwt_string *bwt_sub, int root, int lastnode);
void heap_sort(bwt_string *bwt_sub, int length);
int compare_text(string parent, string child);
void BWT(string filename);//BWT복원
void trivial(struct dna *ref, string filename);
#endif