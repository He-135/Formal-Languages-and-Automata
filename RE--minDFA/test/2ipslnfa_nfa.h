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


typedef struct node
{
	int data; struct node* next; int len = 0;
} node, * Linklist;
Linklist head, p, q;

void ini(Linklist& l) {
	l = (Linklist)malloc(sizeof(node));
	l->data = -1;
	l->next = NULL;
	l->len = 1;
}

status insert(Linklist& l, int e) {
	l = (Linklist)malloc(sizeof(node));
	l->data = e;
	l->next = NULL;
	l->len = 1; return OK;
}
status insert(Linklist& L, int e, int i) {
	Linklist s, p;
	int j;
	p = L; j = 0;
	while (p && j < i - 1) { p = p->next; ++j; } if (p == NULL || j > i - 1) return ERROR;
	s = (Linklist)malloc(sizeof(node)); if (!s) return ERROR;
	s->data = e;
	s->next = p->next; p->next = s;
	L->len++; return OK;
}

queue<int>t1, t2;

void eclose(int i, int& last, int sizeM, int**& e, int& c, Linklist& l) {
	if (c > sizeM) return;
	int j = 0;
	int& p = j;
	Linklist g; g = l;
	for (p; (p < sizeM); p++) {
		if ((last != p) && (e[last][p] == 1)) {
			int v = 0;
			while (g->next) {
				if (g->data == p) {
					v = 1;
					break;
				}
				g = g->next;
			}
			if (!v) {
				e[i][p] = 1;
				insert(l, p, l->len);//把一路上经过的所有状态都放进队列里
				eclose(i, p, sizeM, e, ++c, l);
			}
		}
	}

	return;
}




int** IPSLNFAtoNFA(int** p, nfa& nfa1) {


	int** n; int** e;//定义和初始化二维数组n,e存放每个状态的闭包
	int* b1, * b2;//存放i k能到达的所有状态

	int sizeM = p[0][0];

	b1 = new int[sizeM];
	b2 = new int[sizeM];
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
	{e = new int* [sizeM];
	for (int i = 0; i < sizeM; i++) {
		e[i] = new int[sizeM];
	}}
	for (int i = 0; i < sizeM; i++) {
		for (int j = 0; j < sizeM; j++) {
			e[i][j] = -1;
		}
	}


	int i, j, k, l, z, x;//后续循环用到的变量

	for (i = 0; i < sizeM; i++) //先求出每个状态的闭包
	{
		for (j = 0; j < sizeM; j++)
		{
			if (p[i][j] == 3 || i == j)//如果可以通过空转移到达
			{
				e[i][j] = 1;//不能则为-1
			}
		}
	}



	for (i = 0; i < sizeM; i++)
	{
		int t = 0;
		Linklist p;

		insert(p, i);
		eclose(i, i, sizeM, e, t, p);
	}



	cout << endl;
	for (i = 0; i < sizeM; i++) {
		for (j = 0; j < sizeM; j++) {
			cout << e[i][j] << " ";
		}
		cout << endl;
	}
	//若在ε-NFA中,状态i经过ε到达j,j经过0到达k,k经过ε到达l,则在对应的NFA中,状态i经过0到达l
	//矩阵的所有单元初始化为5;若可以通过0到达，则记为0；若可以通过1到达，则记为1；
	//若通过0 1都可以到达，则记为2；若可以ε通过到达，则记为#；
	//其他组合情况已经在RE转换到ε-NFA中删去


	for (i = 0; i < sizeM; i++) {


		for (j = 0; j < sizeM; j++) {
			if (e[i][j] == 1)//若状态i到j可以通过ε到达

				for (k = 0; k < sizeM; k++) {
					if (p[j][k] == 0 || p[j][k] == 1)//若状态j到k可以通过0到达
					{
						for (l = 0; l < sizeM; l++) {//若状态k到l可以通过ε到达
							if (e[k][l] == 1)//若状态k到l可以通过ε到达
							{

								if (p[j][k] == 0) {
									if (n[i][l] != 2) {
										if (n[i][l] == 1)
											n[i][l] = 2;
										else //若状态i经过1不可以到达l
											n[i][l] = 0;
									}
								}
								else if (p[j][k] == 1) {
									if (n[i][l] != 2) {
										if (n[i][l] == 0)
											n[i][l] = 2;
										else //若状态i经过1不可以到达l
											n[i][l] = 1;
									}
								}


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
	cout << endl;
	cout << "\t";
	for (int i = 0; i < sizeG; i++) {
		if (i < 10) { cout << i << "  "; }
		else if (i >= 10) { cout << i << " "; }
	}
	cout << endl;
	for (int i = 0; i < sizeG; i++) {
		cout << i << "\t";
		for (int j = 0; j < sizeG; j++) {
			cout << w[i][j] << "  ";
		}
		cout << endl;
	}

	return w;
}