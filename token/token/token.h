#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include <list>
#include "Utilize.h"
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
static tokenType reservedLookup(char *s);
bool isLetter(char c);
bool isDigit(char c);
static tokenType reservedLookup(char *s);
void printToken(tokenType token, const char tokenString[]);

void getToken(string ss);


extern list<tokenInfo> resultTok;
extern int line;
extern ofstream token_ana;
extern stateType state;
extern bool hasError;
extern list<errorNode> err;
extern errorNode errt;