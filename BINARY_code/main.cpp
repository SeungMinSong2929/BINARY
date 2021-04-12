#include "DNA_set.h"
#include "bwt.h"
#include "denovo.h"
#include "kmp.h"
#include "boyermoore.h"

struct dna ref[DL];
struct dna bwt_ref[DL + 1];
char pattern[PN][PL];
int pcnt = 0; //���� ����
char shortreads[ReadMax]; // read ���� ��, ���Ͽ� ������ read 1�� �迭
char my[DL]; // myDNA ���Ͽ��� �о ����

struct SHORT AllSR[ReadN]; // ��� short read ������ ������ �迭
bool first[ReadN] = { true }; // first�� ã�Ƴ��� �迭


int main() {
	clock_t begin, end;
	cout << "reference.txt ���Ͽ� ref ����" << endl;
	reference("reference.txt");
	cout << "myDNA.txt ���Ͽ� myDNA ����" << endl;
	MYDNA("myDNA.txt");
	cout << "myDNA.txt ���Ͽ��� myDNA �о�ͼ� SR.txt�� ����" << endl;
	make_SR("myDNA.txt");

	// TRIVIAL
	//cout << "SR.txt ������ shortread�� trivial ����" << endl;
	//begin = clock(); //�ð����� ����
	//trivial(::ref, "SR.txt");
	//end = clock(); //�ð����� ��
	//cout << "trivial ����ð�: " << ((double)(end - begin) / CLOCKS_PER_SEC) << endl <<endl;
	//cout << "���� myDNA.txt�� ������ trivialMyDNA.txt ���ϸ鼭 ��ġ�� �˻�." << endl;
	//similarity("myDNA.txt", "trivialMyDNA.txt");

	// KMP
	//cout << "SR.txt ������ shortread�� KMP ����" << endl;
	//begin = clock(); //�ð����� ����
	//KMP(::ref, "SR.txt");
	//end = clock(); //�ð����� ��
	//cout << "KMP ����ð�: " << ((double)(end - begin) / CLOCKS_PER_SEC) << endl << endl;
	//cout << "���� myDNA.txt�� ������ kmpMyDNA.txt ���ϸ鼭 ��ġ�� �˻�." << endl;
	//similarity("myDNA.txt", "kmpMyDNA.txt");

	// DENOVO
	cout << "de novo�˰������� ����" << endl;
	begin = clock(); //�ð����� ����
	denovo();
	end = clock(); //�ð����� ��
	cout << "de novo ����ð�: " << ((double)(end - begin) / CLOCKS_PER_SEC) << endl;
	similarity("myDNA.txt", "denovoMyDNA.txt");

	// BOYERMOORE
	//cout << "SR.txt ������ shortread�� boyermoore ����" << endl;
	//begin = clock(); //�ð����� ����
	//BoyerMoore(ReadN, ReadMax);
	//end = clock(); //�ð����� ��
	//similarity("myDNA.txt", "boyermooreMyDNA.txt");
	//cout << "BoyerMoore ����ð�: " << ((double)(end - begin) / CLOCKS_PER_SEC) << endl;
	//system("pause");

	// BWT
	BWT("SR.txt");

}