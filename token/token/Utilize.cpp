#include "Utilize.h"
#include "lex.h"
using namespace std;


list<string> split(string &str,const string &pattern) {
	//将文法右部的产生式分割成一个一个的符号,通过find函数来找到位置进行分割
	list<string> res;
	if (str == "")
		return res;
	//在字符串末尾也加入分隔符，方便截取最后一段
	string strs = str + pattern;
	size_t pos = strs.find(pattern);
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + pattern.length(), strs.size());
		pos = strs.find(pattern);
	}

	return res;
}

void printToken(tokenType token, const char tokenString[]) {
	switch (token) {
	case IF:
	case ELSE:
	case INT:
	case REAL:
	case WHILE:
		token_ana << "Reserved Word: " << tokenString << endl;
		break;

	case LEFT_BRA:
	case RIGHT_BRA:
	case LEFT_INDEX:
	case RIGHT_INDEX:
	case LEFT_BOUND:
	case RIGHT_BOUND:
	case SIN_QUE:
	case DOU_QUE:
	case COMMA:
	case SEMI:

		token_ana << "Bound Symbol: " << tokenString << endl;
		break;

	case PLUS:
	case MINUS:
	case MUL:
	case DIV:
	case MOD:
	case ASSIGN:
	case LES:
	case LES_EQU:
	case GRT:
	case GRT_EQU:
	case NOT_EQU:
	case EQU:
		token_ana << "Operator: " << tokenString << endl; break;

	case INT_NUM:
		token_ana << "integer:" << tokenString << endl; break;
	case REAL_NUM:
		token_ana << "real number:" << tokenString << endl; break;
		break;

	case ID:
		token_ana << "Identifier: " << tokenString << endl;
		break;
	case LINE_NOTE:
		token_ana << "line comment:" << tokenString << endl;
		break;
	case MULTI_NOTE:
		token_ana << "multipul lines comment:" << tokenString << endl;
		break;
	case ERROR:
		hasError = true;
		token_ana << "ERROR token:" << tokenString << endl;
		errt.content = tokenString;
		errt.line = line;
		err.push_back(errt);



		break;
	default:
		token_ana << "Unknown Token:" << tokenString << endl;
	}
}

void saveTokenRes(list<tokenInfo> resultTok, ofstream &out) {
//打印词法分析结果
	out << "————————————————————————词法分析结果如下——————————————————————————" << endl;
	if (out.is_open()) {
		for (list<tokenInfo>::const_iterator it = resultTok.cbegin(); it != resultTok.cend(); it++) {
			out << it->Line << " " << it->type << " " << it->content<<endl;
		}
	}
	out << endl << endl << endl;
}

void saveNoneTerminal(set<string> noneTerminalSymbols, ofstream &out) {
	out << "————————————————————————文法非终结符如下——————————————————————————" << endl;
	if (out.is_open()) {
		for (set<string>::const_iterator it = noneTerminalSymbols.cbegin(); it != noneTerminalSymbols.cend(); it++) {
			out << *it << endl;
		}
	}
	out << endl << endl << endl;
}

//保存终结符到文件中
void saveTerminal(set<string> terminalSymbols, ofstream &out) {
	out << "————————————————————————文法终结符如下————————————————————————" << endl;
	if (out.is_open()) {
		for (set<string>::const_iterator it = terminalSymbols.cbegin(); it != terminalSymbols.cend(); it++) {
			out << *it << endl;
		}
	}
	out << endl << endl << endl;
}

//保存文法符号的first集到文件中
void saveFirst(map<string, set<string>> firstSet, ofstream &out) {
	out << "————————————————————————文法符号的first集如下————————————————————————" << endl;
	if (out.is_open()) {
		for (auto ite = firstSet.cbegin(); ite != firstSet.cend(); ite++) {
			out <<  ite->first;
			out << " --- {";
			for (auto ite2 = ite->second.cbegin(); ite2 != ite->second.cend(); ite2++) {
				if (++ite2 == ite->second.cend()) {
					ite2--;
					out << *ite2 << "}" << endl;
				}
				else {
					ite2--;
					out << *ite2 << ",";
				}
			}
		}
	}
	out << endl << endl << endl;
}


//保存产生式的first集到文件中
void saveProductionFirst(map<int, set<string>> productionFirstSet, ofstream &out) {
	out << "————————————————————————文法产生式的first集如下————————————————————————" << endl;
	if (out.is_open()) {
		for (auto ite = productionFirstSet.cbegin(); ite != productionFirstSet.cend(); ite++) {
			out << totalProductions[ite->first] << " --- {";
			//ite->second means you get the value of the map
			for (auto ite2 = ite->second.cbegin(); ite2 != ite->second.cend(); ite2++) {
				if (++ite2 == ite->second.cend()) {
					ite2--;
					out << *ite2 << "}" << endl;
				}
				else {
					ite2--;
					out << *ite2 << ",";
				}
			}
		}
	}
	out << endl << endl << endl;
}

