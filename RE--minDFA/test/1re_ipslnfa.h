#pragma once

#include <iostream>
#include <stdio.h>
#include <vector>
#include <stack>
#include <queue>
#include <array>
#include <unordered_map>
#include <cassert>
using namespace std;
typedef enum { OK, ERROR }status;

class IPSLNFA {
public:
	vector<unordered_map<char, vector<int>>> G;
	int start, end;
	IPSLNFA() {
		G.resize(1);
		start = end = 0;
	}
	size_t size()const {
		return G.size();
	}
	unordered_map<char, vector<int>>& operator[](int n) {
		assert(n < G.size());//断言
		return G[n];
	}
	IPSLNFA& operator+=(IPSLNFA n) {
		int offset = size();
		for (int i = 0; i < n.size(); ++i) {
			G.push_back(n[i]);
			for (auto& j : G.back()) {
				for (auto& v : j.second) {
					v += offset;
				}
			}
		}
		return *this;
	}
};
int pri(char ch) {
	int level = 0; // 优先级
	switch (ch) {
	case '(': {
		level = 1;
		break; }
	case '+': {
		level = 2;
		break; }
	case '&': {
		level = 3;
		break; }
	case '*': {
		level = 4;
		break; }
	default:
		break;
	}
	return level;
}
bool isOperator(char c) {
	switch (c) {
	case '*':
	case '+':
	case '&':
		return true;
	default:
		return false;
	}
}
vector<char> toSuffix(const vector<char>& expr) {
	stack<char> op;
	vector<char> suffix;
	for (const auto& c : expr) {
		if (isOperator(c)) {
			if (op.empty()) {
				op.emplace(c);
			}
			else {
				while (!op.empty()) {
					int t = op.top();
					if (pri(c) <= pri(t)) {
						op.pop();
						suffix.push_back(t);
					}
					else {
						break;
					}
				}
				op.emplace(c);
			}
		}
		else {
			if (c == '(') {
				op.emplace(c);
			}
			else if (c == ')') {
				while (op.top() != '(') {
					suffix.push_back(op.top());
					op.pop();
				}
				op.pop();
			}
			else {
				suffix.push_back(c);
			}
		}
	}
	while (!op.empty()) {
		suffix.push_back(op.top());
		op.pop();
	}
	return suffix;
}
vector<char> IniRE() {
	cout << "请输入正则表达式:";
	string str;
	vector<char> v;
	vector<char> v1;
	cin >> str;
	for (int i = 0; i < str.size(); i++) {
		if ((str[i] == '0' || str[i] == '1' || str[i] == '*' || str[i] == ')')
			&& (str[i + 1] == '0' || str[i + 1] == '1' || str[i + 1] == '(')) {
			v.push_back(str[i]);
			v.push_back('&');
		}
		else {
			v.push_back(str[i]);
		}
	}
	v1 = toSuffix(v);
	for (char c : v1) {
		//cout << c;
	}
	cout << endl;
	return v1;
}
IPSLNFA buildIPSLNFA() {
	IPSLNFA nfa;
	stack<IPSLNFA> stk;
	vector<char> reg = IniRE();
	for (auto& c : reg) {
		if (!isOperator(c)) {
			IPSLNFA a;
			a.G.resize(2);
			a.end = 1;
			a[0][c] = { a.end };
			stk.emplace(a);
		}
		else {
			switch (c) {
			case'+': {
				IPSLNFA tmp;
				IPSLNFA b = stk.top();
				stk.pop();
				IPSLNFA a = stk.top();
				stk.pop();
				size_t size1 = tmp.size();
				tmp += a;
				tmp[tmp.start]['#'].emplace_back(a.start + size1);
				size_t size2 = tmp.size();
				tmp += b;
				tmp[tmp.start]['#'].emplace_back(b.start + size2);
				tmp[a.end + size1]['#'] = { int(tmp.size()) };
				tmp[b.end + size2]['#'] = { int(tmp.size()) };
				tmp.end = tmp.size();
				tmp.G.emplace_back(unordered_map<char, vector<int>>());
				stk.emplace(tmp);
				break;
			}
			case'*': {
				IPSLNFA tmp;
				IPSLNFA a = stk.top();
				stk.pop();
				size_t size1 = tmp.size();
				tmp += a;
				tmp[tmp.start]['#'].emplace_back(a.start + size1);
				tmp[a.end + size1]['#'].emplace_back(a.start + size1);
				size_t s = tmp.size();
				tmp[a.end + size1]['#'].emplace_back(s);
				tmp.end = s;
				tmp.G.emplace_back(unordered_map<char, vector<int>>());
				tmp[tmp.start]['#'].emplace_back(s);
				stk.emplace(tmp);
				break;
			}
			case'&': {
				IPSLNFA b = stk.top();
				stk.pop();
				IPSLNFA a = stk.top();
				stk.pop();
				a.G.pop_back();
				size_t s = a.size();
				a += b;
				a.end = b.end + s;
				stk.emplace(a);//每次新产生的子NFA都放入栈中
				break;
			}
			default:
				break;
			}
		}
	}
	nfa = stk.top();
	return nfa;
}
int** NFAtoMatrix() {
	IPSLNFA NFA = buildIPSLNFA();

	int sizeM = NFA.size();
	int** p;
	{p = new int* [sizeM];
	for (int i = 0; i < sizeM; i++) {
		p[i] = new int[sizeM];
	}
	for (int i = 0; i < sizeM; i++) {
		for (int j = 0; j < sizeM; j++) {
			p[i][j] = 5;
		}
	}
	}
	for (int i = 0; i < NFA.size(); i++) {
		auto iter = NFA.G[i].begin();
		while (iter != NFA.G[i].end()) {
			char ch = iter->first;
			if (ch == '#') {
				for (int j : iter->second) {
					p[i][j] = 3;
				}
			}
			else if (ch == '0') {
				for (int j : iter->second) {
					p[i][j] = 0;
				}
			}
			else if (ch == '1') {
				for (int j : iter->second) {
					p[i][j] = 1;
				}
			}
			++iter;
		}
	}
	p[0][0] = sizeM;
	cout << endl;
	cout << "\t";
	for (int i = 0; i < sizeM; i++) {
		if (i < 10) { cout << i << "  "; }
		else if (i >= 10) { cout << i << " "; }
	}
	cout << endl;
	for (int i = 0; i < sizeM; i++) {
		cout << i << "\t";
		for (int j = 0; j < sizeM; j++) {
			if (p[i][j] == 5)
				cout << "   ";
			else
				cout << p[i][j] << "  ";
		}
		cout << endl;
	}
	return p;
}


