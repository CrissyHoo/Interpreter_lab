#include "semantic.h"
#include <sstream>
//˼·�Ǳ����﷨����Ȼ��������Ӧ�Ľڵ�Ӧ�ø����Ӧ��ֵ���ڵ�����������ڵ㴴����Ϻ�ɶ�����и�ֵ
#define MAXLENGTH  100
static int level=0;//�����Ĳ����������ĸ�������
int el = 0;//�ڼ�����ʽʱ���ʽ�ĳ���
treeNode expre[MAXLENGTH];//��E�����Ҷ�ӽڵ�
symbolArr ta;
void executePro(treeNode* s) {
	//����ǶԿ�ʼ�ڵ�Ĳ���
	for (int i = 0; i < s->childNum; i++) {
		executeStmt(s->children[i]);
	}
	
}

void executeStmt(treeNode* node) {
	//�����кܶ��ֲ�ͬ�Ľڵ㣬���ݽڵ����͵Ĳ�ͬ��ȡ��ͬ�Ĳ���
	switch (node->NTtype) {
	case DECLARENODE:
		executeDeclar(node, level);
		break;
	case ASSIGNNODE:
		executeAssign(node);
		break;
	case IFNODE:
		executeIf(node);
		break;
	case WHILENODE:
		executeWhile(node, level);
		break;
	case READNODE:
		executeIn(node, level);
		break;
	case WRITENODE:
		executeOut(node, level);
		break;
	case STMTBLOCK:
		executeStateB(node, level);
		break;
	}
}

