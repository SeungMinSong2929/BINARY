#include "DNA_set.h"
#include "BWT.h"
void swap(struct bwt_string *a, struct bwt_string *b)
{
	bwt_string *tmp = a;
	a = b;
	b = tmp;
}

int compare_text(string parent, string child)// string원소 한개씩 비교
{
	int i = 0;

	if (parent[0] == '$')  return 1;
	if (child[0] == '$') return  2;
	while (1)
	{
		if (parent[i] < child[i])
			return 1;
		else if (parent[i] > child[i])
			return 2;
		i++;
	}
	return 0;
}
void make_heap_tree(bwt_string *bwt_sub, int root, int lastnode)//힙생성
{
	int parent = root;
	bwt_string rootvalue = bwt_sub[root];
	int left = 2 * parent + 1; int right = left + 1;
	int son;
	while (left <= lastnode)//마지막 노드보다 작으면
	{
		if (right <= lastnode && compare_text(bwt_sub[left].dna_st, bwt_sub[right].dna_st) == 1)//오른쪽이 왼쪽보다 크면 
			son = right;
		else son = left;

		if (compare_text(rootvalue.dna_st, bwt_sub[son].dna_st) == 1) {
			swap(bwt_sub[parent], bwt_sub[son]);
			parent = son;
			left = parent * 2 + 1;
			right = left + 1;
		}
		else break;
	}

}
void heap_sort(bwt_string *bwt_sub, int length)//heap sort
{
	for (int i = length / 2; i >= 0; i--) { make_heap_tree(bwt_sub, i, length - 1); }//최대 heap 생성
	cout << "heap 생성" << endl;
	for (int i = length - 1; i > 0; i--)//sort
	{
		swap(bwt_sub[0], bwt_sub[i]);
		make_heap_tree(bwt_sub, 0, i - 1);
	}

}
void find(int i, int &first, int A_cnt, int C_cnt, int G_cnt)
{

	char check = shortreads[i];
	if (check == 'A')
	{
		first = 1;
	}
	else if (check == 'C')
	{
		first = A_cnt + 1;
	}
	else if (check == 'G')
	{
		first = A_cnt + C_cnt + 1;
	}
	else if (check == 'T')
	{
		first = A_cnt + C_cnt + G_cnt + 1;
	}

}


void BWT(string filename) {
	int k = 0, j;//short read 읽어오는 
	int miss = 0; // mismatch 개수
	int L = 0; // read 1개 길이
	int A_cnt = 0; int C_cnt = 0; int G_cnt = 0; // A , C , G의 개수
	char c = ' ';
	cout << "reference.txt 읽어 string ref에 저장" << endl;
	ifstream in("reference.txt");
	string  ref;
	int first = 0;//short_read탐색시 처음 위치
	in >> ref;    in.close();// ref에 reference DNA 넣기
	ref += "$"; // $ 추가 
	int length = ref.length();


	struct bwt_string * bwt_sub = new bwt_string[length];//BWT에서 사용할 struct 

	clock_t begin, end, sort_begin, sort_end;//시간 측정
	begin = clock();//실행 시간 측정 시작
	sort_begin = clock();



	int cnt = 0;// fill the rest 배열 순서 
	int index = 0;//string 읽어오는 순서
	char* tmp = new char[DL + 2];// bwt_sub string의 tmp 변수
	for (int i = length - 1; i >= 0; i--) //fill the rest 배열 생성
	{
		memset(tmp, 0, DL + 2);

		for (int j = i; j < length; j++) { tmp[index++] = ref[j]; }//suffix
		for (int j = 0; j < i; j++) { tmp[index++] = ref[j]; }//prefix
		bwt_sub[cnt].dna_st = tmp;//전체 string
		bwt_sub[cnt].first_char = tmp[0];//맨 처음 char
		if (bwt_sub[cnt].first_char == 'A') //처음 dna 개수 count (A,C,G만 count T는 전체 길이에서 A+C+G뺀것(사용 x))
			A_cnt++;//A면
		else if (bwt_sub[cnt].first_char == 'C')//처음 dna가 C면
			C_cnt++;
		else if (bwt_sub[cnt].first_char == 'G')//처음 dna G면
			G_cnt++;
		
		bwt_sub[cnt].last_index = (i + length - 1) % length;//마지막 원소 원래 reference 의 위치 
		cnt++;
		index = 0;//초기화

	}
	delete[] tmp;//tmp 제거

	cout << "fill_the_rest 완료" << cnt << endl;

	heap_sort(bwt_sub, length);
	cout << "SORT 완료!" << endl;

	sort_end = clock();
	cout << "fill_the_rest & Heap sort 수행시간" << (double)(sort_end - sort_begin) / CLOCKS_PER_SEC << endl;
	cout << "read 읽어와서, ref 배열에 반영" << endl;

	// SR.txt에서 read 매번 읽어서, BWT로 복원
	int i;
	ifstream readSR(filename);
	if (readSR.is_open())
	{
		// read 개수만큼 SR 파일에서 불러오기
		for (i = 0; i < ReadN; i++) //  ReadN
		{
			readSR.get(c); j = 0; L = 0;
			miss = 0;
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
			int next;

			find(L - 1, first, A_cnt, C_cnt, G_cnt);// short read의 마지막 원소의 bwt_sub에서 첫번째 위치 찾기 

			if (bwt_sub[first].first_char == shortreads[L - 1])//일치 확인
			{
				next = bwt_sub[first].last_index;//다음 탐색 last_index가져오기
				for (int j = L - 2; j >= 0; j--)// read j번째 매칭  
				{
					if (miss > mismatch) {//mismatch개수를 넘기면 
						first++; //처음 위치 한칸 앞으로 
						if (bwt_sub[first%length].first_char == shortreads[L - 1]) {//처음 dna와 일치시 초기화
							next = bwt_sub[first%length].last_index;//다음 탐색 last_index가져오기
							miss = 0;//초기화 
							j = L - 2; //일치하는지 확인 --> L-2부터 다시 비교
						}
						else { break; }// 실패 

					}


					for (int x = 0; x < length; x++)// read 에서 i번째 문자 매칭 
					{
						if ((bwt_sub[x].last_index+1)%length == next) {//next와 first index와 일치시 
							if (bwt_sub[x].first_char != shortreads[j]) miss++;// dna mismatch 확인
							next = bwt_sub[x].last_index%length;//다음 index넣어주기 
							break;
						}

					}
					if (j == 0) {//short read 위치 찾기 성공 
						next += 1; //first index로변경 == patter시작 위치
						next %= length;
						for (int y = 0; y < L; y++)
						{
							if (ref[(next + y) % length] != shortreads[y])
								ref[(next + y) % length] = shortreads[y];
						}
					}//성공
				}
			}
			first = 0;
			miss = 0;
		}
	}
	readSR.close();

	end = clock();
	cout << "수행시간" << (double)(end - begin) / CLOCKS_PER_SEC << endl;

	// 복원된 ref를 파일에 저장
	int cnt2 = 0;
	ofstream writeTriv("BWTMyDNA.txt");
	if (writeTriv.is_open())
	{
		for (int i = 0; i < DL; i++)
		{
			writeTriv << ref[i];
		}
	}
	writeTriv.close();
	// 원본 MY DNA(my 배열)과 복원된 MY DNA(ref) 비교하면서 일치율 검사.
	similarity("myDNA.txt", "BWTMyDNA.txt");


}