#include "lex.h"
#include "Utilize.h"
/*��grammar��ע��
S����ʼ����
A������
B��block��ģ�飬����while��飬if���
D:int����float
I��ID
N:һ�������������߸�����
L:later������������ڶ���ʱ���Ѿ���ֵ�����

*/
//��#��ʾ��������$��ʾ��

set<string> terminalSym; //�����ս���ż�
set<string> nonterminalSym;//������ս���ż�
string startToken;//�洢�ķ��Ŀ�ʼ����
string totalProductions[100];//����ķ��ļ���ÿһ�У������ķ�������100
production productions[100];//�洢���еĲ���ʽ 
int productionSum;//����ʽ��Ŀ 
fstream grammarFile; //�ķ��ļ�
map<string, set<string>> firstSet;//�������ķ����ŵ�first���ϣ�ע��map���÷�
map<int, set<string>> productionFirstSet;//�洢���в���ʽ��first����
map<string, set<string>> followSet;//�������ķ���follow����
map<string, set<string>> selectSet;//�����е�select����
map<string, map<string, string>> predictionTable;//LL1���Ǹ���Ԥ�������
fstream parseFile("parse.txt");//����﷨��������ļ�
ofstream setFile("setFile.txt");//���LL1������͸��ּ��ϱ�


stack<string> anaStack;
stack<tokenInfo> remainTok;
int top;  //����ջջ��ָ��

void init() {
	//�����������
	terminalSym.clear();
	nonterminalSym.clear();
	for (int i = 0; i < 100; i++) {
		productions[i].genera.clear();
	}
}

//���ķ��ļ��еĲ���ʽ�洢�����ݽṹ��
void saveProduction(string grammarName) {
	init();
	char perProduction[100];   //ÿ����������ʽ
	char *pos = NULL;//�洢�ַ����ֵ�λ��
	string productionRight;      //����ʽ�Ҳ�
	string noneTerminalSymbol;   //���ս��

	grammarFile.open(grammarName, ios::in);//�Զ�����ʽ��
	if (grammarFile.is_open() != true) {
		cout << "���ļ�ʧ��" << endl;
		return;
	}

	productionSum = 0;
	while (!grammarFile.eof()) {//ɨ���ķ��ļ����洢
		productionSum++;
		grammarFile.getline(perProduction, 100);//getline����������ȡ�����ַ������ڵ�һ��������
		totalProductions[productionSum - 1] = perProduction;
		pos = strstr(perProduction, ":=");  //�����ַ������ȳ��֣�=��λ��
		productionRight.replace(productionRight.begin(), productionRight.end(), pos + 3);   //ȡ������ʽ�Ҳ��и��ո�����+3
		noneTerminalSymbol.replace(noneTerminalSymbol.begin(), noneTerminalSymbol.end(), perProduction,
			pos - perProduction);    //ȡ������ʽ�󲿣�Ҳ���Ƿ��ս������

		productions[productionSum - 1].nonTerminalSym = noneTerminalSymbol;//��productions��ֵ

		list <string> perProRight = split(productionRight, " ");//������ʽ�Ҳ����ֳ�һ�����ĵ�������

		//�������list��Ȼ��Ѳ���ʽ���ּӵ����ȫ��������ţ���������vector��pushback������pushback�����������ڵ�ǰvector��β������һ��Ԫ��

		for (list<string>::const_iterator it = perProRight.cbegin(); it != perProRight.cend(); ++it) {
			productions[productionSum - 1].genera.push_back(*it);
		}

		//�����ս����������ս������
		if (nonterminalSym.find(noneTerminalSymbol) == nonterminalSym.end()) {//û�ҵ������Ծ�Ҫ�ӽ�ȥ
			nonterminalSym.insert(noneTerminalSymbol);
		}
		//Ĭ���ķ���һ������ʽ��Ϊ��ʼ��
		if (productionSum == 1) {
			startToken = noneTerminalSymbol;
		}
	}

	//�洢����֮��洢�ս���ż�
	string tmp;
	for (int i = 0; i < productionSum; i++) {
		//�������еĲ���ʽ�Ҳ������øոշָ�õ�������
		for (int j = 0; j < productions[i].genera.size(); j++) {
			tmp = productions[i].genera[j];
			if ((terminalSym.find(tmp) == terminalSym.end()) &&(
				nonterminalSym.find(tmp) == nonterminalSym.end())) {
				terminalSym.insert(tmp);
			}
		}

	}
	terminalSym.insert("#");
	terminalSym.insert("$");    //����

	saveNoneTerminal(nonterminalSym, setFile);
	saveTerminal(terminalSym, setFile);

}

