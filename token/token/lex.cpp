#include "lex.h"
#include "Utilize.h"
/*对grammar的注释
S：开始符号
A：句子
B：block，模块，比如while板块，if板块
D:int或者float
I：ID
N:一个数，整数或者浮点数
L:later，后续，针对在定义时就已经赋值的情况

*/
//用#表示结束，用$表示空

set<string> terminalSym; //定义终结符号集
set<string> nonterminalSym;//定义非终结符号集
string startToken;//存储文法的开始符号
string totalProductions[100];//存放文法文件的每一行，存储的内容跟下面这个相同，为了便于输出
production productions[100];//存储所有的产生式 
int productionSum;//产生式数目 
fstream grammarFile; //文法文件
map<string, set<string>> firstSet;//存所有文法符号的first集合，注意map的用法
map<int, set<string>> productionFirstSet;//存储所有产生式的first集合
map<string, set<string>> followSet;//存所有文法的follow集合
map<int, set<string>> selectSet;//存所有的select集合
map<string, map<string, string>> predictionTable;//LL1的那个表，预测分析表
fstream parseFile("parse.txt");//存放语法分析结果文件
ofstream setFile("setFile.txt");//存放LL1分析表和各种集合表


stack<string> anaStack;
stack<tokenInfo> remainTok;
int top;  //分析栈栈顶指针



void init() {
	saveTokenRes(resultTok, setFile);
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

		list <string> perProRight = split(productionRight, " ");//将产生式右部划分成一个个的单个符号

		//遍历这个list，然后把产生式部分加到这个全局里面存着，这里用了vector的pushback函数，pushback函数作用是在当前vector的尾部加上一个元素

		for (list<string>::const_iterator it = perProRight.cbegin(); it != perProRight.cend(); ++it) {
			productions[productionSum - 1].genera.push_back(*it);
		}

		//将非终结符名存入非终结符集中
		if (nonterminalSym.find(noneTerminalSymbol) == nonterminalSym.end()) {//没找到，所以就要加进去
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
			if ((terminalSym.find(tmp) == terminalSym.end()) &&(
				nonterminalSym.find(tmp) == nonterminalSym.end())) {
				terminalSym.insert(tmp);
			}
		}

	}
	terminalSym.insert("#");
	terminalSym.insert("$");    //空字

	saveNoneTerminal(nonterminalSym, setFile);
	saveTerminal(terminalSym, setFile);

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
		for (int i = 0; i < productionSum; ++i) {  //对于每一个产生式
			preSize = firstSet[productions[i].nonTerminalSym].size();   //某个非终结符对应的first集中元素个数（初始值应该为0）
			bool shouldContinue = true;     //是否继续求此产生式的first集

			for (int j = 0; j < productions[i].genera.size() && shouldContinue; ++j) {  //遍历产生式右部的每个元素 S->ABc
				
				for (set<string>::const_iterator ite = firstSet[productions[i].genera[j]].cbegin(); 
					ite != firstSet[productions[i].genera[j]].cend(); ite++) {         //获得产生式右部每个元素的first集中的元素，以A为例，先获得A的first集
					
					if (firstSet[productions[i].nonTerminalSym].find(*ite) ==firstSet[productions[i].nonTerminalSym].end() && *ite != "$") 
					{
						firstSet[productions[i].nonTerminalSym].insert(*ite);           //将A的first集元素都加到S的first集中,除了空字
					}

				}
				if (firstSet[productions[i].genera[j]].find("$") ==
					firstSet[productions[i].genera[j]].end()) {
					shouldContinue = false;                                                 //如果A的first集中没有空字，则结束求此产生式的first集。否则继续对B操作
				}
			}
			if (shouldContinue) firstSet[productions[i].nonTerminalSym].insert("$");//若最后一个元素的first集仍有空字，则将空字也加入此产生式左部非终结符的first集中
			curSize = firstSet[productions[i].nonTerminalSym].size();
			if (curSize > preSize) changed = true;
		}
	} while (changed);  //若first集改变了，则继续重复操作
	//存到文件中，便于检查
	saveFirst(firstSet, setFile);

}


