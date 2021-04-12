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
	FILE* fp = fopen("reference.txt", "r"); //500000���� ���� reference DNA txt����
	if (fp == NULL)
	{
		cout << "���� ���� �Ұ���" << endl;
		fclose(fp); return;
	}
	else
	{
		fseek(fp, 0, SEEK_END); //���� �����͸� ������ �̵���Ŵ
		buf_size = ftell(fp); //������������ ������ġ�� ����
		buffer = (char*)malloc(buf_size + 1); //���� ũ�⸸ŭ �����޸� �Ҵ�
		memset(buffer, 0, buf_size + 1); //���� �޸𸮸� 0���� �ʱ�ȭ
		fseek(fp, 0, SEEK_SET); //���� �����͸� ó������ �̵���Ŵ
		fread(buffer, buf_size, 1, fp); //���ۻ����ŭ buffer�� �о��(���ϳ����� �о��)
		refer = buffer;
		free(buffer);
		fclose(fp);
	}
	FILE* file2 = fopen("myDNA.txt", "wt"); //500000���� myDNA txt���� ����
	int k = 0;
	int num = 0;
	if (file2 != NULL)
	{
		i = 0;
		while (i < DL)
		{
			//�տ� ���ϱ���(PL=11)��ŭ�� �״�� �Է�
			for (int j = 0; j < PL; j++)
			{
				if (i == DL) break;
				fprintf(file2, "%c", refer[i]);
				//	cout << refer[i];
				i++;
			}
			if (i % 10000 == 0)
				cout << i << '\t';
			//��������(RL=61)�϶�
			while (k < RL)
			{
				if (i == DL) break;
				//���ݱ��� ���� ���� ���� mismatch������ �϶��� ����Ȯ���� �ٲ�
				if ((num < mismatch) && (dis(gen) < 20)) // (SNP) 20%�� Ȯ���� �ٲ�
				{
					num++;
					switch (refer[i])
					{
					case 'A':
						if (rand() % 10 < 3) fprintf(file2, "C"); //30%�� Ȯ���� C�� �ٲ�
						else if (rand() % 10 < 3) fprintf(file2, "G"); //30%�� Ȯ���� G�� �ٲ�
						else fprintf(file2, "T"); //������ Ȯ���� T�� �ٲ�
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
				else //�״�� �Է�
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
	// write�ϱ� ���� ���� ���� �� open
	ofstream out(filename);

	// ���� ����
	MakePattern();

	cout << "pattern ���� �Ϸ� " << endl;

	while (i < DL) {
		if (i % 10000 == 0)
			cout << i << endl;
		// ���� �Է�
		for (int t = 0; t < PL; t++)
			out << pattern[j][t];
		j++;

		// ���� �κ� ����
		for (int k = 0; k < RL; k++) {
			// random �κ� ���̸�ŭ string ���� �� ����
			//tmp = rand() % 4;
			tmp = dis(gen);
			// a, c, g,t�� ��ȯ
			switch (tmp) {
			case 0: c = 'A'; break;
			case 1: c = 'C'; break;
			case 2: c = 'G'; break;
			case 3: c = 'T'; break;
			}
			if (k < 2) {
				stmp[k] = c;
			}
			// �����ؼ� ������ ���ϵ��� üũ
			else if (stmp[k - 1] != c || stmp[k - 2] != c) {
				stmp[k] = c;
			}
			// �����ؼ� ������ ��� �ٽ� ����
			else {
				k--;
			}
		}

		// ���� �κ� �Է�
		for (int t = 0; t < RL; t++)
			out << stmp[t];

		//i=i+ ���� ���� + ���� ����
		i = i + PL + RL;
	}
	cout << "���� �Է� �Ϸ�" << endl;

	out.close();
}

void MakePattern() {
	char alphabet[4] = { 'A', 'C', 'G', 'T' };
	char arr[PL];

	PermuWithRepet(alphabet, arr, 4, PL, PL, 4);//arr_s ���̰� PL�� �ߺ����� ���� 

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
void make_SR(string filename) { // shortRead �߶� SR.txt ���Ͽ� ����
	// A,C,G,T�� �ӽ� ������ ����
	char c;
	int i, j, k;
	int readCount; // shortRead�� N�� �����Ǿ����� Ȯ��

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(ReadMin, ReadMax); // ���� Ȯ���ϱ�
	uniform_int_distribution<int> overLap(ReadMin / 2, ReadMax / 2); // read ��ġ�� �ϴ� ����

	srand((unsigned int)time(0));

	ifstream in(filename);
	// MY DNA���Ϸ� ���� dna������ �޾ƿ��� for��
	for (i = 0; in.get(c) && i < DL; i++) {
		my[i] = c;
	}
	in.get(c);

	i = 0; readCount = 0;

	k = dis(gen);
	string fileN = "SR.txt";

	ofstream outstm(fileN);
	if (outstm.is_open()) {

		for (i = DL; i > DL / 2; i -= k) { // �ڿ������� k��ŭ ������ �̵�. 
			// i-k���� i-1��°���� k���� ���ڿ�.
			for (j = (i - k); j < i; j++)
				outstm << my[j]; // ���Ͽ� �ٷ� shortread ���� 1���� ����
			outstm << '\n';

			i += overLap(gen);

			k = dis(gen);
			readCount++;
		}
		cout << readCount << endl;

		for (i = 0; i <= DL / 2; i += k) { // �տ������� k��ŭ �ڷ� �̵�.
			// i���� i+k-1��°���� k���� ���ڿ�.
			for (j = i; j < i + k; j++)
				outstm << my[j]; // ���Ͽ� �ٷ� shortread ���� 1���� ����
			outstm << '\n';

			i -= overLap(gen);

			k = dis(gen);
			readCount++;
		}
		cout << readCount << endl;
		i = DL / 4;
		while (readCount < ReadN) { // n���� �ǵ��� �Ǵٸ� ������Ģ���� read���� �ڸ���
			// i���� i+k-1��°���� k���� ���ڿ�.
			for (j = i; j < i + k; j++)
				outstm << my[j]; // ���Ͽ� �ٷ� shortread ���� 1���� ����
			outstm << '\n';

			i += overLap(gen);
			k = dis(gen);
			readCount++;
		}
	}
	if (readCount == ReadN)
		cout << " ��� ������ ���Ե� short read����!!! �Ϸ�!!!!!!!!!1" << endl;

	in.close();
	outstm.close();
}
void PrintPermuWithRepet(char*arr, int C) {//������ �迭 arr_s�� �־��ֱ� 
	int i;

	for (i = 0; i < C; i++) {
		pattern[pcnt][i] = arr[i];
	}
	pcnt++;
	return;
}

void swap(char*num1, char*num2) {//swap �Լ� 
	char temp = *num1;
	*num1 = *num2;
	*num2 = temp;
	return;
}

void PermuWithRepet(char*set, char*arr, int setsize, int arrsize, int C, int i) {//�ߺ� ���� ���� �Լ�

	if (arrsize == 0) {//������ �ߺ� ���� �����Ϸ�
		PrintPermuWithRepet(arr, C);//������ �迭 arr_S�� �־��ֱ� 
		return;
	}
	else {//�ߺ� ���� ����
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

	cout << " my DNA�� ���� DNA�� ���絵�� = " << ((double)(DL - diff) / DL) * 100 << "% �̴�. " << endl;

	in1.close();
	in2.close();
}

void getRF(struct dna *Ref) {
	char c;
	ifstream in("reference.txt");
	// MY DNA���Ϸ� ���� dna������ �޾ƿ��� for��
	for (int i = 0; in.get(c) && i < DL; i++) {
		Ref[i].gene = c;
		Ref[i].check = 'F';
	}
	in.close();
}

void trivial(struct dna *ref, string filename) {
	int i = 0, k = 0, m, j;

	int miss = 0; // mismatch ����
	int L = 0; // read 1�� ����

	char c = ' ';

	// ref �о����
	cout << "getRF�Լ����� ref �迭�� refDNA ����" << endl;
	getRF(ref);

	cout << "read �о�ͼ�, ref �迭�� �ݿ�" << endl;
	// SR.txt���� read �Ź� �о, trivial�� ����
	ifstream readSR(filename);
	if (readSR.is_open())
	{
		// read ������ŭ SR ���Ͽ��� �ҷ�����
		for (i = 0; i < ReadN; i++) //  ReadN
		{
			readSR.get(c); j = 0; L = 0;
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

			// trival ���� ����			
			for (k = 0; k <= DL - L; k++)
			{
				miss = 0;
				if (ref[k].check != 'T')
				{
					// L��ŭ reference DNA�� short read���� ���ϴ� for��
					for (m = 0; m < L; m++)
					{
						//  �ٸ� ���
						if (ref[k + m].gene != shortreads[m]) miss++;
						if (miss > mismatch) break;
					}
					// miss������ mismatch���ϸ� �´� ��ġ
					if (miss <= mismatch)
					{
						// ���� üũ �ȵȰ�
						ref[k].check = 'T'; // �湮 ǥ��
						for (m = 0; m < L; m++) { // snp
							// �̹� ref�� ���� �����Ƿ�, �ٸ� ���� ����
							if (ref[k + m].gene != shortreads[m])
								ref[k + m].gene = shortreads[m];
						}
						break;
					}
				}
			}

			if (i % 100 == 0)
				cout << i << " ��° read�� ���� ��" << endl;
		}
	}

	readSR.close();

	// ������ ref�� ���Ͽ� ����
	ofstream writeTriv("trivialMyDNA.txt");
	if (writeTriv.is_open())
	{
		for (i = 0; i < DL; i++)
			writeTriv << ref[i].gene;
	}
	writeTriv.close();
}
