#include "boyermoore.h"
#include "DNA_set.h"

//bad character 테이블을 만듦
int* createBC(char* pat, int M)
{
	int* bc = new int[NUM_OF_CHARS];

	// 테이블을 -1로 초기화(패턴에 존재하지 않는 문자는 모두 -1)
	for (int i = 0; i < NUM_OF_CHARS; ++i)
		bc[i] = -1;

	//패턴에 존재하는 캐릭터의 마지막 위치를 bc테이블에 넣음
	for (int i = 0; i < M - 1; ++i)
		bc[(int)pat[i]] = i; //캐릭터의 int형인 숫자의 배열에 위치 삽입

	return bc;
}

void BoyerMoore(int SR_num, int l)
{ //SR_num: SR_num개의 short read, l: 패턴배열(ShortRead)의 크기
	int i = 0, j = 0;
	int n; //n: 텍스트배열(reference파일)의 크기
	char* buffer;
	int buf_size;
	string text;
	string pattern;
	char* buf = (char*)malloc(l + 2);
	int x = 0;
	int miss = 0; //miss 매치 변수
	int* bc = NULL; //bc테이블

	FILE* fp = fopen("reference.txt", "r");
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
		text = buffer;
		free(buffer);
		fclose(fp);
	}
	n = text.length(); //n: 텍스트배열(reference파일)의 크기

	FILE* file = fopen("SR.txt", "r");
	if (file != NULL)
	{
		while (x < SR_num) //short read 개수 : 20000
		{
			fgets(buf, ReadMax + 2, file); //file에서 한줄읽어옴
			pattern = buf;
			l = pattern.length() - 1; // l: 패턴길이
			bc = createBC(buf, l); //패턴의 bc테이블을 만듦
			i = 0;
			while (i <= n - l)
			{
				miss = 0;
				//보이어 무어 알고리즘은 뒤에서부터 접근하므로 pattern길이의 - 1을 해준다.
				//-1을 해주는 이유는 인덱스가 0부터 시작하기 때문
				j = l - 1;
				//뒤에서부터 검사하고 인덱스를 감소하는 형식이므로 j가 0보다 이상일때
				while (j >= 0)
				{
					//패턴과 텍스트가 같지 않다면
					if (pattern[j] != text[i + j])
					{
						miss++; //miss 증가
						//miss개수가 mismatch(허용 개수)보다 크다면
						if (miss > mismatch)
						{
							//텍스트의 bad character가 패턴에 존재한다면 그 위치로 패턴을 이동시킴.
							//bad character의 위치가 패턴의 현재 비교 위치보다 뒤에 있을 수 있는데
							//이럴 경우, j - bc[text[i + j]]가 음수가 될 수 있으므로
							//이때는 max를 이용하여 1만큼 증가시킨다.
							i += max(1, j - bc[text[i + j]]);
							break;
						}
					}
					j = j - 1;
				}
				//miss개수가 mismatch(허용 개수) 이하일때 매칭이 된 것이므로
				if (miss <= mismatch)
				{
					//끝글자부터 비교하면서 하나씩 감소하면서 비교한다.
					for (j = l - 1; j >= 0; j--) //for문을 돌려 하나씩 비교
					{
						//동일하지않으면 그 부분이 SNP이므로 text(referenceDNA)부분을 pattern(shortRead)으로 바꿈
						if (pattern[j] != text[i + j])
						{
							text[i + j] = pattern[j];
						}
					}
					break; // for문 나가서 나머지 short read 수행
				}
			}
			if (x % 100 == 0)
				cout << x << " 번째 read로 복구 중" << endl;
			x++;
		}
	}
	FILE* dna_file = fopen("boyermooreMyDNA.txt", "wt");
	if (dna_file != NULL)
	{
		for (int i = 0; i < DL; i++)
		{
			fprintf(dna_file, "%c", text[i]);
		}
	}
	free(buf);
	fclose(dna_file);
	delete[] bc;
}