void executeDeclar(treeNode* node,int lev) {
	//�������Ļ���Ҫ�ӵ�symbol��
	symbol news;
	value*va=new value();//1. ������ת����size��������һ��str������value�У�����valueת��size
	news.setLevel(lev);
	int curType = 0;
	//Q->T V IN�����ifelse�Ǹ�������
	if (node->children[0]->children[0]->tokenStr=="INT") {
		news.setType(news.INTTYPE);//0
		curType = 0;
	}
	else {
		news.setType(news.REALTYPE);//1
		curType = 1;
	}
	
	//V:= I D ���if else�Ǽ�¼�����������ֵ�Ļ���Ҫ���и�ֵ
	news.setName(node->children[1]->children[0]->children[0]->VTcontent);
	//Ȼ���ж��ǲ�������
	if (node->children[1]->children[1]->tokenStr == "$") {
		//���Ϊ��ͨ�����������ǲ�֧�ֶ���������������ģ�����ÿ������ֻ����һ���������Ž�ȥ
		ta.table.push_back(news);
		//���������ʱ�и�ֵ����и�ֵ,���鿴IN  IN->ASSIGN L L�ķ�չֻ�����ֿ��ܣ�һ����E����һ��������ĸ�ֵ�����Ĭ����E�����ָ�ֵ��
		if (node->children[2]->tokenStr != "$") {
			value* result = executeExpr(node->children[2]->children[1]->children[0]);

			//�˺�Ҫ������������벻�ȵ�����Դ�����ʱ����
		}
	}
	else{
		//������
		//set size
		if (node->children[1]->children[1]->children[1]->children[0]->children[0]->tokenStr == "INT_NUM") {
			//�����С������һ������0����
			convert2Num(node->children[1]->children[1]->children[1]->children[0]->children[0]->VTcontent,"int", va);
			if (va->getIntValue() > 0) {
				news.setSize(va->getIntValue());
			}
			else {
				cout << "the array size is wrong! in line " << node->Line;
				abort();
			}
		}
		else {
			cout << "the array size is wrong! in line " << node->Line;
			abort();
		}
		
		ta.arrTable.push_back(news);//����ֻ�洢�������ͺ�id,���������Ķ�����ֱ�Ӵ浽��arraytable��
		//���������ʱ�и�ֵ����и�ֵ,���鿴IN  IN->ASSIGN L
		if (node->children[2]->children[0]->tokenStr != "$") {
			//IN��Ϊ�գ�Ҫ���и�ֵ
			/*L: = LEFT_BOUND MN RIGHT_BOUND
			MN : = N NB
			NB : = COMMA N NB
			NB : = $*/
			//����ĳ�ʼ��ֻ����������ĳ�ʼ����������{2,3,4},δ��ʼ����Ĭ��Ϊ0
			//���Ҹ���curType��ֵ
			treeNode* tmp = node->children[2]->children[1]->children[1];//MN
			//��һ��ʽ����Ҫ����Դ���������ģʽ��һ����
			if (curType == 0 ) {//int����

					if (tmp->children[1]->children[0]->tokenStr != "$") {
						int i;
						stringstream convert;
						try{
						convert << tmp->children[0]->children[0]->VTcontent;
						convert >> i;
						news.addVecInt(i);
						tmp = tmp->children[1];//ָ���˵ڶ������ʽ��NB
						}
						catch (exception e) {
							//������Ҫ�������쳣��i��ֵ��int�����������һ��int���鵫�Ǹ�ֵ������float
							cout <<"in line "<<tmp->Line<< " have exceptions:you may have errors with type convert."<<endl;
						}
					}
					while (tmp->children[2]->children[0]->tokenStr!="$") {
						//���ڶ������ʽ֮����ʽ�ȶ���
						if (news.intA.size() >= news.getSize()) {
							//����Խ���ˣ������ٳ�ʼ��
							cout << "array is out of bound" << endl;
							abort();
						}
						int i;
						stringstream convert;
						try {
							convert << tmp->children[1]->children[0]->VTcontent;
							convert >> i;
							news.addVecInt(i);
							tmp = tmp->children[2];//ָ���˵ڶ������ʽ��NB
						}
						catch (exception e) {
							//������Ҫ�������쳣��i��ֵ��int�����������һ��int���鵫�Ǹ�ֵ������float
							cout << "in line " << tmp->Line << " have exceptions:you may have errors with type convert." << endl;
						}
					}
			}
			else {
			//double����
				
				if (tmp->children[1]->children[0]->tokenStr != "$") {
					int i;
					stringstream convert;
					try {
						convert << tmp->children[0]->children[0]->VTcontent;
						convert >> i;
						news.addVecInt(i);
						tmp = tmp->children[1];//ָ���˵ڶ������ʽ��NB
					}
					catch (exception e) {
						//������Ҫ�������쳣��i��ֵ��int�����������һ��int���鵫�Ǹ�ֵ������float
						cout << "in line " << tmp->Line << " have exceptions:you may have errors with type convert." << endl;
					}
				}
				while (tmp->children[2]->children[0]->tokenStr != "$") {
					//���ڶ������ʽ֮����ʽ�ȶ���
					if (news.douA.size() >= news.getSize()) {
						//����Խ���ˣ������ٳ�ʼ��
						cout << "array is out of bound" << endl;
						abort();
					}
					double i;
					stringstream convert;
					try {
						convert << tmp->children[1]->children[0]->VTcontent;
						convert >> i;
						news.addVecDou(i);
						tmp = tmp->children[2];//ָ���˵ڶ������ʽ��NB
					}
					catch (exception e) {
						//������Ҫ�������쳣��i��ֵ��int�����������һ��double���鵫�Ǹ�ֵ������int
						cout << "in line " << tmp->Line << " have exceptions:you may have errors with type convert." << endl;
					}
				}
			}
			}
		}
}

void getLeaves(treeNode* node) {
	if (node->childNum == 0) {//��������Ҷ�ӽڵ�
		expre[el].VTcontent = node->VTcontent;   //VTcontent���Ǿ����ֵ����ID��˵����a,apple֮��ģ���������־���2,3.4֮������ֵ,��һ��ȱ�������еĶ�����stringȥ�洢��
		el++;//tokenstr�����ս����Ӧ��type
	}
	int i = 0;
	for (i; i < node->childNum; i++) {
			getLeaves(node->children[i]);
		}
	}
	

bool isOperator(treeNode node) {
	//�ж϶����������ǲ��ǲ�����
	if (node.tokenStr == "PLUS" || node.tokenStr == "MINUS" || node.tokenStr == "MUL" || node.tokenStr == "DIV" || node.tokenStr == "MOD"
		|| node.tokenStr == "GRT" || node.tokenStr == "LES" || node.tokenStr == "GRT_EQU" || node.tokenStr == "LES_EQU" || node.tokenStr == "EQU" || node.tokenStr == "NOT_EQU") {
		return true;
	}
}

