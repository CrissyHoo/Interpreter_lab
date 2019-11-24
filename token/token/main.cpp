#include "token.h"
#include "lex.h"

int line;
errorNode *herr=new errorNode();

int main() {
	
	//逐行读取文件进行词法分析
	fstream in1("test.txt");
	cout << "词法分析结果如下：" << endl;
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
		//如果说已经到文件尾了,但是仍然处在多行注释的环境中
		if (state == INMULCOM1 || state == INMULCOM2) {
			hasError = true;
			errt.content = "缺少*/";
			errt.line = line;
			err.push_back(errt);
		}
		
	}
	

	//开始语法分析,若有错误先改错
	
	if (hasError) {
		cout << "词法分析中有错误的token,请先修改:" << endl;
		for (list<errorNode>::iterator it = err.begin(); it != err.end(); it++) {
			cout << "位于" << it->line << "行的错误：" << it->content << endl;
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