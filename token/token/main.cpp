#include "token.h"
#include "lex.h"

int line;
errorNode *herr=new errorNode();

int main() {
	
	//���ж�ȡ�ļ����дʷ�����
	fstream in1("test.txt");
	cout << "�ʷ�����������£�" << endl;
	string ss;
	line = 0;
	while (in1.peek() != EOF)
	{
		line++;
		getline(in1, ss);
		token_ana << line << " " << ss << endl;
 		getToken(ss);
		
		token_ana << endl;
		
		
	}
	if (in1.peek() == EOF) {
		//���˵�Ѿ����ļ�β��,������Ȼ���ڶ���ע�͵Ļ�����
		if (state == INMULCOM1 || state == INMULCOM2) {
			hasError = true;
			errt.content = "ȱ��*/";
			errt.line = line;
			err.push_back(errt);
		}
		
	}
	

	//��ʼ�﷨����,���д����ȸĴ�
	
	if (hasError) {
		cout << "�ʷ��������д����token,�����޸�:" << endl;
		for (list<errorNode>::iterator it = err.begin(); it != err.end(); it++) {
			cout << "λ��" << it->line << "�еĴ���" << it->content << endl;
		}
	}else{
		
		saveProduction("grammar.txt");
		getFirstSet();
		getProductionFirstset();
		getFollowSet();
		getSelectSet();
		getPredictTable();
		analyse();


	}

	cin.get();
	cin.get();
}