value* executeExpr(treeNode* node){
	//��������������Ǽ�����ʽ��ֵ�����и�ֵ�����ʽ��ֵ��������ͨ���ʽ��ֵ�Ͳ���ֵ�ı��ʽ����ӦE��OE
	value* va = new value();
	value* tmp1, *tmp2;
	tokenType op;
	/*
	E:= IT M
	IT:= F FR
	FR:= MUL F FR
	FR:= DIV F FR
	FR:= MOD F FR
	F:= LEFT_BRA E RIGHT_BRA
	F:= V
	F:= N
	F:= $
	FR:= $
	M:= PLUS IT M
	M:= MINUS IT M
	M:= $
	*/
	//˼·���ñ��ʽ��ֵ���㷨����������ջ��Ȼ����ֵ�󷵻�һ��valueֵ�����Կ���E��Ҫ�õ����ʽ�����ʽ������EΪ���ڵ��Ҷ�ӽڵ㣬���ǽ�������һ��������
	//������ջ��һ���ǲ�����ջ��һ���ǲ�����ջ
	stack<value*> numbers;//���ڱ���Ҫ�õ�����ֵ����������ֵĻ���ֱ����һ������
	stack<tokenType> operators;
	int i = 0;
	for(i; i <= el+1; i++) {
		treeNode x = expre[i];
		if (i == el+1) {
			//������һ��Ԫ���Ѿ��������˵����
			if (operators.empty()) {
				//�յĻ�����true
				va = numbers.top();
				return va;
			}
			else {
				tmp1 = numbers.top();
				numbers.pop();
				tmp2 = numbers.top();
				numbers.pop();
				op = operators.top();

				operators.pop();
				va = executeOp(op,tmp1,tmp2);
				numbers.push(va);
				if ( x.VTcontent== ")")//����������������������ţ������������������
				{
					operators.pop();
				}
			}

		}
		else if (!isOperator(x)) {
			//���x����һ���������Ļ�
			value*tmp=new value();
			if (x.tokenStr == "INT_NUM") {
				tmp->setType(tmp->INTTYPE);
				convert2Num(x.VTcontent, "int", tmp);
				numbers.push(tmp);
			}
			else if (x.tokenStr == "REAL_NUM") {
				tmp->setType(tmp->REALTYPE);
				convert2Num(x.VTcontent, "double", tmp);
				numbers.push(tmp);
			}else if (x.tokenStr == "ID") {
				//�����һ����ʶ���Ļ�,��Ҫȥ���ҷ��ű�����û������Ķ��壬���л������Ӧ�Ĵ�����Ȼ�������Ļ�Ҫ���ر�ʶ����Ӧ��ֵ
				symbol* sym=ta.searchTa(x.VTcontent, level);
				if (sym != NULL) {
					if (sym->getType() == sym->INTTYPE) {
						//��һ��intֵ
						tmp->setIntValue(sym->getIntV());

					}
					else {
						//��һ��doubleֵ
						tmp->setDouValue(sym->getDouV());
					}
					numbers.push(tmp);
				}
				else {
					//������
				}
			}
			
		}
		else if (isOperator(x)) {//����������һ������
			//���ջ�ǿյĵĻ���ֱ��push
			if (operators.empty() == true) {
				operators.push(str2Token(x.tokenStr));
			}
			else {
				//ջ�ڲ�����Ƚ����ȼ�
				if (getPerior(operators.top()) >= getPerior(str2Token(x.tokenStr))) {
					//������ȼ���Ȼ���top>��ȥ
					if (operators.top() == LEFT_BRA) {
						//���top�������ŵĻ�����Ȼ���ȼ��϶��������ŵͣ�������ֱ�ӽ�ȥ
						operators.push(str2Token(x.tokenStr));
					}
					else {
						//��ͨ�������Ҫ��������
						tmp1 = numbers.top();
						numbers.pop();
						tmp2 = numbers.top();
						numbers.pop();
						op = operators.top();
						operators.pop();
						va = executeOp(op,tmp1, tmp2);
						numbers.push(va);
						if (x.VTcontent == ")")//����������������������ţ������������������
						{
							operators.pop();//�������Ÿ����
						}
					}

				}
				else if (getPerior(operators.top()) < getPerior(str2Token(x.tokenStr))) {
					//��ȥ����top
					operators.push(str2Token(x.tokenStr));

				}
			}

		}
	}
	return va;
	
	
}

