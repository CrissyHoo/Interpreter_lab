#include "lex.h"
#include "Utilize.h"


set<string> terminalSym; //�����ս���ż�
set<string> nonterminalSym;//������ս���ż�
string startToken;//�洢�ķ��Ŀ�ʼ����
string totalProductions[100];//����ķ��ļ���ÿһ�У������ķ�������100
production productions[100];//�洢���еĲ���ʽ 
int productionSum;//����ʽ��Ŀ 
fstream grammarFile; //�ķ��ļ�
map<string, set<string>> firstSet;//�������ķ����ŵ�first���ϣ�ע��map���÷�
map<int, set<string>> productionFirstSet;//�洢���в���ʽ��first����
//map<string, set<string>> followSet;//�������ķ���follow����
//map<string, set<string>> selectSet;//�����е�select����
map<string, map<string, string>> predictionTable;//LL1���Ǹ���Ԥ�������


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

		list <string> perProRight = split(productionRight, " ");

		//�������list��Ȼ��Ѳ���ʽ���ּӵ����ȫ��������ţ���������vector��pushback������pushback�����������ڵ�ǰvector��β������һ��Ԫ��

		for (list<string>::const_iterator it = perProRight.cbegin(); it != perProRight.cend(); ++it) {
			productions[productionSum - 1].genera.push_back(*it);
		}

		//�����ս����������ս������
		if (nonterminalSym.find(noneTerminalSymbol) == nonterminalSym.end()) {//һ����û�ҵ�
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
			if (terminalSym.find(tmp) == terminalSym.end() &&
				nonterminalSym.find(tmp) == nonterminalSym.end()) {
				terminalSym.insert(tmp);
			}
		}

	}
	terminalSym.insert("#");


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
		for (int i = 0; i < productionSum; i++) {
			preSize = firstSet[productions[i].nonTerminalSym].size();

			//�������ʽ�Ҳ��ĵ�һ��Ԫ���Ƿ��ս��,�Ͱ��ұ��Ǹ�Ԫ�ص�first���������Ԫ�ص�first��,���ս���Ļ�Ҳû�£������Ѿ�����ֵ��
			for (set<string>::const_iterator it = firstSet[productions[i].genera[0]].cbegin(); it != firstSet[productions[i].genera[0]].cend(); it++) {

				firstSet[productions[i].nonTerminalSym].insert(*it);
			}

			curSize = firstSet[productions[i].nonTerminalSym].size();
			if (curSize > preSize) changed = true;

		}
	} while (changed);
	//��飬������з��ս���ŵ�first��
	//for (map<string, set<string>>::iterator iter = firstSet.begin(); iter != firstSet.end(); iter++) {
		//cout << iter->first << endl;
		//for (set<string>::iterator it = iter->second.begin(); it != iter->second.end(); it++)
		//{
			//cout << *it << endl;
		//}
		//cout << endl;
	//}

}

/*˵�����ҵ��ķ������Ѿ�ȥ���˿ռ����������ڲ���Ҫ��follow���Ϻ�select���ˣ�
��ϰһ��select���Ķ����ǵ���first�������first��Ϊ�յĻ����͵����Ǹ�����ʽ��Ԫ�ص�follow��
��������һ���ǻ�ȡÿ������ʽ��first���͹���Ԥ�������
*/ 
void getProductionFirstset() {
	for (int i = 0; i < productionSum; i++) {
		for (set<string>::const_iterator it = firstSet[productions[i].genera[0]].cbegin(); it != firstSet[productions[i].genera[0]].cend(); it++) {
			//����ÿ������ʽ�����Ҳ���һ��Ԫ�ص�first��������Ӧ����ʽ,��Ϊ�Ѿ�������ʽ��
			productionFirstSet[i].insert(*it);
		}
	}
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



	


