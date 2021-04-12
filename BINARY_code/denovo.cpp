#include "denovo.h"
#include "DNA_set.h"

void denovo() {
	struct SHORT *First;

	// 1. 모든 short read 정보 load
	cout << "1. 모든 short read 정보 load" << endl;
	getSR(AllSR);

	// 2. 연결 관계 및 처음 파악
	cout << "2. 연결 관계 및 처음 파악" << endl;
	First = findConnect(AllSR);

	// 3. 복원
	cout << "3. 복원" << endl;
	DE_NOVO(First);


}

void getSR(struct SHORT *AllSR) {
	char c;
	ifstream in("SR.txt");

	// 모든 SR에 대한 정보 저장
	for (int i = 0; i < ReadN; i++) {
		// short read DNA 정보 저장
		for (int j = 0; in.get(c) && c != '\n'; j++) {
			AllSR[i].gene.push_back(c);
		}
		AllSR[i].back = NULL; // NULL로 초기화
		AllSR[i].b = 0; // 0으로 초기화
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
				// start ~ end까지 비교
				while (start <= end) {
					// 처음 원소가 같은 경우
					if (AllSR[j].gene.at(0) == AllSR[i].gene.at(start)) {
						// 마지막 원소가 같은 경우
						//cout << "처음 원소가 같은 경우" <<" ";
						if (AllSR[j].gene.at(size - start - 1) == AllSR[i].gene.back()) {
							// 모든 원소에 대해서 비교
							//cout << "마지막 원소가 같은 경우" << endl;
							k = 1;
							// 처음 +1 ~ 마지막 -1 같은지 비교
							while (k + start < size - 1) {
								// 같지 않은 경우 빠져나감. 
								c1 = AllSR[j].gene.at(k);
								c2 = AllSR[i].gene.at(start + k);
								if (c1 != c2) break;
								else k++;
							}
							// 일치하는 경우
							if (k + start == size - 1) {
								cout << "일치하는 경우";
								first[j] = false;

								// 처음 겹치는 SR이 나온 경우
								if (back) {
									cout << "back 생성" << endl;
									AllSR[i].back = &AllSR[j];
									AllSR[i].b = start;
									back = false;
								}
								// 처음이 아닌 경우
								else {
									m1 = AllSR[i].back->gene.size() - (size - AllSR[i].b);
									m2 = AllSR[j].gene.size() - (size - start);

									// 원래 back보다 긴 경우
									if (m1 < m2) {
										m1 = size - AllSR[i].b;
										m2 = size - start;
										// back와 비교해서 뒷 부분이 일치하면 back
										while (m1 < AllSR[i].back->gene.size()) {
											// 같지 않은 경우 빠져나감. 
											if (AllSR[i].back->gene.at(m1) != AllSR[j].gene.at(m2)) break;
											else {
												m1++;
												m2++;
											}
										}
										// 일치하는 경우
										if (m1 == AllSR[i].back->gene.size()) {
											cout << "back 갱신 ";
											// back 갱신
											AllSR[i].back = &AllSR[j];
											AllSR[i].b = start;
										}
										// 일치하지 않으면 맞지 않음. 
									}
									// 이미 back이 있는 경우 && back보다 짧은 경우
									// pass
								}
							}
							else {
								//cout << "일치하지 않음. ";
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
				// 파일에 쓰기
				of << BACK->gene[t];
			}
			BACK = BACK->back;
		}
	}

	of.close();


}