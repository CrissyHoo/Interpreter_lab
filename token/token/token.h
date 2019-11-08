#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include <list>
#include "Utilize.h"
using namespace std;


typedef enum {

	INIT,//默认初始值
	//异常
	ERROR,

	//关键字
	IF,   //if
	ELSE, //else
	WHILE,  //while
	INT,   //int
	REAL,   //float

	//数字
	INT_NUM,
	REAL_NUM,

	//标识符
	ID,

	//运算符
	PLUS,       //+
	MINUS,      //-
	MUL,        //*
	DIV,        // /
	MOD,        // % 
	ASSIGN, LES, LES_EQU, GRT, GRT_EQU, NOT_EQU, EQU,//=,<,<=,>,>=,<>,==

	//界限符
	LEFT_BRA, RIGHT_BRA,   //     (, )
	LEFT_INDEX, RIGHT_INDEX,  //  [, ]
	LEFT_BOUND, RIGHT_BOUND,//    {, }
	POINTE, COMMA, SEMI, SIN_QUE, DOU_QUE, //. , ; ' "
	

	//注释

	LINE_NOTE,  //  //
	MULTI_NOTE  //  /**/

}tokenType;

typedef enum {
	//DFA状态集
	START,
	INNUM,  //一个数字
	INID,   //标识符	

	//运算符
	INPLUS,
	INMIN,
	INMUL,
	INDIV,
	INMOD,
	INASSIGN,
	INLES,
	INGRT,

	//实数
	INREAL0,
	INREAL,

	//关系符
	INSPECIAL,

	//关于注释
	INLINECOM,
	INMULCOM1,
	INMULCOM2,
	DONE

}stateType;

//为语法分析设置的结构，存储每个token的信息
struct tokenInfo {
	string type;
	string content;
	int line;//token所处位置
	
	//初始化函数
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