void executeAssign(treeNode* node) {
	//AF->V AC  ; 
	//AC:= ASSIGN L SEMI
	//AC: = SEMI
	symbol* tmpsym = executeValue(node, level);
	//��trycatch handle�쳣
	try {
		if (node->children[1]->children[0]->tokenStr == "ASSIGN") {
			if (node->children[0]->children[1]->children[0]->tokenStr == "$") {
				//������ͨ��������
				if (tmpsym->getType() == tmpsym->INTTYPE) {
					tmpsym->setInt(executeExpr(node->children[1]->children[1])->getIntValue());//L�ڵ�
				}
				else {
					tmpsym->setDou(executeExpr(node->children[1]->children[1])->getDouValue());//L�ڵ�
				}
			}
			else {
				//��������
				int index = 0;
				/*V:= I D
				I:= ID
				D:= LEFT_INDEX E RIGHT_INDEX
				F:= E
				F:= V
				F:= N
				F:= $
				*/
				//��ȡҪ������ֵ
				if (executeExpr(node->children[0]->children[1]->children[1])->getType() != 0) {
					cout << "line " << node->Line << ": wrong index" << endl;
					abort();
				}
				else {
					index = executeExpr(node->children[0]->children[1]->children[1])->getIntValue();
					if (index >= tmpsym->getSize()) {
						cout << "illegal access to the array. in line " << node->Line << endl;
					}
				}
				//�����һ��int����
				if (tmpsym->getType() == tmpsym->INTTYPE) {
					tmpsym->intA[index] = executeExpr(node->children[1]->children[1])->getIntValue();//L�ڵ�

				}
				else {
					tmpsym->douA[index]=executeExpr(node->children[1]->children[1])->getDouValue();//L�ڵ�
				}
				
			}
		}
		//�������assign�Ļ����Ǽ򵥵������������κδ���
	}
	catch (exception e) {
		cout << "in line " << node->Line << ": the variable hasn't been defined." << endl;
	}

	 

}

//����ջ������ʽ��ֵ
value* executeOp(tokenType opera, value* op1, value*  op2){
	value *va = new value();
	int type;
	//ȷ���������������ͣ���һ��Ϊdouble�������Ϊdouble
	if (op1->getType() == 1 || op2->getType() == 1) {
		//1��ʾdouble����
		type =1;
	}
	else {
		type = 0;
	}
	va->setType(type);
	//��������������������
	switch (opera) {
	case PLUS:
		try {
			if (type == 0) {//int
				va->setIntValue(op1->getIntValue() + op2->getIntValue());
			}
			else {
				//���������������ȫ��ת��Ϊdouble����
				va->setDouValue(op1->getDouValue() + op2->getDouValue());
			}
		}
		catch (exception e) {
			cout << "you may have problems about casting." << endl;
		}
		break;
	case MINUS:
		if (type == 0) {//int
			va->setIntValue(op1->getIntValue() - op2->getIntValue());
		}
		else {
			//���������������ȫ��ת��Ϊdouble����
			va->setDouValue(op1->getDouValue() - op2->getDouValue());
		}
		break;
	case MUL:
		if (type ==0) {//int
			va->setIntValue(op1->getIntValue()*op2->getIntValue());
		}
		else {
			//���������������ȫ��ת��Ϊdouble����
			va->setDouValue(op1->getDouValue()*op2->getDouValue());
		}
		break;
	case DIV:
		//���������
		if (op2->getDouValue() == 0.0||op2->getIntValue()==0) {
			cout << "��������Ϊ0" << endl;
		}
		if (type == 0) {
			va->setIntValue(op1->getIntValue() / op2->getIntValue());
		}
		else {
			//���������������ȫ��ת��Ϊdouble����
			va->setDouValue(op1->getDouValue() / op2->getDouValue());
		}
		break;
	case MOD:
		if (op2->getDouValue() == 0.0 || op2->getIntValue() == 0) {
			cout << "��������Ϊ0" << endl;
		}
		if (op1->getType() == 1 || op2->getType() == 1) {//����double
			va->setDouValue(0.0);
		}
		else {
			va->setIntValue(op1->getIntValue() - op1->getIntValue() / op2->getIntValue()*op2->getIntValue());
		}
	case GRT:
		if (op1->getDouValue() > op2->getDouValue()) {//��doubleֵΪ׼�����бȽ�,�и����⣬���double����Ҫ��һ��ʼ����int�أ�����
			va->setType(va->TRUETYPE);//error ������һ��С���⣬����һ��
		}
		else {
			va->setType(va->FALSETYPE);
		}
		break;
	case LES:
		if (op1->getDouValue() < op2->getDouValue()) {
			va->setType(va->TRUETYPE);
		}
		else {
			va->setType(va->FALSETYPE);
		}
		break;
	case EQU:
		if (op1->getDouValue() == op2->getDouValue()) {
			va->setType(va->TRUETYPE);
		}
		else {
			va->setType(va->FALSETYPE);
		}
		break;
	case NOT_EQU:
		if (op1->getDouValue() != op2->getDouValue()) {
			va->setType(va->TRUETYPE);
		}
		else {
			va->setType(va->FALSETYPE);
		}
		break;
	case GRT_EQU:
		if (op1->getDouValue() >= op2->getDouValue()) {
			va->setType(va->TRUETYPE);
		}
		else {
			va->setType(va->FALSETYPE);
		}
		break;
	case LES_EQU:
		if (op1->getDouValue() <= op2->getDouValue()) {
			va->setType(va->TRUETYPE);
		}
		else {
			va->setType(va->FALSETYPE);
		}
		break;
	}
	return va;
}

