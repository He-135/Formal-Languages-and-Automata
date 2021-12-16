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


	int** n;//����ͳ�ʼ����ά����n
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

	int i, j, k, l, z, x;//����ѭ���õ��ı���

	//���ڦ�-NFA��,״̬i�����ŵ���j,j����0����k,k�����ŵ���l,���ڶ�Ӧ��NFA��,״̬i����0����l
	//��������е�Ԫ��ʼ��Ϊ5;������ͨ��0������Ϊ0��������ͨ��1������Ϊ1��
	//��ͨ��0 1�����Ե�����Ϊ2�������Ԧ�ͨ��������Ϊ#��
	//�����������Ѿ���REת������-NFA��ɾȥ
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
			if (p[i][j] == 3 || i == j)//��״̬i��j����ͨ���ŵ���
			{
				c1.push(j);//jΪiͨ��һ������

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
			c1.push(j);//jΪi�ıհ��е�״̬
			for (k = 0; k < sizeM; k++) {
				if (p[j][k] == 0)//��״̬j��k����ͨ��0����
				{
					for (l = 0; l < sizeM; l++) {//��״̬k��l����ͨ���ŵ���
						if (p[k][l] == 3 || k == l)//��״̬k��l����ͨ���ŵ���
						{
							c3.push(l);//lΪkͨ��һ������

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
						c3.push(l);//lΪk�ıհ��е�״̬
						if (n[i][l] != 2) {
							if (n[i][l] == 1)//��״̬i����1Ҳ���Ե���l
								n[i][l] = 2;
							else //��״̬i����1�����Ե���l
								n[i][l] = 0;
						}

					}


				}
				else if (p[j][k] == 1)//��״̬j��k����ͨ��0��1����
				{
					for (l = 0; l < sizeM; l++) {//��״̬k��l����ͨ���ŵ���

						if (p[k][l] == 3 || k == l)//��״̬k��l����ͨ���ŵ���
						{
							c5.push(l);//lΪkͨ��һ������

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
						c5.push(l);//lΪk�ıհ��е�״̬
						if (n[i][l] != 2) {
							if (n[i][l] == 0)//��״̬i����0Ҳ���Ե���l
								n[i][l] = 2;
							else //��״̬i����0�����Ե���l
								n[i][l] = 1;
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


	return w;
}