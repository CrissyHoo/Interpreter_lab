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
#include <sstream>
#define MAXCHILDREN 10
using namespace std;
extern int line;
typedef enum {

	INIT=0,//Ĭ�ϳ�ʼֵ
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
	MULTI_NOTE,  //  /**/
	END

}tokenType;

//��������������õ���
#define MAX 65536
//����һ��value��,���ر��ʽ����Ľ��,�������þ���һ�����������õ��Ľ���ٽ�һ������symbol����
class value {
public:
	const int INTTYPE = 0;
	const int REALTYPE = 1;
	const int TRUETYPE = 2;//����ֵ
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

//�����symbol�࣬ר�������洢�������Žṹ
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
	//��������Ϊpublic��ԭ���Ƕ�����Ĳ����Ƚϸ��ӣ��������private����д�Ļ���ܶ�
	vector<int> intA;//��������飬�洢int����
	vector<double> douA;//��������飬�洢�����ֵ

private:
	string name;
	int type;//����ĸ�ֵ��ͨ��value������ֵ�ģ�value�����������˱������͵���Ϣ
	int intValue;//�����int����������洢int�����ľ���ֵ
	double realValue;//�����һ��ʵ������������洢ʵ��������ֵ
	int level;//��ʾ�����ڲ�
	symbol *next;//nextָ��һ��ͬ����������
	int size;//��ʼ��ʱ��size
};

//symbolֻ��Ե����ķ��Ŷ�����ṹ�洢����һ����
class symbolArr {
	//������list��������ű���Ϊ��ͨ�ķ��ű�ʹ洢����ķ��ű�������Ժ���˵
public:
	list<symbol> table;
	list<symbol> arrTable;//�����Ľṹ��table��һ���ģ�Ȼ��symbol���д洢����ı���intA,douA
	//��Ҫ��һ������Ԫ�صĲ���,����id��ֵ���ض�Ӧ���Ǹ�Ԫ�أ�����������ڷ��ű��в���
	symbol* searchTa(string name, int level) {//��level������һ����ʾ������Σ�����˵�����Σ���һ������
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
	//������������¼�������,��Ϊ�������ͨ����������ͬһ�����֣��������
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


//�������ڵ�����
typedef enum {
	DECLARENODE = 0,
	ASSIGNNODE,
	IFNODE,
	WHILENODE,
	READNODE,
	WRITENODE,
	STMTBLOCK,
	OPNODE,
	NEGATIVENODE    //����
}nodeType;

//Ϊ�˷���ʹ����Ҫ��enumת����string
const string stokenType[]={

	"INIT",//Ĭ�ϳ�ʼֵ
	//�쳣
	"ERROR",

	//�ؼ���
	"IF",   //if
	"ELSE", //else
	"WHILE",  //while
	"INT",   //int
	"REAL",   //float

	//����
	"INT_NUM",
	"REAL_NUM",

	//��ʶ��
	"ID",

	//�����
	"PLUS",       //+
	"MINUS",      //-
	"MUL",        //*
	"DIV",        // /
	"MOD",        // % 
	"ASSIGN", "LES", "LES_EQU", "GRT", "GRT_EQU", "NOT_EQU", "EQU",//=,<,<=,>,>=,<>,==

	//���޷�
	"LEFT_BRA", "RIGHT_BRA",   //     (, )
	"LEFT_INDEX", "RIGHT_INDEX",  //  [, ]
	"LEFT_BOUND", "RIGHT_BOUND",//    {, }
	"POINTE", "COMMA", "SEMI", "SIN_QUE", "DOU_QUE", //. , ; ' "


	//ע��

	"LINE_NOTE",  //  //
	"MULTI_NOTE",  //  /**/
	"END"

};
string EToString(const tokenType eParam);

//Ϊ�﷨�������õĽṹ���洢ÿ��token����Ϣ
struct tokenInfo {
	tokenType type;
	string content;
	int Line;//token����λ��

	//��ʼ������
	tokenInfo() {
		type = INIT;
		content = " ";
		Line = line;
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

//������͵�����ģ����ǻ����Ӳ������﷨��
struct treeNode {           //�﷨���ڵ�
	string tokenStr;//�����ս�����ԣ����token�Ǻ���ģ���������token��type�;����ֵ������type=ID��content=a
	nodeType NTtype;//���ڷ��ս�����ԣ�type��nodetype������tokentype
	string VTcontent;
	int Line;
	int childNum;           //������Ŀ
	treeNode* children[MAXCHILDREN];//ָ����ĺ���
	//Ϊ�˱��ڽ���Щ�ڵ���뵽����ջ�У���Ҫһ�����캯��
	treeNode(string tokenS) {
		tokenStr = tokenS;//���tokenStr���ڷ��ս�����ԣ����Ƿ��ս���ķ��ţ������ս�����ԣ����ս����type
		childNum = 0;
		Line = line;
	}
	treeNode() {
		//���캯���������κ�
		Line = line;
		childNum = 0;
	}
	nodeType getType() {
		return this->NTtype;
	}

};

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
