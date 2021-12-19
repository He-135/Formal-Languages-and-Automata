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
				insert(l, p, l->len);//��һ·�Ͼ���������״̬���Ž�������
				eclose(i, p, sizeM, e, ++c, l);
			}
		}
	}

	return;
}




int** IPSLNFAtoNFA(int** p, nfa& nfa1) {


	int** n; int** e;//����ͳ�ʼ����ά����n,e���ÿ��״̬�ıհ�
	int* b1, * b2;//���i k�ܵ��������״̬

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


	int i, j, k, l, z, x;//����ѭ���õ��ı���

	for (i = 0; i < sizeM; i++) //�����ÿ��״̬�ıհ�
	{
		for (j = 0; j < sizeM; j++)
		{
			if (p[i][j] == 3 || i == j)//�������ͨ����ת�Ƶ���
			{
				e[i][j] = 1;//������Ϊ-1
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
	//���ڦ�-NFA��,״̬i�����ŵ���j,j����0����k,k�����ŵ���l,���ڶ�Ӧ��NFA��,״̬i����0����l
	//��������е�Ԫ��ʼ��Ϊ5;������ͨ��0������Ϊ0��������ͨ��1������Ϊ1��
	//��ͨ��0 1�����Ե�����Ϊ2�������Ԧ�ͨ��������Ϊ#��
	//�����������Ѿ���REת������-NFA��ɾȥ


	for (i = 0; i < sizeM; i++) {


		for (j = 0; j < sizeM; j++) {
			if (e[i][j] == 1)//��״̬i��j����ͨ���ŵ���

				for (k = 0; k < sizeM; k++) {
					if (p[j][k] == 0 || p[j][k] == 1)//��״̬j��k����ͨ��0����
					{
						for (l = 0; l < sizeM; l++) {//��״̬k��l����ͨ���ŵ���
							if (e[k][l] == 1)//��״̬k��l����ͨ���ŵ���
							{

								if (p[j][k] == 0) {
									if (n[i][l] != 2) {
										if (n[i][l] == 1)
											n[i][l] = 2;
										else //��״̬i����1�����Ե���l
											n[i][l] = 0;
									}
								}
								else if (p[j][k] == 1) {
									if (n[i][l] != 2) {
										if (n[i][l] == 0)
											n[i][l] = 2;
										else //��״̬i����1�����Ե���l
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
		r[i] = 0;//��ʼ��r

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

	int** w;//����ͳ�ʼ����ά����k

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
			if (p[ca][cb] == 3)//��״̬i��j����ͨ���ŵ���
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