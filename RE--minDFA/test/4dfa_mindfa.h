#pragma once

#include"3nfa_dfa.h"

class List {//״̬������
public:
	int q1, q2;
	List* next;
	List() = default;
	List(int t1, int t2) {//�洢ʱҪ��q1>q2
		if (t1 > t2) {
			q1 = t1;
			q2 = t2;
		}
		else {
			q1 = t2;
			q2 = t1;
		}
	}
	bool inList(int t1, int t2) {
		if (t1 < t2) {//��֤t1>t2
			int t = t1;
			t1 = t2;
			t2 = t;
		}
		List* temp = this;
		while (temp != nullptr) {
			if (temp->q1 == t1 && temp->q2 == t2) { return true; }
			temp = temp->next;
		}
		return false;
	}
};

int** DFA_Minimize(int** matrix, int end[], int count, int count_end, vector<int>& minimizeEnd, int& countMinimize);
bool isEnd(int q, int end[], int count_end);
int transition(int q, int ch, int** matrix, int count);//����״̬q����ch���������״̬
void tagRelevance(vector<List*>& v, int q1, int q2, int** tag);
//void tagRelevance(vector<List*>&v, int q1, int q2, int tag[][10]);

void save(vector<vector<int>>& result, int q1, int q2);
void save(vector<vector<int>>& result, int q);
bool inVec(vector<int>result, int temp);

int** DFA_Minimize(int** matrix, int end[], int count, int count_end, vector<int>& minimizeEnd, int& countMinimize) {
	//��ɾ�����ɴ�״̬��ĳһ�о�Ϊ-1��Ϊ���ɴ�״̬
	for (int i = 1; i < count; i++) {//i��ʾ��i�У���i=1��ʼ����Ϊ��ʼ״̬q0��Զ�ɴ�
		bool reach = false;
		for (int j = 0; j < count; j++) {//j��ʾ��j��
			if (i != j && matrix[j][i] != -1) {//˵����i��״̬�ɴ�,�����жϵ�i+1�У���i+1��״̬��
				reach = true;
				break;
			}
		}
		if (!reach && i != 0) {//�����״̬���ɴ���ɾ��������״̬iΪ��ʼ�ıߣ�ע�⣺������ʼ״̬����ɾ����
			for (int k = 0; k < count; k++) {
				if (matrix[i][k] != -1) {//������״̬iΪ��ʼ�ı�
					matrix[i][k] = -1;
				}
			}
			int temp = i;
			//��i��֮���ÿһ����Ҫ������
			for (; i + 1 < count; i++) {
				for (int k = 0; k < count; k++) {
					matrix[i][k] = matrix[i + 1][k];
				}
			}
			//��i���ұߵ�ÿһ����Ҫ������
			for (i = temp; i + 1 < count; i++) {
				for (int k = 0; k < count; k++) {
					matrix[k][i] = matrix[k][i + 1];
				}
			}
			count--;
			i = 0;
		}
	}

	int** tag = new int* [count];//���ڱ����״̬�Ƿ������
	for (int i = 0; i < count; i++) {
		tag[i] = new int[count];
	}
	//int tag[10][10];
	//0Ϊ�����֣�1Ϊ�������֣���ʼ����Ϊ0��0��ʾδ��ǣ�
	for (int i = 1; i < count; i++) {//ÿһ�д�q1��ʼ��ÿһ�д�q0��ʼ
		for (int j = 0; j < i; j++) {
			tag[i][j] = 0;
		}
	}
	//����ֹ״̬�йص�״̬��һ���ǲ������ֵ�
	for (int i = 0; i < count_end; i++) {
		//����j��ʾ�У�k��ʾ��
		for (int j = end[i], k = 0; k < end[i]; k++) {//�ѵ�end[i]�еĶ������
			if (tag[j][k] == 0 && !isEnd(k, end, count_end)) {//(qk, qj)δ�����qk���ǽ���״̬
				tag[j][k] = 1;
			}
		}
		for (int k = end[i], j = end[i] + 1; j < count; j++) {//�ѵ�end[i]�еĶ������
			if (tag[j][k] == 0 && !isEnd(j, end, count_end)) {//(qk, qj)δ�����qj���ǽ���״̬
				tag[j][k] = 1;
			}
		}
	}
	//��δ��ǵ��ж��Ƿ�Ҫ���
	//����j��ʾ�У�k��ʾ��
	vector<List*>relevance;//�洢״̬������
	for (int k = 0; k < count - 1; k++) {
		for (int j = k + 1; j < count; j++) {
			if (tag[j][k] == 0) {//(qk, qj)δ���
				int temp1 = transition(j, 0, matrix, count);//���k,j����0�������״̬
				int temp2 = transition(k, 0, matrix, count);
				if (temp1 < temp2) {//��֤temp1>temp2
					int t = temp1;
					temp1 = temp2;
					temp2 = t;
				}
				if ((temp1 != temp2 && (temp1 != j || temp2 != k) && tag[temp1][temp2] == 1) || ((temp1 == -1 || temp2 == -1) && temp1 != temp2)) {//ת����������״̬
					tag[j][k] = 1;
					//Ҫ��(qk, qj)����״̬�������ϵ�����״̬���
					tagRelevance(relevance, j, k, tag);
				}
				else {
					if (temp1 != temp2 && (temp1 != j || temp2 != k) && tag[temp1][temp2] == 0) { //ת������״̬δ���
						//�ŵ�״̬�������ϲ�����relevance����
						List* head = new List(j, k);
						head->next = new List(temp1, temp2);
						relevance.push_back(head);
					}
					//���k,j����1�������״̬
					temp1 = transition(j, 1, matrix, count);
					temp2 = transition(k, 1, matrix, count);

					if (temp1 < temp2) {//��֤temp1>temp2
						int t = temp1;
						temp1 = temp2;
						temp2 = t;
					}
					if ((temp1 != temp2 && (temp1 != j || temp2 != k) && tag[temp1][temp2] == 1) || ((temp1 == -1 || temp2 == -1) && temp1 != temp2)) {
						tag[j][k] = 1;
						//Ҫ��(qk, qj)����״̬�������ϵ�����״̬���
						tagRelevance(relevance, j, k, tag);
					}
					else {
						if (temp1 != temp2 && (temp1 != j || temp2 != k) && tag[temp1][temp2] == 0) {//ת������״̬δ���
						//�ŵ�״̬�������ϲ�����relevance����
							List* head = new List(j, k);
							head->next = new List(temp1, temp2);
							relevance.push_back(head);
						}
					}
				}
			}
		}
	}
	//ͳ�ƿ��Ժϲ���״̬
	vector<vector<int>>result;
	int* check = new int[count];
	for (int i = 0; i < count; i++) {
		check[i] = -1;
	}
	for (int k = 0; k < count - 1; k++) {
		for (int j = k + 1; j < count; j++) {
			if (tag[j][k] == 0) {//��ǿɺϲ���״̬
				check[j] = k;
				check[k] = j;
			}
		}
	}

	for (int i = 0; i < count; i++) {
		if (check[i] != -1) {
			save(result, check[i], i);
		}
		else {
			save(result, i);
		}
	}

	//���������ֵ�״̬�ϲ���������С��DFA
	countMinimize = result.size();
	for (int i = 0; i < count_end; i++) {
		for (int j = 0; j < result.size(); j++) {
			if (inVec(result[j], end[i]) && !inVec(minimizeEnd, j)) {
				minimizeEnd.push_back(j);
				break;
			}
		}
	}
	int** matrix_new = new int* [result.size()];
	for (int i = 0; i < result.size(); i++) {
		matrix_new[i] = new int[result.size()];
		for (int j = 0; j < result.size(); j++) {
			matrix_new[i][j] = -1;//��ʼ��Ϊ-1��-1��ʾi��j֮���ޱ�
		}
	}
	for (int i = 0; i < result.size(); i++) {
		int temp = transition(result[i][0], 0, matrix, count);
		for (int j = 0; j < result.size(); j++) {
			if (inVec(result[j], temp)) {
				matrix_new[i][j] += 1;
				break;
			}
		}
		temp = transition(result[i][0], 1, matrix, count);
		for (int j = 0; j < result.size(); j++) {
			if (inVec(result[j], temp)) {
				matrix_new[i][j] += 2;
				break;
			}
		}
	}
	return matrix_new;
}