void getProductionFirstset() {
	int i, j;
	bool has$;

	for (i = 0; i < productionSum; i++) {
		has$ = true;
		for (j = 0; j < productions[i].genera.size() && has$; j++) {    //遍历产生式右部，如S->ABc,先看first(A),并且包含有空集
			for (set<string>::const_iterator ite = firstSet[productions[i].genera[j]].cbegin(); ite != firstSet[productions[i].genera[j]].cend(); ite++) 
			{//遍历右部每个元素的first集？
				if (*ite != "$") {
					productionFirstSet[i].insert(*ite);
				}
			}

			//如果当前符号的first中没有空字，则求产生式first集可以结束了
			if (firstSet[productions[i].genera[j]].find("$") == firstSet[productions[i].genera[j]].end()) {
				has$ = false;
				break;
			}
		}
		//如果遍历到产生式右部末尾，且最后一个符号的first集仍有空字，则迫不得已将空字也加入产生式的first集中，只有靠follow集了
		if (j == productions[i].genera.size() && has$) {
			productionFirstSet[i].insert("$");
		}
	}

	//保存到文件中
	saveProductionFirst(productionFirstSet, setFile);
}

//求所有非终结符的follow集合
void getFollowSet() {
	followSet[startToken].insert("#");  //文法的开始符号的follow集中包含{#}
	int preSize;    //非终结符之前的follow集元素个数
	int curSize;    //非终结符现在的follow集元素个数
	bool changed;   //非终结符的follow集是否改变
	bool has$;  //是否有空字
	int i, j, k;//i记录了产生式的序号，j记录了右部非终结符的位置
	int lastNoneTerminal = -1;   //产生式右部最后一个非终结符的位置

	do {
		changed = false;
		for (i = 0; i < productionSum; ++i) {
			for (j = 0; j < productions[i].genera.size(); ++j) {    //对于每条产生式,遍历它的右部，如 A->aBβα;另外假设如果是A->aB的情况
				//一旦在右部找到了非终结符,举例B，则求B的follow集
				if (nonterminalSym.find(productions[i].genera[j]) != nonterminalSym.end()) {

					preSize = followSet[productions[i].genera[j]].size();   //先求此时B的follow集元素个数

					for (k = j + 1; k < productions[i].genera.size(); ++k) {  //遍历B之后的文法符号，即βα
						has$ = false;
						for (set<string>::const_iterator ite = firstSet[productions[i].genera[k]].cbegin();ite != firstSet[productions[i].genera[k]].cend(); ite++) 
						{ 
							//遍历这个符号的first集，先假设都不为空，只要这个不为空，就把这个符号加到follow集中
							if (*ite == "$") {
								has$ = true;
							}
							else if (followSet[productions[i].genera[j]].find(*ite) ==followSet[productions[i].genera[j]].end() && *ite != "$") 
							{
								followSet[productions[i].genera[j]].insert(*ite);
							}
						}
						if (!has$) {    //这个循环过后，如果不含空集了，说明这个文法符号B的follow集已经求完了
							break;
						}
					}   //继续遍历之后的文法符号

					if (k == productions[i].genera.size() && has$) {    //如果读到产生式末尾，并且最后一个仍包含空字，即B后为空，则把follow(A)也加入follow(B)中
						for (set<string>::const_iterator ite = followSet[productions[i].nonTerminalSym].cbegin();ite != followSet[productions[i].nonTerminalSym].cend(); ite++) 
						{
							if (followSet[productions[i].genera[j]].find(*ite)==followSet[productions[i].genera[j]].end()) 
							{
								followSet[productions[i].genera[j]].insert(*ite);
							}
						}
					}
					curSize = followSet[productions[i].genera[j]].size();   //再求此时B的follow集元素个数
					if (curSize > preSize) changed = true;

					lastNoneTerminal = j;   //记录最后产生式最后一个非终结符的位置
				}
			}
			//上面已经遍历完了第i条产生式的右部

			if (lastNoneTerminal == j - 1) {//如果产生式最后一个符号是非终结符，则该follow集将无法由上述步骤求出。因此直接将follow(A)加入该follow集中
				for (set<string>::const_iterator ite = followSet[productions[i].nonTerminalSym].cbegin();ite != followSet[productions[i].nonTerminalSym].cend(); ite++) {
					if (followSet[productions[i].genera[lastNoneTerminal]].find(*ite) ==followSet[productions[i].genera[lastNoneTerminal]].end()) {
						followSet[productions[i].genera[lastNoneTerminal]].insert(*ite);
					}
				}
				lastNoneTerminal = -1;      //重新恢复初始值
			}
			else {
				lastNoneTerminal = -1;      //重新恢复初始值
			}
		}
	} while (changed);  //若follow集改变了，则继续重复操作

	//保存到文件中
	saveFollow(followSet, setFile);
}