//�����з��ŵ�first����
void getFirstSet() {
	//�����ս����˵��first����������
	for (set<string>::const_iterator it = terminalSym.cbegin(); it != terminalSym.cend(); it++) {
		firstSet[*it].insert(*it);
	}
	//������ս������
	int preSize;
	int curSize;//��ǰ��size
	bool changed;//���������������Ƿ��first�������˸ı�

	do {
		changed = false;
		for (int i = 0; i < productionSum; ++i) {  //����ÿһ������ʽ
			preSize = firstSet[productions[i].nonTerminalSym].size();   //ĳ�����ս����Ӧ��first����Ԫ�ظ�������ʼֵӦ��Ϊ0��
			bool shouldContinue = true;     //�Ƿ������˲���ʽ��first��

			for (int j = 0; j < productions[i].genera.size() && shouldContinue; ++j) {  //��������ʽ�Ҳ���ÿ��Ԫ�� S->ABc
				
				for (set<string>::const_iterator ite = firstSet[productions[i].genera[j]].cbegin(); 
					ite != firstSet[productions[i].genera[j]].cend(); ite++) {         //��ò���ʽ�Ҳ�ÿ��Ԫ�ص�first���е�Ԫ�أ���AΪ�����Ȼ��A��first��
					
					if (firstSet[productions[i].nonTerminalSym].find(*ite) ==firstSet[productions[i].nonTerminalSym].end() && *ite != "$") 
					{
						firstSet[productions[i].nonTerminalSym].insert(*ite);           //��A��first��Ԫ�ض��ӵ�S��first����,���˿���
					}

				}
				if (firstSet[productions[i].genera[j]].find("$") ==
					firstSet[productions[i].genera[j]].end()) {
					shouldContinue = false;                                                 //���A��first����û�п��֣��������˲���ʽ��first�������������B����
				}
			}
			if (shouldContinue) firstSet[productions[i].nonTerminalSym].insert("$");//�����һ��Ԫ�ص�first�����п��֣��򽫿���Ҳ����˲���ʽ�󲿷��ս����first����
			curSize = firstSet[productions[i].nonTerminalSym].size();
			if (curSize > preSize) changed = true;
		}
	} while (changed);  //��first���ı��ˣ�������ظ�����
	//�浽�ļ��У����ڼ��
	saveFirst(firstSet, setFile);

}


void getProductionFirstset() {
	int i, j;
	bool has$;

	for (i = 0; i < productionSum; i++) {
		has$ = true;
		for (j = 0; j < productions[i].genera.size() && has$; j++) {    //��������ʽ�Ҳ�����S->ABc,�ȿ�first(A),���Ұ����пռ�
			for (set<string>::const_iterator ite = firstSet[productions[i].genera[j]].cbegin(); ite != firstSet[productions[i].genera[j]].cend(); ite++) 
			{//�����Ҳ�ÿ��Ԫ�ص�first����
				if (*ite != "$") {
					productionFirstSet[i].insert(*ite);
				}
			}

			//�����ǰ���ŵ�first��û�п��֣��������ʽfirst�����Խ�����
			if (firstSet[productions[i].genera[j]].find("$") == firstSet[productions[i].genera[j]].end()) {
				has$ = false;
				break;
			}
		}
		//�������������ʽ�Ҳ�ĩβ�������һ�����ŵ�first�����п��֣����Ȳ����ѽ�����Ҳ�������ʽ��first���У�ֻ�п�follow����
		if (j == productions[i].genera.size() && has$) {
			productionFirstSet[i].insert("$");
		}
	}

	//���浽�ļ���
	saveProductionFirst(productionFirstSet, setFile);
}