void executeIf(treeNode* node) {
	/*
	OE:= E O E
	C:= IF CB
	CB:= LEFT_BRA OE RIGHT_BRA CF EL
	EL:= ELSE CF
	EL:= $
	CF��һ���µĳ����*/
	value* con1 = executeExpr(node->children[1]->children[1]->children[0]);
	value*con2= executeExpr(node->children[1]->children[1]->children[2]);
	value* res = executeOp(str2Token(node->children[1]->children[1]->children[1]->children[0]->VTcontent), con1, con2);
	//true�ͷ�0ֵʹ����Ϊ��,����CF
	if (res->getType() == res->TRUETYPE) {
		executeStateB(node->children[1]->children[3]->children[2]->children[1], level);//CF
	}
	else {
		if (node->children[1]->children[4]->children[0]->tokenStr!="$") {
			//�����else
			executeStateB(node->children[1]->children[4]->children[1]->children[1], level);//CF
		}
	}
	
}

void executeWhile(treeNode* node, int lev) {
	/*
	WL:= WHILE WLB
	WLB:= LEFT_BRA OE RIGHT_BRA CF*/
	value* con1 = executeExpr(node->children[1]->children[1]->children[0]);
	value* con2 = executeExpr(node->children[1]->children[1]->children[2]);
	value*res = executeOp(str2Token(node->children[1]->children[1]->children[1]->children[0]->VTcontent), con1, con2);
	while (res->getType() == res->TRUETYPE ) {
		executeStateB(node->children[1]->children[3]->children[1], level);
	
		
	}
	

	
}
void executeIn(treeNode* node, int lev) {

}
void executeOut(treeNode* node, int lev) {

}
void executeStateB(treeNode* node, int lev) {
	//��������������ǿ��Ǹ������
	level++;//��������
	for (int i = 0; i < node->childNum;i++) {
			executeStmt(node);
		
	}
	
}


//����������������ڷ��ű��в�����Ӧ�ı���������ҵ��ˣ�������Ӧ��symbol�ڵ㣬����һ��������ԣ�
//û�ҵ��ͱ������������������ȥsybolarr����������ң�û�ҵ��ͱ����򵥴ֱ���
//��������Ѿ������������ķ��������ˣ������ں���BB�ķ�������AF�ڵ�
/* 
    AF: = V AC
	AC : = ASSIGN L SEMI
	AC : = SEMI
	*/
symbol* executeValue(treeNode* node, int level) {//ע���������ǲ�����value�ڵ�ģ�
	symbol* tmp;
	//AF->V AC
	//AC:= ASSIGN L SEMI
	//AC: = SEMI
	//��������Ƿ�ֵ��value�Ĺ���ֻ����Ա������Ҳ�����
		if ((node->children[0]->children[1]->children[0]->tokenStr == "$"))
		{
			//DΪ�������Ǹ���ͨ������ֵ�����Ǹ�ֵ���������Ǽ򵥵��ٴ�����һ��
			tmp = ta.searchTa(node->children[0]->children[0]->children[0]->VTcontent, level);
		}
		else {
			//���������
			tmp = ta.searchArr(node->children[0]->children[0]->children[0]->VTcontent, level);
		}
	
	return tmp;
}

