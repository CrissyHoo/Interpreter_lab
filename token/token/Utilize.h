#pragma once
//���������һЩͨ�õĺ���
#include <vector>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <map>
#include <stack>
using namespace std;
typedef enum {

	INIT,//Ĭ�ϳ�ʼֵ
	//�쳣
	ERROR,

	//�ؼ���
	IF,   //if
	ELSE, //else
	WHILE,  //while
	INT,   //int
	REAL,   //float

	//����
	INT_NUM,
	REAL_NUM,

	//��ʶ��
	ID,

	//�����
	PLUS,       //+
	MINUS,      //-
	MUL,        //*
	DIV,        // /
	MOD,        // % 
	ASSIGN, LES, LES_EQU, GRT, GRT_EQU, NOT_EQU, EQU,//=,<,<=,>,>=,<>,==

	//���޷�
	LEFT_BRA, RIGHT_BRA,   //     (, )
	LEFT_INDEX, RIGHT_INDEX,  //  [, ]
	LEFT_BOUND, RIGHT_BOUND,//    {, }
	POINTE, COMMA, SEMI, SIN_QUE, DOU_QUE, //. , ; ' "


	//ע��

	LINE_NOTE,  //  //
	MULTI_NOTE  //  /**/

}tokenType;
//Ϊ�﷨�������õĽṹ���洢ÿ��token����Ϣ
struct tokenInfo {
	string type;
	string content;
	int line;//token����λ��

	//��ʼ������
	tokenInfo() {
		type = " ";
		content = " ";
		line = -1;
	}
};

struct errorNode {          //����ڵ㣬������֯�ɴ�������
	string content;         //����
	int line;               //��������
};

typedef enum {
	//DFA״̬��
	START,
	INNUM,  //һ������
	INID,   //��ʶ��	

	//�����
	INPLUS,
	INMIN,
	INMUL,
	INDIV,
	INMOD,
	INASSIGN,
	INLES,
	INGRT,

	//ʵ��
	INREAL0,
	INREAL,

	//��ϵ��
	INSPECIAL,

	//����ע��
	INLINECOM,
	INMULCOM1,
	INMULCOM2,
	DONE

}stateType;

//�ָ��
list<string> split(string &str, const string &pattern);
void printToken(tokenType token, const char tokenString[]);
void saveNoneTerminal(set<string> noneTerminalSymbols, ofstream &out);
void saveTerminal(set<string> terminalSymbols, ofstream &out);
void saveFirst(map<string, set<string>> firstSet, ofstream &out);
void saveProductionFirst(map<int, set<string>> productionFirstSet, ofstream &out);
void saveFollow(map<string, set<string>> followSet, ofstream &out);
void saveProductionSelect(map<int, set<string>> selectSet, ofstream &out);
void savePredictionTable(map<string, map<string, string>> predictionTable, ofstream &out);

extern ofstream token_ana;
extern list<tokenInfo> resultTok;
extern int line;
extern stateType state;
extern bool hasError;
extern list<errorNode> err;
extern errorNode errt;
extern string totalProductions[100];
