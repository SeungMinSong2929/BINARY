#include "DNA_set.h"
#include "bwt.h"
#include "denovo.h"
#include "kmp.h"
#include "boyermoore.h"

struct dna ref[DL];
struct dna bwt_ref[DL + 1];
char pattern[PN][PL];
int pcnt = 0; //패턴 생성
char shortreads[ReadMax]; // read 만들 때, 파일에 저장할 read 1개 배열
char my[DL]; // myDNA 파일에서 읽어서 저장

struct SHORT AllSR[ReadN]; // 모든 short read 정보를 저장할 배열
bool first[ReadN] = { true }; // first를 찾아내는 배열


int main() {
	clock_t begin, end;
	cout << "reference.txt 파일에 ref 저장" << endl;
	reference("reference.txt");
	cout << "myDNA.txt 파일에 myDNA 저장" << endl;
	MYDNA("myDNA.txt");
	cout << "myDNA.txt 파일에서 myDNA 읽어와서 SR.txt에 저장" << endl;
	make_SR("myDNA.txt");

	// TRIVIAL
	//cout << "SR.txt 파일의 shortread로 trivial 복구" << endl;
	//begin = clock(); //시간측정 시작
	//trivial(::ref, "SR.txt");
	//end = clock(); //시간측정 끝
	//cout << "trivial 수행시간: " << ((double)(end - begin) / CLOCKS_PER_SEC) << endl <<endl;
	//cout << "원본 myDNA.txt와 복원된 trivialMyDNA.txt 비교하면서 일치율 검사." << endl;
	//similarity("myDNA.txt", "trivialMyDNA.txt");

	// KMP
	//cout << "SR.txt 파일의 shortread로 KMP 복구" << endl;
	//begin = clock(); //시간측정 시작
	//KMP(::ref, "SR.txt");
	//end = clock(); //시간측정 끝
	//cout << "KMP 수행시간: " << ((double)(end - begin) / CLOCKS_PER_SEC) << endl << endl;
	//cout << "원본 myDNA.txt와 복원된 kmpMyDNA.txt 비교하면서 일치율 검사." << endl;
	//similarity("myDNA.txt", "kmpMyDNA.txt");

	// DENOVO
	cout << "de novo알고리즘으로 복원" << endl;
	begin = clock(); //시간측정 시작
	denovo();
	end = clock(); //시간측정 끝
	cout << "de novo 수행시간: " << ((double)(end - begin) / CLOCKS_PER_SEC) << endl;
	similarity("myDNA.txt", "denovoMyDNA.txt");

	// BOYERMOORE
	//cout << "SR.txt 파일의 shortread로 boyermoore 복구" << endl;
	//begin = clock(); //시간측정 시작
	//BoyerMoore(ReadN, ReadMax);
	//end = clock(); //시간측정 끝
	//similarity("myDNA.txt", "boyermooreMyDNA.txt");
	//cout << "BoyerMoore 수행시간: " << ((double)(end - begin) / CLOCKS_PER_SEC) << endl;
	//system("pause");

	// BWT
	BWT("SR.txt");

}