//�����з��ս����follow����
void buildFollowSet() {
	followSet[startToken].insert("#");  //�ķ��Ŀ�ʼ���ŵ�follow���а���{#}
	int preSize;    //���ս��֮ǰ��follow��Ԫ�ظ���
	int curSize;    //���ս�����ڵ�follow��Ԫ�ظ���
	bool changed;   //���ս����follow���Ƿ�ı�
	bool has$;  //�Ƿ��п���
	int i, j, k;
	int lastNoneTerminal = -1;   //����ʽ�Ҳ����һ�����ս����λ��

	do {
		changed = false;
		for (i = 0; i < productionSum; ++i) {
			for (j = 0; j < productions[i].generation.size(); ++j) {    //����ÿ������ʽx,�� A->aB�¦�
				//���x[j]�Ƿ��ս��,������B������B��follow��
				if (noneTerminalSymbols.find(productions[i].generation[j]) != noneTerminalSymbols.end()) {
					preSize = followSet[productions[i].generation[j]].size();   //�����ʱB��follow��Ԫ�ظ���
					for (k = j + 1; k < productions[i].generation.size(); ++k) {  //����B֮����ķ����ţ����¦�
						has$ = false;
						for (set<string>::const_iterator ite = firstSet[productions[i].generation[k]].cbegin();
							ite != firstSet[productions[i].generation[k]].cend(); ite++) { //��first(��)�ķǿ�Ԫ�ؼ���follow(B)��
							if (*ite == "$") {
								has$ = true;
							}
							else if (followSet[productions[i].generation[j]].find(*ite) ==
								followSet[productions[i].generation[j]].end() && *ite != "$") {
								followSet[productions[i].generation[j]].insert(*ite);
							}
						}
						if (!has$) {    //���first(��)���������֣����˳�
							break;
						}
					}   //��������֮����ķ�����
					if (k == productions[i].generation.size() && has$) {    //�����������ʽĩβ���������һ���԰������֣����follow(A)Ҳ����follow(B)��
						for (set<string>::const_iterator ite = followSet[productions[i].noneTerminalSymbol].cbegin();
							ite != followSet[productions[i].noneTerminalSymbol].cend(); ite++) {
							if (followSet[productions[i].generation[j]].find(*ite) ==
								followSet[productions[i].generation[j]].end()) {
								followSet[productions[i].generation[j]].insert(*ite);
							}
						}
					}
					curSize = followSet[productions[i].generation[j]].size();   //�����ʱB��follow��Ԫ�ظ���
					if (curSize > preSize) changed = true;

					lastNoneTerminal = j;   //��¼������ʽ���һ�����ս����λ��
				}
			}
			if (lastNoneTerminal == j - 1) {    //�������ʽ���һ�������Ƿ��ս�������follow�����޷�������������������ֱ�ӽ�follow(A)�����follow����
				for (set<string>::const_iterator ite = followSet[productions[i].noneTerminalSymbol].cbegin();
					ite != followSet[productions[i].noneTerminalSymbol].cend(); ite++) {
					if (followSet[productions[i].generation[lastNoneTerminal]].find(*ite) ==
						followSet[productions[i].generation[lastNoneTerminal]].end()) {
						followSet[productions[i].generation[lastNoneTerminal]].insert(*ite);
					}
				}
				lastNoneTerminal = -1;      //���»ָ���ʼֵ
			}
			else {
				lastNoneTerminal = -1;      //���»ָ���ʼֵ
			}
		}
	} while (changed);  //��follow���ı��ˣ�������ظ�����

	//���浽�ļ���
	saveFollow(followSet, set_file);
}

