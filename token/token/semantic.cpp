#include "semantic.h"
//思路是遍历语法树，然后遇到对应的节点应该赋予对应的值
static int level=0;//遍历的层数
valueArr ta;
void executePro(treeNode s) {
	//这个是对开始节点的操作
	level = 0;
	for (int i = 0; i < s.childNum; i++) {
		executeStmt(*(s.children[i]));
	}
	//后面有一个删除符号表的操作，暂时不知道为什么
}

void executeStmt(treeNode node) {
	//这里有很多种不同的节点，根据节点类型的不同采取不同的操作
	switch (node.type) {
	case DECLARENODE:
		executeDeclar(node, level);
		break;
	case ASSIGNNODE:
		executeAssign(node, level);
		break;
	case IFNODE:
		executeIf(node, level);
		break;
	case WHILENODE:
		executeWhile(node, level);
		break;
	case READNODE:
		executeRead(node, level);
		break;
	case WRITENODE:
		executeWrite(node, level);
		break;
	case STMTBLOCK:
		executeStmtblock(node, level);
		break;
	}
}
void executeDeclar(treeNode node,int lev) {
	//有声明的话就要加到symbol中
	symbol news;
	news.setLevel(lev);
	if (node.children[0].getValue().getType()==INT) {//Q节点，生成T V IN,T是一个value类型的节点
		//如果这是一个int类型数据
		news.setType(news.INTTYPE);
	}
	else {
		news.setType(news.REALTYPE);
	}
	//V:= I D
	news.setName(node.children[1]->children[0]->tokenStr);//有争议
	//然后判断是不是数组
	if (node.children[1]->children[1]->tokenStr == "$") {
		//如果不是数组，这里是不支持多个逗号这样声明的，所以每次声明只会有一个变量被放进去
		ta.table.push_back(news);
		//如果在声明时有赋值则进行赋值,即查看IN  IN->ASSIGN L
		if (node.children[2]->tokenStr != "$") {
			value result = executeExpr(node.children[2]->children[1]->children[0], level);
			//此后还要进行类型相等与不等的区别对待，暂时不管
		}
	}
	else {
		//是数组
		ta.arrTable.push_back(news);//这里只存储变量类型和id,不存其他的东西。
		//如果在声明时有赋值则进行赋值,即查看IN  IN->ASSIGN L
		if (node.children[2]->tokenStr != "$") {
			value result = //另一种赋值方式
		}
	}
	

}

void executeAssign(treeNode node, int lev) {//AF->V AC
	symbol tmpsym = executeValue(*node.children[0], level);
	//如果遇到了assign节点，需要把节点的第一个孩子的值赋给一个变量

	 

}

void executeIf(treeNode node, int lev) {

}

void executeWhile(treeNode node, int lev) {

}
void executeRead(treeNode node, int lev) {

}
void executeWrite(treeNode node, int lev) {

}
void executeStmtblock(treeNode node, int lev) {

}
//value节点的作用是返回相应的符号表变量？？
symbol executeValue(treeNode node, int level) {
	symbol tmp;
	//如果只是普通的变量，直接查找AF->V AC  V->I D
	if (node.childNum == 2 && node.children[0]->children[1]->tokenStr == "$") {
		tmp = ta.search(node.children[0]->children[0]->content, level);
	}
	else {
		//是数组变量

	}
	return tmp;
	 
	
}