bool isEnd(int q, int end[], int count_end) {//ʹ�ö��ַ������ж�һ��״̬�Ƿ��ǽ���״̬
	int low = 0;
	int high = count_end - 1;
	int middle;
	while (low <= high) {
		middle = (low + high) / 2;
		if (end[middle] == q) { return true; }
		else if (end[middle] > q) { high = middle - 1; }
		else if (end[middle] < q) { low = middle + 1; }
	}
	return false;
}

int transition(int q, int ch, int** matrix, int count) {
	for (int k = 0; k < count; k++) {
		if (matrix[q][k] == 2) { return k; }//״̬q����0��1����ת�Ƶ�״̬k;
		else if (matrix[q][k] == ch) { return k; }//״̬q����ch��ת�Ƶ�״̬k;
	}
	return -1;//״̬q������ch
}

void tagRelevance(vector<List*>& v, int q1, int q2, int** tag) {
	//void tagRelevance(vector<List*>&v, int q1, int q2, int tag[][10]){
	for (int i = 0; i < v.size(); i++) {
		if (v[i]->inList(q1, q2)) {//״̬q1��q2��״̬��������
			List* temp = v[i];
			while (temp != NULL) {
				tag[temp->q1][temp->q2] = 1;
				/*if (temp->q1 != q1 && temp->q2 != q2) {
					tagRelevance(v, temp->q1, temp->q2, tag);
				}*/
				temp = temp->next;
			}
			v.erase(v.begin() + i);
		}
	}
}

void save(vector<vector<int>>& result, int q1, int q2) {
	for (int i = 0; i < result.size(); i++) {
		if (inVec(result[i], q1)) {//�ж�q1�Ƿ��Ѿ���vector��
			if (inVec(result[i], q2)) { return; }
			result[i].push_back(q2);
			return;
		}
		else if (inVec(result[i], q2)) {//ǰ��õ�q1����vector�У������ж�q2�Ƿ���vector��
			result[i].push_back(q1);
			return;
		}
		/*for (int j = 0; j < result[i].size(); j++) {
			if (result[i][j] == q1) {
				for (int k = 0; k < result[i].size(); k++) {
					if (result[i][k] == q2) { return; }
				}
				result[i].push_back(q2);
				return;
			}
			else if (result[i][j] == q2) {
				for (int k = 0; k < result[i].size(); k++) {
					if (result[i][k] == q1) { return; }
				}
				result[i].push_back(q1);
				return;
			}
		}*/
	}
	//�����е��˴�˵��q1��q2������vector��
	vector<int> temp;
	temp.push_back(q1);
	temp.push_back(q2);
	result.push_back(temp);
}

void save(vector<vector<int>>& result, int q) {
	for (int i = 0; i < result.size(); i++) {
		if (inVec(result[i], q)) {//�ж�q1�Ƿ��Ѿ���vector��
			return;
		}
	}
	vector<int> temp;
	temp.push_back(q);
	result.push_back(temp);
}
bool inVec(vector<int>result, int temp) {
	for (int i = 0; i < result.size(); i++) {
		if (result[i] == temp) { return true; }
	}
	return false;
}