//保存非终结符的follow集到文件中
void saveFollow(map<string, set<string>> followSet, ofstream &out) {
	out << "————————————————————————非终结符的follow集如下————————————————————————" << endl;
	if (out.is_open()) {
		for (auto ite = followSet.cbegin(); ite != followSet.cend(); ite++) {
			out  << ite->first << " --- {";
			for (auto ite2 = ite->second.cbegin(); ite2 != ite->second.cend(); ite2++) {
				if (++ite2 == ite->second.cend()) {
					ite2--;
					out << *ite2 << "}" << endl;
				}
				else {
					ite2--;
					out << *ite2 << ",";
				}
			}
		}
	}
	out << endl << endl << endl;
}


//保存产生式的select集到文件中
void saveProductionSelect(map<int, set<string>> selectSet, ofstream &out) {
	out << "————————————————————————产生式的select集如下————————————————————————" << endl;
	if (out.is_open()) {
		for (auto ite = selectSet.cbegin(); ite != selectSet.cend(); ite++) {
			out << totalProductions[ite->first] << " --- {";
			for (auto ite2 = ite->second.cbegin(); ite2 != ite->second.cend(); ite2++) {
				if (++ite2 == ite->second.cend()) {
					ite2--;
					out << *ite2 << "}" << endl;
				}
				else {
					ite2--;
					out << *ite2 << ",";
				}
			}
		}
	}
	out << endl << endl << endl;
}


//保存预测分析表到文件中
void savePredictionTable(map<string, map<string, string>> predictionTable, ofstream &out) {
	out << "————————————————————————预测分析表如下————————————————————————" << endl;
	if (out.is_open()) {
		for (auto ite1 = predictionTable.cbegin(); ite1 != predictionTable.cend(); ite1++) {
			for (auto ite2 = ite1->second.cbegin(); ite2 != ite1->second.cend(); ite2++) {
				out << ite1->first << " 遇到了 " << ite2->first << ",采用产生式:" << ite2->second << endl;
			}
		}
	}
	out << endl << endl << endl;
}

string EToString(const tokenType eParam)
{
	return stokenType[eParam];
}

tokenType str2Token(string s) {
	//由于只需要符号的，所以只写一个符号的
	if (s == "PLUS") {
		return PLUS;
	}
	else if (s == "MINUS") {
		return MINUS;
	}
	else if (s == "MUL") {
		return MUL;
	}
	else if (s == "DIV") {
		return DIV;
	}
	else if (s == "MOD") {
		return MOD;
	}
	else if (s == "GRT") {
		return GRT;
	}
	else if(s=="GRT_EQU")
	{
		return GRT_EQU;
	}
	else if (s == "LES") {
		return LES;
	}
	else if (s == "LES_EQU") {
		return LES_EQU;
	}
	else if (s == "EQU") {
		return EQU;
	}
	else if (s == "NOT_EQU") {
		return NOT_EQU;
	}
	else {
		cout << "error:the expression may have mistakes." << endl;
	}
}

int getPerior(tokenType s)
{
	if (s == PLUS) {
		return 1;
	}
	else if (s == MINUS) {
		return 1;
	}
	else if (s == MUL) {
		return 2;
	}
	else if (s == DIV) {
		return 2;
	}
	else if (s == MOD) {
		return 2;
	}
	else if (s == GRT) {
		return 3;
	}
	else if (s == GRT_EQU)
	{
		return 3;
	}
	else if (s == LES) {
		return 3;
	}
	else if (s == LES_EQU) {
		return 3;
	}
	else if (s == EQU) {
		return 3;
	}
	else if (s == NOT_EQU) {
		return 3;
	}
	else if (s == LEFT_BRA) {
		return 4;
	}
	else {
		return -1;
	}
	
}

void saveTree(const treeNode* head, ofstream &out) {
	
	out << "树" << endl;
	if (head->childNum != 0) 
		
		for (int i = 0; i < head->childNum; i++) {
			if (head->children[i] != NULL) {
				out << head->tokenStr << ":" << head->children[i]->tokenStr;
				saveTree(head->children[i], out);
			}
		}
	
}

void convert2Num(string s, string type,value*va)
{
	stringstream ss;
	ss << s;
	//因为大多数都是通过string来传递的，所以使用一个函数来写比较方便
	if (type == "int") {
		int i;
		ss >> i;
		va->setIntValue(i);
	}
	else if (type == "double") {
		double i;
		ss >> i;
		va->setDouValue(i);
	}
}
