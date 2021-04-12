#include "kmp.h"
#include "DNA_set.h"

void KMP(struct dna *ref, string filename)
{
	int s = 0; // m은 패턴 인덱스
	int miss = 0; // mismatch 개수
	int L = 0; // read 1개 길이

	char c = ' ';

	int m; int t; int tmpM;

	// ref 읽어오기
	cout << "getRF함수에서 ref 배열에 refDNA 저장" << endl;
	getRF(ref);

	cout << "read 읽어와서, ref 배열에 반영" << endl;
	// SR.txt에서 read 매번 읽어서, kmp로 복원
	short SP[ReadMax] = { 0 };
	ifstream readSR(filename);
	if (readSR.is_open())
	{
		// read 개수만큼 SR 파일에서 불러오기
		for (s = 0; s < ReadN; s++) //  ReadN
		{
			// 1) read 읽어오기
			readSR.get(c); int r = 0; L = 0;
			while (c != '\n')
			{
				shortreads[r] = c;
				if (s % 100 == 0)
					cout << shortreads[r];
				r++; L++;
				readSR.get(c); // 다음 글자 읽어오기
			}
			if (s % 100 == 0)
				cout << endl;

			// 2) read의 최대 접두부 테이블 만들기
			for (int sp = 0; sp < L; sp++)
				SP[sp] = 0;
			int i, j = 0;
			// 최대 접두부 테이블 생성 for문
			for (i = 1; i < L; i++) {
				while (j > 0 && shortreads[j] != shortreads[i])
					j = SP[j - 1];
				if (shortreads[j] == shortreads[i])
					SP[i] = ++j;
			}

			// 3) kmp 일치 하는 부분 찾는 for문 & while문
			miss = 0;
			t = 0;
			tmpM = 0; m = 0;
			for (t = 0; t < DL - L + 1; t) {
				// 방문한 경우
				if (ref[t].check == 'T')
					t = t + ReadMin; // 최소 ReadMin만큼은 복원됐으므로, ReadMin만큼 t는 건너뜀.
				while (m < L) {
					if (ref[t + m].gene != shortreads[m]) {
						miss++;
						// 처음 miss 발생하는 read의 인덱스 저장.
						if (miss == 1) tmpM = m;

						if (miss < mismatch) { // mismatch 허용 개수보다 작으면
							m++; // read의 다음 인덱스로 넘어감
						}
						else { // miss>=mismatch
							// 매칭되는 경우
							if (miss == mismatch && m == L - 1) {
								m++;
								break; // 복원하러 나가기
							}
							// mismatch 허용 개수 초과이므로, t를 이동하여 다시 비교 시작해야 함.
							miss = 0; // miss 초기화 
							if (tmpM == 0) { // 처음 mismatch 발생 위치가 0인 경우
								// 처음부터 miss가 발생했던 것이므로,
								t++; // t+1 위치부터 다시 비교하면 됨
								m = 0; // m도 0으로 초기화
							}
							else { // 처음 mismatch 발생 위치가 0보다 큰 경우
								t = t + tmpM; // ref에서 miss가 발생한 곳을 시작으로, 다시 비교.
								m = SP[tmpM - 1]; // tmpM 전까지는 miss가 없었으므로, SP 이용하여 이동 가능
							}
							break;
						}
					}
					else {
						m++; // 문자가 일치할때						
					}
				}
				// 일치 
				if (m == L && miss <= mismatch && ref[t].check != 'T') {
					ref[t].check = 'T'; // 방문 표시
					for (int z = 0; z < L; z++)
					{
						// 이미 ref에 값이 있으므로, 다른 곳만 대입
						if (ref[t + z].gene != shortreads[z])
							ref[t + z].gene = shortreads[z];
					}
					break;
				}
			}

			if (s % 100 == 0)
				cout << s << " 번째 read로 KMP복구 중" << endl;
		}
	}
	readSR.close();
	
	// kmpMyDNA.txt에 복원된 myDNA 쓰기
	ofstream writekmp("kmpMyDNA.txt");
	if (writekmp.is_open())
	{
		for (int w = 0; w < DL; w++)
			writekmp << ref[w].gene;
	}
	writekmp.close();
}