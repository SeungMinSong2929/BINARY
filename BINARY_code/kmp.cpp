#include "kmp.h"
#include "DNA_set.h"

void KMP(struct dna *ref, string filename)
{
	int s = 0; // m�� ���� �ε���
	int miss = 0; // mismatch ����
	int L = 0; // read 1�� ����

	char c = ' ';

	int m; int t; int tmpM;

	// ref �о����
	cout << "getRF�Լ����� ref �迭�� refDNA ����" << endl;
	getRF(ref);

	cout << "read �о�ͼ�, ref �迭�� �ݿ�" << endl;
	// SR.txt���� read �Ź� �о, kmp�� ����
	short SP[ReadMax] = { 0 };
	ifstream readSR(filename);
	if (readSR.is_open())
	{
		// read ������ŭ SR ���Ͽ��� �ҷ�����
		for (s = 0; s < ReadN; s++) //  ReadN
		{
			// 1) read �о����
			readSR.get(c); int r = 0; L = 0;
			while (c != '\n')
			{
				shortreads[r] = c;
				if (s % 100 == 0)
					cout << shortreads[r];
				r++; L++;
				readSR.get(c); // ���� ���� �о����
			}
			if (s % 100 == 0)
				cout << endl;

			// 2) read�� �ִ� ���κ� ���̺� �����
			for (int sp = 0; sp < L; sp++)
				SP[sp] = 0;
			int i, j = 0;
			// �ִ� ���κ� ���̺� ���� for��
			for (i = 1; i < L; i++) {
				while (j > 0 && shortreads[j] != shortreads[i])
					j = SP[j - 1];
				if (shortreads[j] == shortreads[i])
					SP[i] = ++j;
			}

			// 3) kmp ��ġ �ϴ� �κ� ã�� for�� & while��
			miss = 0;
			t = 0;
			tmpM = 0; m = 0;
			for (t = 0; t < DL - L + 1; t) {
				// �湮�� ���
				if (ref[t].check == 'T')
					t = t + ReadMin; // �ּ� ReadMin��ŭ�� ���������Ƿ�, ReadMin��ŭ t�� �ǳʶ�.
				while (m < L) {
					if (ref[t + m].gene != shortreads[m]) {
						miss++;
						// ó�� miss �߻��ϴ� read�� �ε��� ����.
						if (miss == 1) tmpM = m;

						if (miss < mismatch) { // mismatch ��� �������� ������
							m++; // read�� ���� �ε����� �Ѿ
						}
						else { // miss>=mismatch
							// ��Ī�Ǵ� ���
							if (miss == mismatch && m == L - 1) {
								m++;
								break; // �����Ϸ� ������
							}
							// mismatch ��� ���� �ʰ��̹Ƿ�, t�� �̵��Ͽ� �ٽ� �� �����ؾ� ��.
							miss = 0; // miss �ʱ�ȭ 
							if (tmpM == 0) { // ó�� mismatch �߻� ��ġ�� 0�� ���
								// ó������ miss�� �߻��ߴ� ���̹Ƿ�,
								t++; // t+1 ��ġ���� �ٽ� ���ϸ� ��
								m = 0; // m�� 0���� �ʱ�ȭ
							}
							else { // ó�� mismatch �߻� ��ġ�� 0���� ū ���
								t = t + tmpM; // ref���� miss�� �߻��� ���� ��������, �ٽ� ��.
								m = SP[tmpM - 1]; // tmpM �������� miss�� �������Ƿ�, SP �̿��Ͽ� �̵� ����
							}
							break;
						}
					}
					else {
						m++; // ���ڰ� ��ġ�Ҷ�						
					}
				}
				// ��ġ 
				if (m == L && miss <= mismatch && ref[t].check != 'T') {
					ref[t].check = 'T'; // �湮 ǥ��
					for (int z = 0; z < L; z++)
					{
						// �̹� ref�� ���� �����Ƿ�, �ٸ� ���� ����
						if (ref[t + z].gene != shortreads[z])
							ref[t + z].gene = shortreads[z];
					}
					break;
				}
			}

			if (s % 100 == 0)
				cout << s << " ��° read�� KMP���� ��" << endl;
		}
	}
	readSR.close();
	
	// kmpMyDNA.txt�� ������ myDNA ����
	ofstream writekmp("kmpMyDNA.txt");
	if (writekmp.is_open())
	{
		for (int w = 0; w < DL; w++)
			writekmp << ref[w].gene;
	}
	writekmp.close();
}