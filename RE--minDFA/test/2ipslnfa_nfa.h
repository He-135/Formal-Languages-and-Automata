#pragma once

#include"1re_ipslnfa.h"

class nfa {
public:
	int* fstates;
	int finalsize;
	int statesize;
	nfa() {
		fstates = NULL;
		finalsize = statesize = 0;
	}

	~nfa() {
		delete[] fstates;
	}
};


int** IPSLNFAtoNFA(int** p, nfa& nfa1) {


	int** n;//定义和初始化二维数组n
	int sizeM = p[0][0];


	{n = new int* [sizeM];
	for (int i = 0; i < sizeM; i++) {
		n[i] = new int[sizeM];
	}
	for (int i = 0; i < sizeM; i++) {
		for (int j = 0; j < sizeM; j++) {
			n[i][j] = -1;
		}
	}
	}

	int i, j, k, l, z, x;//后续循环用到的变量

	//若在ε-NFA中,状态i经过ε到达j,j经过0到达k,k经过ε到达l,则在对应的NFA中,状态i经过0到达l
	//矩阵的所有单元初始化为5;若可以通过0到达，则记为0；若可以通过1到达，则记为1；
	//若通过0 1都可以到达，则记为2；若可以ε通过到达，则记为#；
	//其他组合情况已经在RE转换到ε-NFA中删去
	queue<int> c1, c2, c3, c4, c5, c6;
	int q1, q2, q3;

	for (i = 0; i < sizeM; i++) {
		while (!c1.empty()) {
			c1.pop();
		}
		while (!c2.empty()) {
			c2.pop();
		}
		while (!c3.empty()) {
			c3.pop();
		}
		while (!c4.empty()) {
			c4.pop();
		}
		while (!c5.empty()) {
			c5.pop();
		}
		while (!c6.empty()) {
			c6.pop();
		}

		for (j = 0; j < sizeM; j++) {
			if (p[i][j] == 3 || i == j)//若状态i到j可以通过ε到达
			{
				c1.push(j);//j为i通过一步到达

			}
		}

		for (q3 = 0; (q3 < sizeM * sizeM) && (!c1.empty()); q3++) {
			q1 = c1.front();
			c1.pop();
			c2.push(q1);

			for (q2 = 0; q2 < sizeM; q2++) {


				if (p[q1][q2] == 3)
					c1.push(q2);
			}
		}




		for (z = 0; (z < sizeM) && (!c2.empty()); z++) {
			j = c2.front();
			c2.pop();
			c1.push(j);//j为i的闭包中的状态
			for (k = 0; k < sizeM; k++) {
				if (p[j][k] == 0)//若状态j到k可以通过0到达
				{
					for (l = 0; l < sizeM; l++) {//若状态k到l可以通过ε到达
						if (p[k][l] == 3 || k == l)//若状态k到l可以通过ε到达
						{
							c3.push(l);//l为k通过一步到达

						}
					}

					for (q3 = 0; (q3 < sizeM * sizeM) && (!c3.empty()); q3++) {
						q1 = c3.front();
						c3.pop();
						c4.push(q1);

						for (q2 = 0; q2 < sizeM; q2++) {

							if (p[q1][q2] == 3)
								c3.push(q2);
						}
					}


					for (x = 0; (x < sizeM) && (!c4.empty()); x++) {
						l = c4.front();
						c4.pop();
						c3.push(l);//l为k的闭包中的状态
						if (n[i][l] != 2) {
							if (n[i][l] == 1)//若状态i经过1也可以到达l
								n[i][l] = 2;
							else //若状态i经过1不可以到达l
								n[i][l] = 0;
						}

					}


				}
				else if (p[j][k] == 1)//若状态j到k可以通过0或1到达
				{
					for (l = 0; l < sizeM; l++) {//若状态k到l可以通过ε到达

						if (p[k][l] == 3 || k == l)//若状态k到l可以通过ε到达
						{
							c5.push(l);//l为k通过一步到达

						}
					}

					for (q3 = 0; (q3 < sizeM * sizeM) && (!c5.empty()); q3++) {
						q1 = c5.front();
						c5.pop();
						c6.push(q1);

						for (q2 = 0; q2 < sizeM; q2++) {


							if (p[q1][q2] == 3)
								c5.push(q2);
						}
					}

					for (x = 0; (x < sizeM) && (!c6.empty()); x++) {
						l = c6.front();
						c6.pop();
						c5.push(l);//l为k的闭包中的状态
						if (n[i][l] != 2) {
							if (n[i][l] == 0)//若状态i经过0也可以到达l
								n[i][l] = 2;
							else //若状态i经过0不可以到达l
								n[i][l] = 1;
						}

					}

				}
			}

		}

	}




	int* r = new int[sizeM];
	for (int i = 0; i < sizeM; i++)
		r[i] = 0;//初始化r

	int m, p1, p2, p3, c = 0, sizeG = sizeM;
	for (m = 0; m < sizeM; m++) {
		for (p1 = 0; p1 < sizeM; p1++) {
			if (n[m][p1] == -1)
				c++;
			if (n[p1][m] == -1)
				c++;
		}
		if (c == 2 * sizeM)
		{
			r[m] = 1; sizeG--;
		}
		c = 0;

	}

	int** w;//定义和初始化二维数组k

	{w = new int* [sizeG];
	for (int i = 0; i < sizeG; i++) {
		w[i] = new int[sizeG];
	}
	for (int i = 0; i < sizeG; i++) {
		for (int j = 0; j < sizeG; j++) {
			w[i][j] = -1;
		}
	}
	}

	queue<int> s;

	for (p2 = 0; p2 < sizeM; p2++) {
		for (p3 = 0; p3 < sizeM; p3++) {
			if (r[p2] == 0 && r[p3] == 0) {
				s.push(n[p2][p3]);
			}
		}
	}


	for (p2 = 0; p2 < sizeG; p2++) {
		for (p3 = 0; p3 < sizeG; p3++) {
			w[p2][p3] = s.front();
			s.pop();
		}
	}
	queue<int>co;

	int ca, cb = sizeM - 1;
	for (i = 0; i < sizeM * sizeM; i++) {
		for (ca = 0; ca < sizeM; ca++) {
			if (!co.empty()) {
				cb = co.front();
				co.pop();
			}
			if (p[ca][cb] == 3)//若状态i到j可以通过ε到达
			{
				co.push(ca);
				if (ca == 0) {
					int* fstate = new int[2];
					fstate[0] = 0;
					fstate[1] = sizeG - 1;
					nfa1.fstates = fstate;
					nfa1.statesize = sizeG;
					nfa1.finalsize = 2;

				}
				else
				{
					int* fstate = new int[1];
					fstate[0] = sizeG - 1;
					nfa1.fstates = fstate;
					nfa1.statesize = sizeG;
					nfa1.finalsize = 1;
				};
			}
		}


	}
	if (nfa1.finalsize != 2) {
		int* fstate = new int[1];
		fstate[0] = sizeG - 1;
		nfa1.fstates = fstate;
		nfa1.statesize = sizeG;
		nfa1.finalsize = 1;
	}


	return w;
}