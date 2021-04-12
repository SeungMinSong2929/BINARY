#include "DNA_set.h"

void MYDNA(string filename)
{
	char* buffer;
	int buf_size;
	string refer;
	int i;
	srand((unsigned int)time(NULL));
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 99);
	FILE* fp = fopen("reference.txt", "r"); //500000개의 랜덤 reference DNA txt파일
	if (fp == NULL)
	{
		cout << "파일 오픈 불가능" << endl;
		fclose(fp); return;
	}
	else
	{
		fseek(fp, 0, SEEK_END); //파일 포인터를 끝으로 이동시킴
		buf_size = ftell(fp); //파일포인터의 현재위치를 얻음
		buffer = (char*)malloc(buf_size + 1); //파일 크기만큼 동적메모리 할당
		memset(buffer, 0, buf_size + 1); //버퍼 메모리를 0으로 초기화
		fseek(fp, 0, SEEK_SET); //파일 포인터를 처음으로 이동시킴
		fread(buffer, buf_size, 1, fp); //버퍼사이즈만큼 buffer에 읽어옴(파일끝까지 읽어옴)
		refer = buffer;
		free(buffer);
		fclose(fp);
	}
	FILE* file2 = fopen("myDNA.txt", "wt"); //500000개의 myDNA txt파일 생성
	int k = 0;
	int num = 0;
	if (file2 != NULL)
	{
		i = 0;
		while (i < DL)
		{
			//앞에 패턴길이(PL=11)만큼은 그대로 입력
			for (int j = 0; j < PL; j++)
			{
				if (i == DL) break;
				fprintf(file2, "%c", refer[i]);
				//	cout << refer[i];
				i++;
			}
			if (i % 10000 == 0)
				cout << i << '\t';
			//랜덤길이(RL=61)일때
			while (k < RL)
			{
				if (i == DL) break;
				//지금까지 나온 스닙 수가 mismatch개이하 일때만 랜덤확률로 바뀜
				if ((num < mismatch) && (dis(gen) < 20)) // (SNP) 20%의 확률로 바뀜
				{
					num++;
					switch (refer[i])
					{
					case 'A':
						if (rand() % 10 < 3) fprintf(file2, "C"); //30%의 확률로 C로 바뀜
						else if (rand() % 10 < 3) fprintf(file2, "G"); //30%의 확률로 G로 바뀜
						else fprintf(file2, "T"); //나머지 확률로 T로 바뀜
						break;
					case 'C':
						if (rand() % 10 < 3) fprintf(file2, "A");
						else if (rand() % 10 < 3) fprintf(file2, "G");
						else fprintf(file2, "T");
						break;
					case 'G':
						if (rand() % 10 < 3) fprintf(file2, "A");
						else if (rand() % 10 < 3) fprintf(file2, "C");
						else fprintf(file2, "T");
						break;
					case 'T':
						if (rand() % 10 < 3) fprintf(file2, "A");
						else if (rand() % 10 < 3) fprintf(file2, "C");
						else fprintf(file2, "G");
						break;
					}
				}
				else //그대로 입력
				{
					fprintf(file2, "%c", refer[i]);
				}
				i++;
				k++;
			}
			num = 0;
			k = 0;
		}
	}
	fclose(file2);
}

