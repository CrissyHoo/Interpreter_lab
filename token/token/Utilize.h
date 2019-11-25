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

	START,//开始
	END

}tokenType;

//下面是语义分析用到的
#define MAX 65536
//定义一个value类,返回表达式计算的结果
class value {
public:
	const int INTTYPE = 0;
	const int REALTYPE = 1;
	const int TRUETYPE = 2;
	const int FALSETYPE = 3;
	int getType() {
		return this->type;
	}
	
private:
	int type;
	double mfloat;
	int mint;
	
};
class valueArr {
	//我们用list来代表符号表，分为普通的符号表和存储数组的符号表，具体的以后再说
public:
	list<symbol> table;
	list<symbol> arrTable;
	//需要有一个查找元素的操作,根据id的值返回对应的那个元素
	symbol search(string name, int level) {//暂时把level看成是一个表示声明层次（或者说代码层次）的一个变量
		
		for (list<symbol>::iterator it = table.begin(); it != table.end(); it++) {
			if ((*it).getName() == name && level == (*it).getLevel()) {
				return *it;
				
			}
			
		}

	}
};
//定义的symbol类，专门用来存储符号
class symbol {
public:
	const int INTTYPE = 0;
	const int REALTYPE = 1;
	
	void setLevel(int lev) {
		this->level = lev;
	}
	void setType(int ty) {
		this->type = ty;
	}
	void setName(string s) {
		this->name = s;
	}
	string getName() {
		return this->name;
	}
	int getLevel() {
		return this->level;
	}

private:
	string name;
	int type;
	int intValue;
	double realValue;
	int level;
	symbol *next;//next指向一个同名的外层变量
};
//value和symbol是一一对应的

//定义树节点类型
typedef enum {
	DECLARENODE = 0,
	ASSIGNNODE,
	IFNODE,
	WHILENODE,
	READNODE,
	WRITENODE,
	STMTBLOCK
}nodeType;

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
	string content;           //非终结符用type，终结符用content
	int type;               //这个type是用来做语义分析的，应该是一个枚举类型，先这么写吧，在这里注释一下你的tokentype可能要改一改
	string tokenStr;        //以字符串形式表示的token,
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
