#include <iostream>
#include <vector>
using namespace std;


class List{//状态关联表
public:
	int q1, q2;
	List* next;
	List() = default;
	List(int t1, int t2){//存储时要求q1>q2
		if(t1 > t2){
			q1 = t1;
			q2 = t2;
		}
		else{
			q1 = t2;
			q2 = t1;
		}
	}
	bool inList(int t1, int t2){
		if(t1 < t2){//保证t1>t2
			int t = t1;
			t1 = t2;
			t2 = t;
		}
		List* temp = this;
		while(temp != nullptr){
			if (temp->q1 == t1 && temp->q2 == t2) { return true; }
			temp = temp->next;
		}
		return false;
	}
};

int** DFA_Minimize(int** matrix, int end[], int count, int count_end, vector<int>& minimizeEnd, int& countMinimize);
bool isEnd(int q, int end[], int count_end);
int transition(int q, int ch, int** matrix, int count);//返回状态q接收ch后所到达的状态
void tagRelevance(vector<List*>&v, int q1, int q2, int**tag);
void save(vector<vector<int>>&result, int q1, int q2);
bool inVec(vector<int>result, int temp);
int main(void){
	int count = 6;//状态个数

	int** matrix = new int*[count];
	for(int i = 0; i < count; i++){
		matrix[i] = new int[count];
		for(int j = 0; j < count; j++){
			matrix[i][j] = -1;//初始化为-1，-1表示i和j之间无边
		}
	}
	matrix[0][1] = 0;
	matrix[1][0] = 0;
	matrix[0][2] = 1;
	matrix[1][3] = 1;
	matrix[2][4] = 0;
	matrix[3][5] = 1;
	matrix[2][5] = 1;
	matrix[3][4] = 0;
	matrix[4][5] = 1;
	matrix[4][4] = 0;
	matrix[5][5] = 2;
	int end[] = { 2, 3, 4 };
	vector<int> minimizeEnd;
	int countMinimize = 0;
	int** minimizeDFA = DFA_Minimize(matrix, end, count, sizeof(end) / sizeof(int), minimizeEnd, countMinimize);
	//输出
	int temp;
	cout << "\t0\t1" << endl;
	cout << "#";	
	for(int i = 0; i < countMinimize; i++){
		if (inVec(minimizeEnd, i)) { cout << "*"; }//为接收状态
		cout << "q" << (i+1) << ":\t";
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
	//输出正则方法
	for(int i = 0; i < countMinimize; i++){
		bool write = false;
		cout << (char)('A' + i) << " > ";
		int re0 = transition(i, 0, minimizeDFA, countMinimize);
		int re1 = transition(i, 1, minimizeDFA, countMinimize);
		if(re0 != -1){
			if (write) { cout << " | "; }
			cout << "0" << (char)('A' + re0);
			write = true;
			if(inVec(minimizeEnd, re0)){
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
	if(!matrix){
		for(int i = 0; i < count; i++){
			delete[]matrix[i];
		}
	}
	if (!minimizeDFA) {
		for (int i = 0; i < countMinimize; i++) {
			delete[]minimizeDFA[i];
		}
	}
	return 0;
}

int** DFA_Minimize(int** matrix, int end[], int count, int count_end, vector<int>& minimizeEnd, int& countMinimize){
	//先删除不可达状态，某一列均为-1则为不可达状态
	for(int i = 0; i < count; i++){//i表示第i列
		bool reach = false;
		for(int j = 0; j < count; j++){//j表示第j行
			if(i != j && matrix[j][i] != -1){//说明第i个状态可达,继续判断第i+1列（第i+1个状态）
				reach = true;
				break;
			}
		}
		if(!reach){//如果该状态不可达，则可删除所有以状态i为起始的边
			for(int k = 0; k < count; k++){
				if(matrix[i][k] != -1){//存在以状态i为起始的边
					matrix[i][k] = -1;
				}
			}
		}
	}
	int** tag = new int* [count];//用于标记两状态是否可区分
	for (int i = 0; i < count; i++) {
		tag[i] = new int[count];
	}
	
	//0为可区分，1为不可区分，初始化均为0（0表示未标记）
	for(int i = 1; i < count; i++){//每一列从q1开始，每一行从q0开始
		for(int j = 0; j < i; j++){
			tag[i][j] = 0;
		}
	}
	//与终止状态有关的状态对一定是不可区分的
	for(int i = 0; i < count_end; i++){
		//以下j表示行，k表示列
		for(int j = end[i], k = 0; k < end[i]; k++){//把第end[i]行的都标记上
			if(tag[j][k] == 0 && !isEnd(k, end, count_end)){//(qk, qj)未标记且qk不是接收状态
				tag[j][k] = 1;
			}
		}
		for(int k = end[i], j = end[i] + 1; j < count; j++){//把第end[i]列的都标记上
			if(tag[j][k] == 0 && !isEnd(j, end, count_end)){//(qk, qj)未标记且qj不是接收状态
				tag[j][k] = 1;
			}
		}
	}
	//将未标记的判断是否要标记
	//以下j表示行，k表示列
	vector<List*>relevance;//存储状态关联表
	for(int k = 0; k < count - 1; k++){
		for(int j = k + 1; j < count; j++){
			if(tag[j][k] == 0){//(qk, qj)未标记
				int temp1 = transition(j, 0, matrix, count);//获得k,j接收0所到达的状态
				int temp2 = transition(k, 0, matrix, count);
				if (temp1 < temp2) {//保证temp1>temp2
					int t = temp1;
					temp1 = temp2;
					temp2 = t;
				}
				if (temp1 != temp2 && (temp1 != j || temp2 != k) && tag[temp1][temp2] == 1){//转化到可区分状态
					tag[j][k] = 1;
					//要将(qk, qj)所在状态关联表上的所有状态标记
					tagRelevance(relevance, j, k, tag);
				}
				else{
					if (temp1 != temp2 && (temp1 != j || temp2 != k) && tag[temp1][temp2] == 0) { //转化到的状态未标记
					  //放到状态关联表上并存入relevance数组
						List* head = new List(j, k);
						head->next = new List(temp1, temp2);
						relevance.push_back(head);
					}
					//获得k,j接收1所到达的状态
					temp1 = transition(j, 1, matrix, count);
					temp2 = transition(k, 1, matrix, count);
					if (temp1 < temp2) {//保证temp1>temp2
						int t = temp1;
						temp1 = temp2;
						temp2 = t;
					}
					if (temp1 != temp2 && (temp1 != j || temp2 != k) && tag[temp1][temp2] == 1) {
						tag[j][k] = 1;
						//要将(qk, qj)所在状态关联表上的所有状态标记
						tagRelevance(relevance, j, k, tag);
					}
					else{
						if (temp1 != temp2 && (temp1 != j || temp2 != k) && tag[temp1][temp2] == 0) {//转化到的状态未标记
						//放到状态关联表上并存入relevance数组
							List* head = new List(j, k);
							head->next = new List(temp1, temp2);
							relevance.push_back(head);
						}
					}
				}
			}
		}
	}
	//统计可以合并的状态
	vector<vector<int>>result;
	bool* check = new bool[count];
	for (int i = 0; i < count; i++) {
		check[i] = false;
	}
	for (int k = 0; k < count - 1; k++) {
		for (int j = k + 1; j < count; j++) {
			if(tag[j][k] == 0){
				check[j] = true;
				check[k] = true;
				save(result, j, k);
			}
		}
	}
	for(int i = 0; i < count; i++){
		if(!check[i]){
			vector<int> temp;
			temp.push_back(i);
			result.push_back(temp);
		}
	}
	if(result.size() == 0){//无可合并状态
		countMinimize = count;
		for (int i = 0; i < count_end; i++) { minimizeEnd.push_back(end[i]); }
		return matrix;
	}
	else{
		//将不可区分的状态合并后生成最小化DFA
		countMinimize = result.size();
		for(int i = 0; i < count_end; i++){
			for(int j = 0; j < result.size(); j++){
				if(inVec(result[j], end[i]) && !inVec(minimizeEnd, j)){
					minimizeEnd.push_back(j);
					break;
				}
			}
		}
		int** matrix_new = new int* [result.size()];
		for (int i = 0; i < result.size(); i++) {
			matrix_new[i] = new int[result.size()];
			for (int j = 0; j < result.size(); j++) {
				matrix_new[i][j] = -1;//初始化为-1，-1表示i和j之间无边
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
}

bool isEnd(int q, int end[], int count_end){//使用二分法进行判断一个状态是否是接收状态
	int low = 0;
	int high = count_end - 1;
	int middle;
	while(low <= high){
		middle = (low + high) / 2;
		if (end[middle] == q) { return true; }
		else if (end[middle] > q) { high = middle - 1; }
		else if (end[middle] < q) { low = middle + 1; }
	}
	return false;
}

int transition(int q, int ch, int** matrix, int count){
	for(int k = 0; k < count; k++){
		if (matrix[q][k] == 2) { return k; }//状态q接收0和1都能转移到状态k;
		else if (matrix[q][k] == ch) { return k; }//状态q接收ch能转移到状态k;
	}
	return -1;//状态q不接收ch
}

void tagRelevance(vector<List*>&v, int q1, int q2, int**tag){
	vector<List*>::iterator it = v.begin();
	for(int i = 0; i < v.size(); i++){
		if(v[i]->inList(q1, q2)){//状态q1和q2在状态关联表上
			List* temp = v[i];
			while(temp != NULL){
				tag[temp->q1][temp->q2] = 1;
				temp = temp->next;
			}
			it = v.erase(it);
		}
		else { it++; }
	}
}

void save(vector<vector<int>>&result, int q1, int q2){

	for (int i = 0; i < result.size(); i++) {
		if (inVec(result[i], q1)) {//判断q1是否已经在vector中
			if (inVec(result[i], q2)) { return; }
			result[i].push_back(q2);
			return;
		}
		else if (inVec(result[i], q2)) {//前面得到q1不在vector中，这里判断q2是否在vector中
			result[i].push_back(q1);
			return;
		}
		for (int j = 0; j < result[i].size(); j++) {
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
		}
	}
	vector<int> temp;
	temp.push_back(q1);
	temp.push_back(q2);
	result.push_back(temp);
}

bool inVec(vector<int>result, int temp){
	for(int i = 0; i < result.size(); i++){
		if (result[i] == temp) { return true; }
	}
	return false;
}