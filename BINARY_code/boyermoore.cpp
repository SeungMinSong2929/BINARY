#include "boyermoore.h"
#include "DNA_set.h"

//bad character ���̺��� ����
int* createBC(char* pat, int M)
{
	int* bc = new int[NUM_OF_CHARS];

	// ���̺��� -1�� �ʱ�ȭ(���Ͽ� �������� �ʴ� ���ڴ� ��� -1)
	for (int i = 0; i < NUM_OF_CHARS; ++i)
		bc[i] = -1;

	//���Ͽ� �����ϴ� ĳ������ ������ ��ġ�� bc���̺� ����
	for (int i = 0; i < M - 1; ++i)
		bc[(int)pat[i]] = i; //ĳ������ int���� ������ �迭�� ��ġ ����

	return bc;
}

void BoyerMoore(int SR_num, int l)
{ //SR_num: SR_num���� short read, l: ���Ϲ迭(ShortRead)�� ũ��
	int i = 0, j = 0;
	int n; //n: �ؽ�Ʈ�迭(reference����)�� ũ��
	char* buffer;
	int buf_size;
	string text;
	string pattern;
	char* buf = (char*)malloc(l + 2);
	int x = 0;
	int miss = 0; //miss ��ġ ����
	int* bc = NULL; //bc���̺�

	FILE* fp = fopen("reference.txt", "r");
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
		text = buffer;
		free(buffer);
		fclose(fp);
	}
	n = text.length(); //n: �ؽ�Ʈ�迭(reference����)�� ũ��

	FILE* file = fopen("SR.txt", "r");
	if (file != NULL)
	{
		while (x < SR_num) //short read ���� : 20000
		{
			fgets(buf, ReadMax + 2, file); //file���� �����о��
			pattern = buf;
			l = pattern.length() - 1; // l: ���ϱ���
			bc = createBC(buf, l); //������ bc���̺��� ����
			i = 0;
			while (i <= n - l)
			{
				miss = 0;
				//���̾� ���� �˰����� �ڿ������� �����ϹǷ� pattern������ - 1�� ���ش�.
				//-1�� ���ִ� ������ �ε����� 0���� �����ϱ� ����
				j = l - 1;
				//�ڿ������� �˻��ϰ� �ε����� �����ϴ� �����̹Ƿ� j�� 0���� �̻��϶�
				while (j >= 0)
				{
					//���ϰ� �ؽ�Ʈ�� ���� �ʴٸ�
					if (pattern[j] != text[i + j])
					{
						miss++; //miss ����
						//miss������ mismatch(��� ����)���� ũ�ٸ�
						if (miss > mismatch)
						{
							//�ؽ�Ʈ�� bad character�� ���Ͽ� �����Ѵٸ� �� ��ġ�� ������ �̵���Ŵ.
							//bad character�� ��ġ�� ������ ���� �� ��ġ���� �ڿ� ���� �� �ִµ�
							//�̷� ���, j - bc[text[i + j]]�� ������ �� �� �����Ƿ�
							//�̶��� max�� �̿��Ͽ� 1��ŭ ������Ų��.
							i += max(1, j - bc[text[i + j]]);
							break;
						}
					}
					j = j - 1;
				}
				//miss������ mismatch(��� ����) �����϶� ��Ī�� �� ���̹Ƿ�
				if (miss <= mismatch)
				{
					//�����ں��� ���ϸ鼭 �ϳ��� �����ϸ鼭 ���Ѵ�.
					for (j = l - 1; j >= 0; j--) //for���� ���� �ϳ��� ��
					{
						//�������������� �� �κ��� SNP�̹Ƿ� text(referenceDNA)�κ��� pattern(shortRead)���� �ٲ�
						if (pattern[j] != text[i + j])
						{
							text[i + j] = pattern[j];
						}
					}
					break; // for�� ������ ������ short read ����
				}
			}
			if (x % 100 == 0)
				cout << x << " ��° read�� ���� ��" << endl;
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