#pragma once
#include"Utilize.h"
#include "token.h"
using namespace std;
struct production {
	//这个结构用来存放产生式
	string nonTerminalSym;//非终结符，左部
	vector<string> genera;//产生式右部
};


void init();
void saveProduction(string grammarName);
void getFirstSet();
void getProductionFirstset();
void getPredictTable();
void analyse();
