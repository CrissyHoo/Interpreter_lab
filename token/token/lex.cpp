#include "lex.h"
#include "Utilize.h"


set<string> terminalSym; //定义终结符号集
set<string> nonterminalSym;//定义非终结符号集
string startToken;//存储文法的开始符号
string totalProductions[100];//存放文法文件的每一行，最大的文法条数是100
production productions[100];//存储所有的产生式 
int productionSum;//产生式数目 
fstream grammarFile; //文法文件
map<string, set<string>> firstSet;//存所有文法符号的first集合，注意map的用法
map<int, set<string>> productionFirstSet;//存储所有产生式的first集合
//map<string, set<string>> followSet;//存所有文法的follow集合
//map<string, set<string>> selectSet;//存所有的select集合
map<string, map<string, string>> predictionTable;//LL1的那个表，预测分析表


stack<string> anaStack;
stack<tokenInfo> remainTok;
int top;  //分析栈栈顶指针

void init() {
	//清空所有数据
	terminalSym.clear();
	nonterminalSym.clear();
	for (int i = 0; i < 100; i++) {
		productions[i].genera.clear();
	}
}

//将文法文件中的产生式存储在数据结构中
void saveProduction(string grammarName) {
	init();
	char perProduction[100];   //每条完整产生式
	char *pos = NULL;//存储字符出现的位置
	string productionRight;      //产生式右部
	string noneTerminalSymbol;   //非终结符

	grammarFile.open(grammarName, ios::in);//以读的形式打开
	if (grammarFile.is_open() != true) {
		cout << "打开文件失败" << endl;
		return;
	}

	productionSum = 0;
	while (!grammarFile.eof()) {//扫描文法文件并存储
		productionSum++;
		grammarFile.getline(perProduction, 100);//getline函数，将读取到的字符串存在第一个参数中
		totalProductions[productionSum - 1] = perProduction;
		pos = strstr(perProduction, ":=");  //返回字符串首先出现：=的位置
		productionRight.replace(productionRight.begin(), productionRight.end(), pos + 3);   //取出产生式右部有个空格，所以+3
		noneTerminalSymbol.replace(noneTerminalSymbol.begin(), noneTerminalSymbol.end(), perProduction,
			pos - perProduction);    //取出产生式左部，也就是非终结符名。

		productions[productionSum - 1].nonTerminalSym = noneTerminalSymbol;//给productions赋值

		list <string> perProRight = split(productionRight, " ");

		//遍历这个list，然后把产生式部分加到这个全局里面存着，这里用了vector的pushback函数，pushback函数作用是在当前vector的尾部加上一个元素

		for (list<string>::const_iterator it = perProRight.cbegin(); it != perProRight.cend(); ++it) {
			productions[productionSum - 1].genera.push_back(*it);
		}

		//将非终结符名存入非终结符集中
		if (nonterminalSym.find(noneTerminalSymbol) == nonterminalSym.end()) {//一个都没找到
			nonterminalSym.insert(noneTerminalSymbol);
		}
		//默认文法第一个产生式左部为开始符
		if (productionSum == 1) {
			startToken = noneTerminalSymbol;
		}
	}

	//存储完了之后存储终结符号集
	string tmp;
	for (int i = 0; i < productionSum; i++) {
		//遍历所有的产生式右部，利用刚刚分割得到的数据
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

//求所有符号的first集合
void getFirstSet() {
	//对于终结符来说，first就是它本身
	for (set<string>::const_iterator it = terminalSym.cbegin(); it != terminalSym.cend(); it++) {
		firstSet[*it].insert(*it);
	}
	//对与非终结符而言
	int preSize;
	int curSize;//当前的size
	bool changed;//这个变量用来检测是否对first集合做了改变
	do {
		changed = false;
		for (int i = 0; i < productionSum; i++) {
			preSize = firstSet[productions[i].nonTerminalSym].size();

			//如果产生式右部的第一个元素是非终结符,就把右边那个元素的first集赋给这个元素的first集,是终结符的话也没事，上面已经赋过值了
			for (set<string>::const_iterator it = firstSet[productions[i].genera[0]].cbegin(); it != firstSet[productions[i].genera[0]].cend(); it++) {

				firstSet[productions[i].nonTerminalSym].insert(*it);
			}

			curSize = firstSet[productions[i].nonTerminalSym].size();
			if (curSize > preSize) changed = true;

		}
	} while (changed);
	//检查，输出所有非终结符号的first集
	//for (map<string, set<string>>::iterator iter = firstSet.begin(); iter != firstSet.end(); iter++) {
		//cout << iter->first << endl;
		//for (set<string>::iterator it = iter->second.begin(); it != iter->second.end(); it++)
		//{
			//cout << *it << endl;
		//}
		//cout << endl;
	//}

}

/*说明：我的文法里面已经去除了空集，所以现在不需要求follow集合和select集了，
复习一下select集的定义是等于first集，如果first集为空的话，就等于那个产生式左部元素的follow集
所以下面一步是获取每个产生式的first集和构建预测分析表
*/ 
void getProductionFirstset() {
	for (int i = 0; i < productionSum; i++) {
		for (set<string>::const_iterator it = firstSet[productions[i].genera[0]].cbegin(); it != firstSet[productions[i].genera[0]].cend(); it++) {
			//遍历每个产生式，将右部第一个元素的first集赋给对应产生式,因为已经消除空式了
			productionFirstSet[i].insert(*it);
		}
	}
}
void getPredictTable() {
	//预测表最好不要有重复，不然就会构成二义性,我们用一个map来存储
	int i;
	for (i = 0; i < productionSum; i++) {
		//遍历产生式
		//遍历第i个产生式的firstset集，并且将第i条文法存储到predictTable[S，a]（S->aB）
		for (set<string>::const_iterator it = productionFirstSet[i].cbegin(); it != productionFirstSet[i].cend();it++) {

			predictionTable[productions[i].nonTerminalSym][*it] = totalProductions[i];
		}
	}


}

//使用预测分析表进行语法分析
void analyse() {

	list<string> usedProduction;
	list<string> usedProductionRight;
	tokenInfo p;
	p.content = "#";
	

	//给remaintok赋值，读出词法分析结果
	remainTok.push(p);
	for (auto it = resultTok.rbegin(); it != resultTok.rend(); it++) {
		
		remainTok.push(*it);//在这里倒序插入了，所以栈顶的元素应该是最开始的那个符号

	}
	
	

	
	anaStack.push(startToken);//首符号进栈

	while (true) {
		//如果分析栈和输入串都只剩下#,分析完成
		if (anaStack.top()=="#"&&remainTok.top().content == "#") {
			cout << "语法分析完成" << endl;
			break;
		}
		else if (nonterminalSym.find(anaStack.top()) != nonterminalSym.end()) {
			//如果分析栈顶是非终结符
			//检测预测表中是否有产生式匹配子串栈的栈顶
			//前提子串栈顶是个终结符，如果满足条件且找到了
			if (terminalSym.find(remainTok.top().content) != terminalSym.end() &&
				predictionTable[anaStack.top()].find(remainTok.top().content) != predictionTable[anaStack.top()].end()) 
			{
				cout << "action:" << predictionTable[anaStack.top()][remainTok.top().content] << endl << endl;
				//获取产生式右部
				usedProduction = split(predictionTable[anaStack.top()][remainTok.top().content], ":= ");
				list<string>::iterator ite = usedProduction.begin();
				ite++;
				//获取右部的每一项
				usedProductionRight = split(*ite, " ");

				//分析栈弹出并且将产生式右部进栈
				anaStack.pop();
				for (auto ite = usedProductionRight.rbegin(); ite != usedProductionRight.rend(); ite++ ) {
					//从右向左读入产生式，并将其入栈，符合之前的习惯
					anaStack.push(*ite);
				}

			}
			else {
				cout << "错误！当前token不可被识别:" << "位于程序第" << remainTok.top().line << "行的" << remainTok.top().content << endl << endl;
				break;
			}
		}
		else if (terminalSym.find(anaStack.top()) != terminalSym.end()) {
			//如果分析栈栈顶是终结符
			//如果子串栈顶的元素还是终结符，查看是否可以匹配
			if (anaStack.top()== remainTok.top().content) {
				//如果可以匹配了,弹出
				anaStack.pop();
				//剩余串的指针后移一个
				remainTok.pop(); 
				cout << "匹配：" << remainTok.top().content << endl;
			}
			else {
				//分析栈栈顶是终结符并且还不可以匹配
				cout << "出现错误：位于程序第" << remainTok.top().line << "行的" << remainTok.top().content << endl;
				break;
			}
		}
	}
}



	


