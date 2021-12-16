#include "4dfa_mindfa.h"

int main() {
	queue<int>mainn;

	int** p;
	int** p2;
	p = NFAtoMatrix();
	nfa nfa1;

	p2 = IPSLNFAtoNFA(p, nfa1);
	int length = p[0][0];
	for (int i = 0; i < length; i++) {
		delete[] p[i];
	}
	p2 = NTD(p2, nfa1.statesize, nfa1.fstates, nfa1.finalsize);

	vector<int> minimizeEnd;
	int countMinimize = 0;              //    接受状态集    总状态数        接收状态数
	int** minimizeDFA = DFA_Minimize(p2, nfa1.fstates, nfa1.statesize, nfa1.finalsize, minimizeEnd, countMinimize);

	int temp;
	cout << "\t0\t1" << endl;
	cout << "#";
	for (int i = 0; i < countMinimize; i++) {
		if (inVec(minimizeEnd, i)) { cout << "*"; }//为接收状态
		cout << "q" << (i + 1) << "\t";
		if ((temp = transition(i, 0, minimizeDFA, countMinimize)) != -1) {
			cout << "q" << (temp + 1) << "\t";
		}
		else {
			cout << "N\t";
		}
		if ((temp = transition(i, 1, minimizeDFA, countMinimize)) != -1) {
			cout << "q" << (temp + 1) << "\t";
		}
		else {
			cout << "N\t";
		}
		cout << endl;
	}
	cout << endl;

	int judge = 0;
	cout << "是否要输出正则文法?  按0不输出，按1输出";
	cin >> judge;

	if (judge == 1) {
		//输出正则方法
		for (int i = 0; i < countMinimize; i++) {
			bool write = false;
			cout << (char)('A' + i) << " > ";
			int re0 = transition(i, 0, minimizeDFA, countMinimize);
			int re1 = transition(i, 1, minimizeDFA, countMinimize);
			if (re0 != -1) {
				if (write) { cout << " | "; }
				cout << "0" << (char)('A' + re0);
				write = true;
				if (inVec(minimizeEnd, re0)) {
					cout << " | ";
					cout << "0";
				}
			}
			if (re1 != -1) {
				if (write) { cout << " | "; }
				cout << "1" << (char)('A' + re1);
				write = true;
				if (inVec(minimizeEnd, re1)) {
					cout << " | ";
					cout << "1";
				}
			}
			cout << endl;
		}
	}

	if (!nfa1.fstates) {
		for (int i = 0; i < nfa1.statesize; i++) {
			delete[](nfa1.fstates);
		}
	}
	if (!minimizeDFA) {
		for (int i = 0; i < countMinimize; i++) {
			delete[] minimizeDFA[i];
		}
	}
	return 0;
}