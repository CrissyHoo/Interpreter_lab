#include "semantic.h"
//˼·�Ǳ����﷨����Ȼ��������Ӧ�Ľڵ�Ӧ�ø����Ӧ��ֵ
static int level=0;//�����Ĳ���
valueArr ta;
void executePro(treeNode s) {
	//����ǶԿ�ʼ�ڵ�Ĳ���
	level = 0;
	for (int i = 0; i < s.childNum; i++) {
		executeStmt(*(s.children[i]));
	}
	//������һ��ɾ�����ű�Ĳ�������ʱ��֪��Ϊʲô
}

void executeStmt(treeNode node) {
	//�����кܶ��ֲ�ͬ�Ľڵ㣬���ݽڵ����͵Ĳ�ͬ��ȡ��ͬ�Ĳ���
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
	//�������Ļ���Ҫ�ӵ�symbol��
	symbol news;
	news.setLevel(lev);
	if (node.children[0].getValue().getType()==INT) {//Q�ڵ㣬����T V IN,T��һ��value���͵Ľڵ�
		//�������һ��int��������
		news.setType(news.INTTYPE);
	}
	else {
		news.setType(news.REALTYPE);
	}
	//V:= I D
	news.setName(node.children[1]->children[0]->tokenStr);//������
	//Ȼ���ж��ǲ�������
	if (node.children[1]->children[1]->tokenStr == "$") {
		//����������飬�����ǲ�֧�ֶ���������������ģ�����ÿ������ֻ����һ���������Ž�ȥ
		ta.table.push_back(news);
		//���������ʱ�и�ֵ����и�ֵ,���鿴IN  IN->ASSIGN L
		if (node.children[2]->tokenStr != "$") {
			value result = executeExpr(node.children[2]->children[1]->children[0], level);
			//�˺�Ҫ������������벻�ȵ�����Դ�����ʱ����
		}
	}
	else {
		//������
		ta.arrTable.push_back(news);//����ֻ�洢�������ͺ�id,���������Ķ�����
		//���������ʱ�и�ֵ����и�ֵ,���鿴IN  IN->ASSIGN L
		if (node.children[2]->tokenStr != "$") {
			value result = //��һ�ָ�ֵ��ʽ
		}
	}
	

}

void executeAssign(treeNode node, int lev) {//AF->V AC
	symbol tmpsym = executeValue(*node.children[0], level);
	//���������assign�ڵ㣬��Ҫ�ѽڵ�ĵ�һ�����ӵ�ֵ����һ������

	 

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
//value�ڵ�������Ƿ�����Ӧ�ķ��ű��������
symbol executeValue(treeNode node, int level) {
	symbol tmp;
	//���ֻ����ͨ�ı�����ֱ�Ӳ���AF->V AC  V->I D
	if (node.childNum == 2 && node.children[0]->children[1]->tokenStr == "$") {
		tmp = ta.search(node.children[0]->children[0]->content, level);
	}
	else {
		//���������

	}
	return tmp;
	 
	
}