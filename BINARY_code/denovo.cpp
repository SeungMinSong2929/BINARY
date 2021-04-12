#include "denovo.h"
#include "DNA_set.h"

void denovo() {
	struct SHORT *First;

	// 1. ��� short read ���� load
	cout << "1. ��� short read ���� load" << endl;
	getSR(AllSR);

	// 2. ���� ���� �� ó�� �ľ�
	cout << "2. ���� ���� �� ó�� �ľ�" << endl;
	First = findConnect(AllSR);

	// 3. ����
	cout << "3. ����" << endl;
	DE_NOVO(First);


}

void getSR(struct SHORT *AllSR) {
	char c;
	ifstream in("SR.txt");

	// ��� SR�� ���� ���� ����
	for (int i = 0; i < ReadN; i++) {
		// short read DNA ���� ����
		for (int j = 0; in.get(c) && c != '\n'; j++) {
			AllSR[i].gene.push_back(c);
		}
		AllSR[i].back = NULL; // NULL�� �ʱ�ȭ
		AllSR[i].b = 0; // 0���� �ʱ�ȭ
	}

	in.close();
}

struct SHORT *findConnect(struct SHORT *AllSR) {
	//bool first[ReadN];
	int start, end, size;
	int k = 1, m1, m2;
	bool back;
	struct SHORT *First = NULL;
	char c1, c2;
	fill_n(first, ReadN, true);

	for (int i = 0; i < ReadN; i++) {
		cout << "\n" << i << " ";
		size = AllSR[i].gene.size();
		start = (int)(size * 0.25);
		end = (int)(size * 0.75);
		//end = size;
		back = true;
		for (int j = 0; j < ReadN; j++) {
			if (i != j) {
				start = (int)(size * 0.25);
				// start ~ end���� ��
				while (start <= end) {
					// ó�� ���Ұ� ���� ���
					if (AllSR[j].gene.at(0) == AllSR[i].gene.at(start)) {
						// ������ ���Ұ� ���� ���
						//cout << "ó�� ���Ұ� ���� ���" <<" ";
						if (AllSR[j].gene.at(size - start - 1) == AllSR[i].gene.back()) {
							// ��� ���ҿ� ���ؼ� ��
							//cout << "������ ���Ұ� ���� ���" << endl;
							k = 1;
							// ó�� +1 ~ ������ -1 ������ ��
							while (k + start < size - 1) {
								// ���� ���� ��� ��������. 
								c1 = AllSR[j].gene.at(k);
								c2 = AllSR[i].gene.at(start + k);
								if (c1 != c2) break;
								else k++;
							}
							// ��ġ�ϴ� ���
							if (k + start == size - 1) {
								cout << "��ġ�ϴ� ���";
								first[j] = false;

								// ó�� ��ġ�� SR�� ���� ���
								if (back) {
									cout << "back ����" << endl;
									AllSR[i].back = &AllSR[j];
									AllSR[i].b = start;
									back = false;
								}
								// ó���� �ƴ� ���
								else {
									m1 = AllSR[i].back->gene.size() - (size - AllSR[i].b);
									m2 = AllSR[j].gene.size() - (size - start);

									// ���� back���� �� ���
									if (m1 < m2) {
										m1 = size - AllSR[i].b;
										m2 = size - start;
										// back�� ���ؼ� �� �κ��� ��ġ�ϸ� back
										while (m1 < AllSR[i].back->gene.size()) {
											// ���� ���� ��� ��������. 
											if (AllSR[i].back->gene.at(m1) != AllSR[j].gene.at(m2)) break;
											else {
												m1++;
												m2++;
											}
										}
										// ��ġ�ϴ� ���
										if (m1 == AllSR[i].back->gene.size()) {
											cout << "back ���� ";
											// back ����
											AllSR[i].back = &AllSR[j];
											AllSR[i].b = start;
										}
										// ��ġ���� ������ ���� ����. 
									}
									// �̹� back�� �ִ� ��� && back���� ª�� ���
									// pass
								}
							}
							else {
								//cout << "��ġ���� ����. ";
							}
						}
					}
					start++;
				}
			}
		}
	}

	for (int f = 0; f < ReadN; f++) {
		if (first[f] == true) {
			cout << "first=" << f << " ";
			First = &AllSR[f];
		}
	}
	cout << endl;

	return First;
}


void DE_NOVO(struct SHORT *First) {
	struct SHORT *BACK = First;
	int t = 0;

	ofstream of("denovoMyDNA.txt");

	if (of.is_open())
	{
		while (BACK != NULL) {
			if (BACK->b == 0) BACK->b = BACK->gene.size();
			for (t = 0; t < BACK->b; t++) {
				// ���Ͽ� ����
				of << BACK->gene[t];
			}
			BACK = BACK->back;
		}
	}

	of.close();


}