void reference(string filename) {
	int i = 0, j = 0, tmp;
	char c;
	char stmp[RL];

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 3);
	// write하기 위해 파일 생성 및 open
	ofstream out(filename);

	// 패턴 생성
	MakePattern();

	cout << "pattern 생성 완료 " << endl;

	while (i < DL) {
		if (i % 10000 == 0)
			cout << i << endl;
		// 패턴 입력
		for (int t = 0; t < PL; t++)
			out << pattern[j][t];
		j++;

		// 랜덤 부분 생성
		for (int k = 0; k < RL; k++) {
			// random 부분 길이만큼 string 생성 후 삽입
			//tmp = rand() % 4;
			tmp = dis(gen);
			// a, c, g,t로 변환
			switch (tmp) {
			case 0: c = 'A'; break;
			case 1: c = 'C'; break;
			case 2: c = 'G'; break;
			case 3: c = 'T'; break;
			}
			if (k < 2) {
				stmp[k] = c;
			}
			// 연속해서 나오지 못하도록 체크
			else if (stmp[k - 1] != c || stmp[k - 2] != c) {
				stmp[k] = c;
			}
			// 연속해서 나오는 경우 다시 생성
			else {
				k--;
			}
		}

		// 랜덤 부분 입력
		for (int t = 0; t < RL; t++)
			out << stmp[t];

		//i=i+ 패턴 길이 + 랜덤 길이
		i = i + PL + RL;
	}
	cout << "파일 입력 완료" << endl;

	out.close();
}

void MakePattern() {
	char alphabet[4] = { 'A', 'C', 'G', 'T' };
	char arr[PL];

	PermuWithRepet(alphabet, arr, 4, PL, PL, 4);//arr_s 길이가 PL인 중복순열 생성 

	srand((unsigned int)time(NULL));
	//	int j;
		//	for (int i = 0; i < 1024; i++)
		//	{
				//cout << i;
				//for (int j = 0; j < 5; j++)
					//cout << pattern[i][j];
				//cout << endl;
		//	}
}
void make_SR(string filename) { // shortRead 잘라서 SR.txt 파일에 저장
	// A,C,G,T를 임시 저장할 변수
	char c;
	int i, j, k;
	int readCount; // shortRead가 N개 생성되었는지 확인

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(ReadMin, ReadMax); // 범위 확인하기
	uniform_int_distribution<int> overLap(ReadMin / 2, ReadMax / 2); // read 겹치게 하는 범위

	srand((unsigned int)time(0));

	ifstream in(filename);
	// MY DNA파일로 부터 dna정보를 받아오는 for문
	for (i = 0; in.get(c) && i < DL; i++) {
		my[i] = c;
	}
	in.get(c);

	i = 0; readCount = 0;

	k = dis(gen);
	string fileN = "SR.txt";

	ofstream outstm(fileN);
	if (outstm.is_open()) {

		for (i = DL; i > DL / 2; i -= k) { // 뒤에서부터 k만큼 앞으로 이동. 
			// i-k부터 i-1번째까지 k개의 문자열.
			for (j = (i - k); j < i; j++)
				outstm << my[j]; // 파일에 바로 shortread 문자 1개씩 쓰기
			outstm << '\n';

			i += overLap(gen);

			k = dis(gen);
			readCount++;
		}
		cout << readCount << endl;

		for (i = 0; i <= DL / 2; i += k) { // 앞에서부터 k만큼 뒤로 이동.
			// i부터 i+k-1번째까지 k개의 문자열.
			for (j = i; j < i + k; j++)
				outstm << my[j]; // 파일에 바로 shortread 문자 1개씩 쓰기
			outstm << '\n';

			i -= overLap(gen);

			k = dis(gen);
			readCount++;
		}
		cout << readCount << endl;
		i = DL / 4;
		while (readCount < ReadN) { // n개가 되도록 또다른 랜덤규칙으로 read마저 자르기
			// i부터 i+k-1번째까지 k개의 문자열.
			for (j = i; j < i + k; j++)
				outstm << my[j]; // 파일에 바로 shortread 문자 1개씩 쓰기
			outstm << '\n';

			i += overLap(gen);
			k = dis(gen);
			readCount++;
		}
	}
	if (readCount == ReadN)
		cout << " 모든 정보가 포함된 short read생성!!! 완료!!!!!!!!!1" << endl;

	in.close();
	outstm.close();
}
void PrintPermuWithRepet(char*arr, int C) {//생성된 배열 arr_s에 넣어주기 
	int i;

	for (i = 0; i < C; i++) {
		pattern[pcnt][i] = arr[i];
	}
	pcnt++;
	return;
}

