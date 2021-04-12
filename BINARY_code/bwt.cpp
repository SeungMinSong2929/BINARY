#include "DNA_set.h"
#include "BWT.h"
void swap(struct bwt_string *a, struct bwt_string *b)
{
	bwt_string *tmp = a;
	a = b;
	b = tmp;
}

int compare_text(string parent, string child)// string���� �Ѱ��� ��
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
void make_heap_tree(bwt_string *bwt_sub, int root, int lastnode)//������
{
	int parent = root;
	bwt_string rootvalue = bwt_sub[root];
	int left = 2 * parent + 1; int right = left + 1;
	int son;
	while (left <= lastnode)//������ ��庸�� ������
	{
		if (right <= lastnode && compare_text(bwt_sub[left].dna_st, bwt_sub[right].dna_st) == 1)//�������� ���ʺ��� ũ�� 
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
	for (int i = length / 2; i >= 0; i--) { make_heap_tree(bwt_sub, i, length - 1); }//�ִ� heap ����
	cout << "heap ����" << endl;
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
	int k = 0, j;//short read �о���� 
	int miss = 0; // mismatch ����
	int L = 0; // read 1�� ����
	int A_cnt = 0; int C_cnt = 0; int G_cnt = 0; // A , C , G�� ����
	char c = ' ';
	cout << "reference.txt �о� string ref�� ����" << endl;
	ifstream in("reference.txt");
	string  ref;
	int first = 0;//short_readŽ���� ó�� ��ġ
	in >> ref;    in.close();// ref�� reference DNA �ֱ�
	ref += "$"; // $ �߰� 
	int length = ref.length();


	struct bwt_string * bwt_sub = new bwt_string[length];//BWT���� ����� struct 

	clock_t begin, end, sort_begin, sort_end;//�ð� ����
	begin = clock();//���� �ð� ���� ����
	sort_begin = clock();



	int cnt = 0;// fill the rest �迭 ���� 
	int index = 0;//string �о���� ����
	char* tmp = new char[DL + 2];// bwt_sub string�� tmp ����
	for (int i = length - 1; i >= 0; i--) //fill the rest �迭 ����
	{
		memset(tmp, 0, DL + 2);

		for (int j = i; j < length; j++) { tmp[index++] = ref[j]; }//suffix
		for (int j = 0; j < i; j++) { tmp[index++] = ref[j]; }//prefix
		bwt_sub[cnt].dna_st = tmp;//��ü string
		bwt_sub[cnt].first_char = tmp[0];//�� ó�� char
		if (bwt_sub[cnt].first_char == 'A') //ó�� dna ���� count (A,C,G�� count T�� ��ü ���̿��� A+C+G����(��� x))
			A_cnt++;//A��
		else if (bwt_sub[cnt].first_char == 'C')//ó�� dna�� C��
			C_cnt++;
		else if (bwt_sub[cnt].first_char == 'G')//ó�� dna G��
			G_cnt++;
		
		bwt_sub[cnt].last_index = (i + length - 1) % length;//������ ���� ���� reference �� ��ġ 
		cnt++;
		index = 0;//�ʱ�ȭ

	}
	delete[] tmp;//tmp ����

	cout << "fill_the_rest �Ϸ�" << cnt << endl;

	heap_sort(bwt_sub, length);
	cout << "SORT �Ϸ�!" << endl;

	sort_end = clock();
	cout << "fill_the_rest & Heap sort ����ð�" << (double)(sort_end - sort_begin) / CLOCKS_PER_SEC << endl;
	cout << "read �о�ͼ�, ref �迭�� �ݿ�" << endl;

	// SR.txt���� read �Ź� �о, BWT�� ����
	int i;
	ifstream readSR(filename);
	if (readSR.is_open())
	{
		// read ������ŭ SR ���Ͽ��� �ҷ�����
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
				readSR.get(c); // ���� ���� �о����
			}
			if (i % 100 == 0)
				cout << endl;
			int next;

			find(L - 1, first, A_cnt, C_cnt, G_cnt);// short read�� ������ ������ bwt_sub���� ù��° ��ġ ã�� 

			if (bwt_sub[first].first_char == shortreads[L - 1])//��ġ Ȯ��
			{
				next = bwt_sub[first].last_index;//���� Ž�� last_index��������
				for (int j = L - 2; j >= 0; j--)// read j��° ��Ī  
				{
					if (miss > mismatch) {//mismatch������ �ѱ�� 
						first++; //ó�� ��ġ ��ĭ ������ 
						if (bwt_sub[first%length].first_char == shortreads[L - 1]) {//ó�� dna�� ��ġ�� �ʱ�ȭ
							next = bwt_sub[first%length].last_index;//���� Ž�� last_index��������
							miss = 0;//�ʱ�ȭ 
							j = L - 2; //��ġ�ϴ��� Ȯ�� --> L-2���� �ٽ� ��
						}
						else { break; }// ���� 

					}


					for (int x = 0; x < length; x++)// read ���� i��° ���� ��Ī 
					{
						if ((bwt_sub[x].last_index+1)%length == next) {//next�� first index�� ��ġ�� 
							if (bwt_sub[x].first_char != shortreads[j]) miss++;// dna mismatch Ȯ��
							next = bwt_sub[x].last_index%length;//���� index�־��ֱ� 
							break;
						}

					}
					if (j == 0) {//short read ��ġ ã�� ���� 
						next += 1; //first index�κ��� == patter���� ��ġ
						next %= length;
						for (int y = 0; y < L; y++)
						{
							if (ref[(next + y) % length] != shortreads[y])
								ref[(next + y) % length] = shortreads[y];
						}
					}//����
				}
			}
			first = 0;
			miss = 0;
		}
	}
	readSR.close();

	end = clock();
	cout << "����ð�" << (double)(end - begin) / CLOCKS_PER_SEC << endl;

	// ������ ref�� ���Ͽ� ����
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
	// ���� MY DNA(my �迭)�� ������ MY DNA(ref) ���ϸ鼭 ��ġ�� �˻�.
	similarity("myDNA.txt", "BWTMyDNA.txt");


}