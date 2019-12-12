#pragma once
#include"Utilize.h"
#include "token.h"
#define MAXTREENODE 1000
using namespace std;
struct production {
	//����ṹ������Ų���ʽ
	string nonTerminalSym;//���ս������
	vector<string> genera;//����ʽ�Ҳ�
};


void init();
void saveProduction(string grammarName);
void getFirstSet();
void getProductionFirstset();
void getPredictTable();
void getFollowSet();
void getSelectSet();
void analyse();

extern int top;
extern string startToken;
extern treeNode* st ;