void swap(char*num1, char*num2) {//swap 함수 
	char temp = *num1;
	*num1 = *num2;
	*num2 = temp;
	return;
}

void PermuWithRepet(char*set, char*arr, int setsize, int arrsize, int C, int i) {//중복 순열 생성 함수

	if (arrsize == 0) {//마지막 중복 순열 생성완료
		PrintPermuWithRepet(arr, C);//생성된 배열 arr_S에 넣어주기 
		return;
	}
	else {//중복 순열 생성
		for (i = setsize - 1; i >= 0; i--) {
			swap(&set[i], &set[setsize - 1]);
			arr[arrsize - 1] = set[setsize - 1];
			PermuWithRepet(set, arr, setsize, arrsize - 1, C, i);
			swap(&set[i], &set[setsize - 1]);
		}
	}
}

void similarity(string MY, string REF) {
	ifstream in1(MY);
	ifstream in2(REF);
	char c1, c2;
	int diff = 0;

	for (int i = 0; i < DL; i++) {
		in1.get(c1);
		in2.get(c2);
		if (c1 != c2) diff++;
	}

	cout << " my DNA와 원본 DNA의 유사도는 = " << ((double)(DL - diff) / DL) * 100 << "% 이다. " << endl;

	in1.close();
	in2.close();
}

void getRF(struct dna *Ref) {
	char c;
	ifstream in("reference.txt");
	// MY DNA파일로 부터 dna정보를 받아오는 for문
	for (int i = 0; in.get(c) && i < DL; i++) {
		Ref[i].gene = c;
		Ref[i].check = 'F';
	}
	in.close();
}

void trivial(struct dna *ref, string filename) {
	int i = 0, k = 0, m, j;

	int miss = 0; // mismatch 개수
	int L = 0; // read 1개 길이

	char c = ' ';

	// ref 읽어오기
	cout << "getRF함수에서 ref 배열에 refDNA 저장" << endl;
	getRF(ref);

	cout << "read 읽어와서, ref 배열에 반영" << endl;
	// SR.txt에서 read 매번 읽어서, trivial로 복원
	ifstream readSR(filename);
	if (readSR.is_open())
	{
		// read 개수만큼 SR 파일에서 불러오기
		for (i = 0; i < ReadN; i++) //  ReadN
		{
			readSR.get(c); j = 0; L = 0;
			while (c != '\n')
			{
				shortreads[j] = c;
				if (i % 100 == 0)
					cout << shortreads[j];
				j++; L++;
				readSR.get(c); // 다음 글자 읽어오기
			}
			if (i % 100 == 0)
				cout << endl;

			// trival 복원 시작			
			for (k = 0; k <= DL - L; k++)
			{
				miss = 0;
				if (ref[k].check != 'T')
				{
					// L만큼 reference DNA와 short read값을 비교하는 for문
					for (m = 0; m < L; m++)
					{
						//  다른 경우
						if (ref[k + m].gene != shortreads[m]) miss++;
						if (miss > mismatch) break;
					}
					// miss개수가 mismatch이하면 맞는 위치
					if (miss <= mismatch)
					{
						// 아직 체크 안된곳
						ref[k].check = 'T'; // 방문 표시
						for (m = 0; m < L; m++) { // snp
							// 이미 ref에 값이 있으므로, 다른 곳만 대입
							if (ref[k + m].gene != shortreads[m])
								ref[k + m].gene = shortreads[m];
						}
						break;
					}
				}
			}

			if (i % 100 == 0)
				cout << i << " 번째 read로 복구 중" << endl;
		}
	}

	readSR.close();

	// 복원된 ref를 파일에 저장
	ofstream writeTriv("trivialMyDNA.txt");
	if (writeTriv.is_open())
	{
		for (i = 0; i < DL; i++)
			writeTriv << ref[i].gene;
	}
	writeTriv.close();
}