//求select集
//求所有产生式的select集合
void getSelectSet() {
	int i, j;

	for (i = 0; i < productionSum; i++) {
		//对于A->a
		//若a不能推出空字，则select(A->a) = first(a)
		if (productionFirstSet[i].find("$") == productionFirstSet[i].end()) 
		{
			for (set<string>::const_iterator ite = productionFirstSet[i].cbegin();ite != productionFirstSet[i].cend(); ite++) {
				selectSet[i].insert(*ite);
			}
		}
		else {    
			//如果a能推出空字，则select(A->a) = (first(a) - {$}) U follow(A)
			for (set<string>::const_iterator ite = productionFirstSet[i].cbegin();ite != productionFirstSet[i].cend(); ite++) {
				
				if (*ite != "$") {
					selectSet[i].insert(*ite);
				}
			}
			//先取出产生式左部非终结符
			string none = productions[i].nonTerminalSym;
			//把follow(A)加入select中
			for (set<string>::const_iterator ite = followSet[none].cbegin();ite != followSet[none].cend(); ite++) {
				
				selectSet[i].insert(*ite);
			}
		}
	}

	//保存到文件中
	saveProductionSelect(selectSet, setFile);
}

void getPredictTable() {
	//用一个map来存储
	int i;
	for (i = 0; i < productionSum; i++) {
		//遍历产生式
		//遍历第i个产生式的select集，并且将第i条文法存储到predictTable[S，a]（S->aB）
		for (set<string>::const_iterator it = selectSet[i].cbegin(); it != selectSet[i].cend();it++) {

			predictionTable[productions[i].nonTerminalSym][*it] = totalProductions[i];
		}
	}

	//存储
	savePredictionTable(predictionTable, setFile);
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

	for (int i = 0;;i++) {
		parseFile << "第" << i + 1 << "步：" << endl;
		parseFile << "分析栈：";
		
		parseFile << "符号串栈:";


		//如果分析栈和输入串都只剩下#,分析完成
		if (anaStack.top()=="#" && remainTok.top().content == "#") {
			cout << "语法分析完成" << endl;
			break;
		}
		else if (nonterminalSym.find(anaStack.top()) != nonterminalSym.end()) {
			//如果分析栈顶是非终结符
			//检测预测表中是否有产生式匹配子串栈的栈顶
			//前提子串栈顶是个终结符，如果满足条件且找到了
			if (terminalSym.find(EToString(remainTok.top().type)) != terminalSym.end() && predictionTable[anaStack.top()].find(EToString(remainTok.top().type)) != predictionTable[anaStack.top()].end()) 
			{
				cout << "action:" << predictionTable[anaStack.top()][EToString(remainTok.top().type)] << endl << endl;
				//获取产生式右部
				usedProduction = split(predictionTable[anaStack.top()][EToString(remainTok.top().type)], ":= ");
				list<string>::iterator ite = usedProduction.begin();
				ite++;
				//获取右部的每一项
				usedProductionRight = split(*ite, " ");

				//分析栈弹出并且将产生式右部进栈
				anaStack.pop();
				for (auto ite = usedProductionRight.rbegin(); ite != usedProductionRight.rend(); ite++ ) {
					//从右向左读入产生式，并将其入栈，符合之前的习惯,如果是空的话就不进栈
					if ((*ite) != "$") {
						anaStack.push(*ite);
					}
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
			if (anaStack.top()== EToString(remainTok.top().type)) {
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



	


