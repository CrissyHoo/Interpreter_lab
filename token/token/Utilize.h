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
#include <sstream>
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

//下面是语义分析用到的
#define MAX 65536
//定义一个value类,返回表达式计算的结果,他的作用就是一个过渡器，得到的结果再进一步给到symbol里面
class value {
public:
	const int INTTYPE = 0;
	const int REALTYPE = 1;
	const int TRUETYPE = 2;//布尔值
	const int FALSETYPE = 3;
	int getType() {
		return this->type;
	}
	void setType(int TYPE) {
		this->type = TYPE;
	}

	void setIntValue(int i) {
		this->mint = i;
	}
	int getIntValue() {
		return this->mint;
	}

	void setDouValue(double j) {
		this->mfloat = j;
	}
	double getDouValue() {
		return this->mfloat;
	}
	
private:
	int type;
	double mfloat;
	int mint;
	
};

//定义的symbol类，专门用来存储单个符号结构
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
	int getType() {
		return this->type;
	}
	void setName(string s) {
		this->name = s;
	}
	void addVecInt(int tmp) {
		this->intA.push_back(tmp);
	}
	void addVecDou(double db) {
		this->douA.push_back(db);
	}
	string getName() {
		return this->name;
	}
	int getLevel() {
		return this->level;
	}
	int getIntV() {
		return this->intValue;
	}
	double getDouV() {
		return this->realValue;
	}
	void setInt(int i) {
		this->intValue = i;
	}
	void setDou(double d) {
		this->realValue = d;
	}
	int getSize() {
		return this->size;
	}
	void setSize(int s) {
		this->size = s;
	}
	//数组设置为public的原因是对数组的操作比较复杂，如果都在private里面写的话会很多
	vector<int> intA;//如果是数组，存储int数组
	vector<double> douA;//如果是数组，存储数组的值

private:
	string name;
	int type;//这个的赋值是通过value类来赋值的，value类里面设置了变量类型的信息
	int intValue;//如果是int变量，这里存储int变量的具体值
	double realValue;//如果是一个实数变量，这里存储实数变量的值
	int level;//显示外层和内层
	symbol *next;//next指向一个同名的外层变量
	int size;//初始化时的size
};

//symbol只针对单个的符号而这个结构存储的是一个表
class symbolArr {
	//我们用list来代表符号表，分为普通的符号表和存储数组的符号表，具体的以后再说
public:
	list<symbol> table;
	list<symbol> arrTable;//数组表的结构跟table是一样的，然后symbol中有存储数组的变量intA,douA
	//需要有一个查找元素的操作,根据id的值返回对应的那个元素，下面这个是在符号表中查找
	symbol* searchTa(string name, int level) {//把level看成是一个表示声明层次（或者说代码层次）的一个变量
		for (list<symbol>::iterator it = table.begin(); it != table.end();it++) {
			if (it->getName() == name && level == it->getLevel()) {
				return &(*it);
				break;
			}
			else {
				return NULL;
			}
			
		}

	}
	//这个是在数组记录里面查找,因为数组和普通变量可以有同一个名字，不会出错
	symbol* searchArr(string name, int level) {
		for (list<symbol>::iterator it = arrTable.begin(); it != arrTable.end(); it++) {
			if (it->getName() == name && it->getLevel() == level) {
				return &(*it);
				break;
			}
			else {
				return NULL;
			}
		}

	}
	


};


//定义树节点类型
typedef enum {
	DECLARENODE = 0,
	ASSIGNNODE,
	IFNODE,
	WHILENODE,
	READNODE,
	WRITENODE,
	STMTBLOCK,
	OPNODE,
	NEGATIVENODE    //负数
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
	"END"

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
	string tokenStr;//对于终结符而言，这个token是合理的，里面存的是token的type和具体的值，例如type=ID，content=a
	nodeType NTtype;//对于非终结符而言，type是nodetype而不是tokentype
	string VTcontent;
	int Line;
	int childNum;           //孩子数目
	treeNode* children[MAXCHILDREN];//指向你的孩子
	//为了便于将这些节点加入到分析栈中，需要一个构造函数
	treeNode(string tokenS) {
		tokenStr = tokenS;//这个tokenStr对于非终结符而言，就是非终结符的符号，对于终结符而言，是终结符的type
		childNum = 0;
		Line = line;
	}
	treeNode() {
		//构造函数，不做任何
		Line = line;
		childNum = 0;
	}
	nodeType getType() {
		return this->NTtype;
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
void convert2Num(string s, string type,value*va);
tokenType str2Token(string s);
int getPerior(tokenType a);

extern ofstream token_ana;
extern list<tokenInfo> resultTok;

extern stateType state;
extern bool hasError;
extern list<errorNode> err;
extern errorNode errt;
extern string totalProductions[100];
