#pragma once
#include"Utilize.h"
#include "token.h"
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
void analyse();
