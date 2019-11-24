#include "Utilize.h"
#include "lex.h"
using namespace std;


list<string> split(string &str,const string &pattern) {
	//���ķ��Ҳ��Ĳ���ʽ�ָ��һ��һ���ķ���,ͨ��find�������ҵ�λ�ý��зָ�
	list<string> res;
	if (str == "")
		return res;
	//���ַ���ĩβҲ����ָ����������ȡ���һ��
	string strs = str + pattern;
	size_t pos = strs.find(pattern);
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//ȥ���ѷָ���ַ���,��ʣ�µ��ַ����н��зָ�
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
//��ӡ�ʷ��������
	out << "�������������������������������������������������ʷ�����������¡���������������������������������������������������" << endl;
	if (out.is_open()) {
		for (list<tokenInfo>::const_iterator it = resultTok.cbegin(); it != resultTok.cend(); it++) {
			out << it->Line << " " << it->type << " " << it->content<<endl;
		}
	}
	out << endl << endl << endl;
}

void saveNoneTerminal(set<string> noneTerminalSymbols, ofstream &out) {
	out << "�������������������������������������������������ķ����ս�����¡���������������������������������������������������" << endl;
	if (out.is_open()) {
		for (set<string>::const_iterator it = noneTerminalSymbols.cbegin(); it != noneTerminalSymbols.cend(); it++) {
			out << *it << endl;
		}
	}
	out << endl << endl << endl;
}

//�����ս�����ļ���
void saveTerminal(set<string> terminalSymbols, ofstream &out) {
	out << "�������������������������������������������������ķ��ս�����¡�����������������������������������������������" << endl;
	if (out.is_open()) {
		for (set<string>::const_iterator it = terminalSymbols.cbegin(); it != terminalSymbols.cend(); it++) {
			out << *it << endl;
		}
	}
	out << endl << endl << endl;
}

//�����ķ����ŵ�first�����ļ���
void saveFirst(map<string, set<string>> firstSet, ofstream &out) {
	out << "�������������������������������������������������ķ����ŵ�first�����¡�����������������������������������������������" << endl;
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


//�������ʽ��first�����ļ���
void saveProductionFirst(map<int, set<string>> productionFirstSet, ofstream &out) {
	out << "�������������������������������������������������ķ�����ʽ��first�����¡�����������������������������������������������" << endl;
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

//������ս����follow�����ļ���
void saveFollow(map<string, set<string>> followSet, ofstream &out) {
	out << "���������������������������������������������������ս����follow�����¡�����������������������������������������������" << endl;
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


//�������ʽ��select�����ļ���
void saveProductionSelect(map<int, set<string>> selectSet, ofstream &out) {
	out << "����������������������������������������������������ʽ��select�����¡�����������������������������������������������" << endl;
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


//����Ԥ��������ļ���
void savePredictionTable(map<string, map<string, string>> predictionTable, ofstream &out) {
	out << "������������������������������������������������Ԥ����������¡�����������������������������������������������" << endl;
	if (out.is_open()) {
		for (auto ite1 = predictionTable.cbegin(); ite1 != predictionTable.cend(); ite1++) {
			for (auto ite2 = ite1->second.cbegin(); ite2 != ite1->second.cend(); ite2++) {
				out << ite1->first << " ������ " << ite2->first << ",���ò���ʽ:" << ite2->second << endl;
			}
		}
	}
	out << endl << endl << endl;
}

string EToString(const tokenType eParam)
{
	return stokenType[eParam];
}

void saveTree(const treeNode* head, ofstream &out) {
	
	out << "��" << endl;
	if (head->childNum != 0) 
		
		for (int i = 0; i < head->childNum; i++) {
			if (head->children[i] != NULL) {
				out << head->tokenStr << ":" << head->children[i]->tokenStr;
				saveTree(head->children[i], out);
			}
		}
	
}