void getPredictTable() {
	//Ԥ�����ò�Ҫ���ظ�����Ȼ�ͻṹ�ɶ�����,������һ��map���洢
	int i;
	for (i = 0; i < productionSum; i++) {
		//��������ʽ
		//������i������ʽ��firstset�������ҽ���i���ķ��洢��predictTable[S��a]��S->aB��
		for (set<string>::const_iterator it = productionFirstSet[i].cbegin(); it != productionFirstSet[i].cend();it++) {

			predictionTable[productions[i].nonTerminalSym][*it] = totalProductions[i];
		}
	}

	//�洢
	savePredictionTable(predictionTable, setFile);
}

//ʹ��Ԥ�����������﷨����
void analyse() {

	list<string> usedProduction;
	list<string> usedProductionRight;
	tokenInfo p;
	p.content = "#";
	

	//��remaintok��ֵ�������ʷ��������
	remainTok.push(p);
	for (auto it = resultTok.rbegin(); it != resultTok.rend(); it++) {
		
		remainTok.push(*it);//�����ﵹ������ˣ�����ջ����Ԫ��Ӧ�����ʼ���Ǹ�����

	}
	
	

	
	anaStack.push(startToken);//�׷��Ž�ջ

	while (true) {
		//�������ջ�����봮��ֻʣ��#,�������
		if (anaStack.top()=="#"&&remainTok.top().content == "#") {
			cout << "�﷨�������" << endl;
			break;
		}
		else if (nonterminalSym.find(anaStack.top()) != nonterminalSym.end()) {
			//�������ջ���Ƿ��ս��
			//���Ԥ������Ƿ��в���ʽƥ���Ӵ�ջ��ջ��
			//ǰ���Ӵ�ջ���Ǹ��ս������������������ҵ���
			if (terminalSym.find(remainTok.top().content) != terminalSym.end() &&
				predictionTable[anaStack.top()].find(remainTok.top().content) != predictionTable[anaStack.top()].end()) 
			{
				cout << "action:" << predictionTable[anaStack.top()][remainTok.top().content] << endl << endl;
				//��ȡ����ʽ�Ҳ�
				usedProduction = split(predictionTable[anaStack.top()][remainTok.top().content], ":= ");
				list<string>::iterator ite = usedProduction.begin();
				ite++;
				//��ȡ�Ҳ���ÿһ��
				usedProductionRight = split(*ite, " ");

				//����ջ�������ҽ�����ʽ�Ҳ���ջ
				anaStack.pop();
				for (auto ite = usedProductionRight.rbegin(); ite != usedProductionRight.rend(); ite++ ) {
					//��������������ʽ����������ջ������֮ǰ��ϰ��
					anaStack.push(*ite);
				}

			}
			else {
				cout << "���󣡵�ǰtoken���ɱ�ʶ��:" << "λ�ڳ����" << remainTok.top().line << "�е�" << remainTok.top().content << endl << endl;
				break;
			}
		}
		else if (terminalSym.find(anaStack.top()) != terminalSym.end()) {
			//�������ջջ�����ս��
			//����Ӵ�ջ����Ԫ�ػ����ս�����鿴�Ƿ����ƥ��
			if (anaStack.top()== remainTok.top().content) {
				//�������ƥ����,����
				anaStack.pop();
				//ʣ�മ��ָ�����һ��
				remainTok.pop(); 
				cout << "ƥ�䣺" << remainTok.top().content << endl;
			}
			else {
				//����ջջ�����ս�����һ�������ƥ��
				cout << "���ִ���λ�ڳ����" << remainTok.top().line << "�е�" << remainTok.top().content << endl;
				break;
			}
		}
	}
}



	


