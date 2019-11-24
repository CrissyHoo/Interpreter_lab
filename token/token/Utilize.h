#pragma once
//这里包含了一些通用的函数
#include <vector>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <map>
#include <stack>
#define MAXCHILDREN 10
using namespace std;
extern int line;
typedef enum {

	INIT=0,//默认初始值
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
	MULTI_NOTE,  //  /**/

	END

}tokenType;
//为了方便使用需要把enum转化成string

const string stokenType[]={

	"INIT",//默认初始值
	//异常
	"ERROR",

	//关键字
	"IF",   //if
	"ELSE", //else
	"WHILE",  //while
	"INT",   //int
	"REAL",   //float

	//数字
	"INT_NUM",
	"REAL_NUM",

	//标识符
	"ID",

	//运算符
	"PLUS",       //+
	"MINUS",      //-
	"MUL",        //*
	"DIV",        // /
	"MOD",        // % 
	"ASSIGN", "LES", "LES_EQU", "GRT", "GRT_EQU", "NOT_EQU", "EQU",//=,<,<=,>,>=,<>,==

	//界限符
	"LEFT_BRA", "RIGHT_BRA",   //     (, )
	"LEFT_INDEX", "RIGHT_INDEX",  //  [, ]
	"LEFT_BOUND", "RIGHT_BOUND",//    {, }
	"POINTE", "COMMA", "SEMI", "SIN_QUE", "DOU_QUE", //. , ; ' "


	//注释

	"LINE_NOTE",  //  //
	"MULTI_NOTE",  //  /**/
	"#"

};
string EToString(const tokenType eParam);

//为语法分析设置的结构，存储每个token的信息
struct tokenInfo {
	tokenType type;
	string content;
	int Line;//token所处位置

	//初始化函数
	tokenInfo() {
		type = INIT;
		content = " ";
		Line = line;
	}
};

struct errorNode {          //错误节点，用来组织成错误链表
	string content;         //内容
	int line;               //所在行数
};

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

//本来想偷个懒的，但是还是逃不过画语法树
struct treeNode {           //语法树节点
	string content;         //token的内容
	int type;               //类型,tokentype类型，enum整数
	string tokenStr;        //以字符串形式表示的token
	int Line;               //所在行数
	int childNum;           //孩子数目
	treeNode* children[MAXCHILDREN];//指向你的孩子
	//为了便于将这些节点加入到分析栈中，需要一个构造函数
	treeNode(string tokenS) {
		tokenStr = tokenS;
		childNum = 0;
		Line = line;
		for (int i = 0; i < MAXCHILDREN; i++) {
			//必须分配内存and构建对象，不然就是野指针
			children[i] = NULL;
		}

	}
	treeNode() {
		//构造函数，不做任何
		Line = line;
		childNum = 0;
	}

};

//分割函数
list<string> split(string &str, const string &pattern);
void printToken(tokenType token, const char tokenString[]);
void saveNoneTerminal(set<string> noneTerminalSymbols, ofstream &out);
void saveTerminal(set<string> terminalSymbols, ofstream &out);
void saveFirst(map<string, set<string>> firstSet, ofstream &out);
void saveProductionFirst(map<int, set<string>> productionFirstSet, ofstream &out);
void saveFollow(map<string, set<string>> followSet, ofstream &out);
void saveProductionSelect(map<int, set<string>> selectSet, ofstream &out);
void savePredictionTable(map<string, map<string, string>> predictionTable, ofstream &out);
void saveTokenRes(list<tokenInfo> resultTok, ofstream &out);
void saveTree(const treeNode* head, ofstream &out);

extern ofstream token_ana;
extern list<tokenInfo> resultTok;

extern stateType state;
extern bool hasError;
extern list<errorNode> err;
extern errorNode errt;
extern string totalProductions[100];
