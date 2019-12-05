#include "semantic.h"
#include <sstream>
//思路是遍历语法树，然后遇到对应的节点应该赋予对应的值，节点的类型在树节点创建完毕后可对其进行赋值
#define MAXLENGTH  100
static int level=0;//遍历的层数，变量的覆盖问题
int el = 0;//在计算表达式时表达式的长度
treeNode expre[MAXLENGTH];//存E下面的叶子节点
symbolArr ta;
void executePro(treeNode* s) {
	//这个是对开始节点的操作
	for (int i = 0; i < s->childNum; i++) {
		executeStmt(s->children[i]);
	}
	
}

void executeStmt(treeNode* node) {
	//这里有很多种不同的节点，根据节点类型的不同采取不同的操作
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
	//有声明的话就要加到symbol中
	symbol news;
	value*va=new value();//1. 将数组转化的size（类型是一个str）存在value中，再由value转到size
	news.setLevel(lev);
	int curType = 0;
	//Q->T V IN，这个ifelse是赋予类型
	if (node->children[0]->children[0]->tokenStr=="INT") {
		news.setType(news.INTTYPE);//0
		curType = 0;
	}
	else {
		news.setType(news.REALTYPE);//1
		curType = 1;
	}
	
	//V:= I D 这个if else是记录下来，如果赋值的话就要进行赋值
	news.setName(node->children[1]->children[0]->children[0]->VTcontent);
	//然后判断是不是数组
	if (node->children[1]->children[1]->tokenStr == "$") {
		//如果为普通变量，这里是不支持多个逗号这样声明的，所以每次声明只会有一个变量被放进去
		ta.table.push_back(news);
		//如果在声明时有赋值则进行赋值,即查看IN  IN->ASSIGN L L的发展只有两种可能，一种是E还有一种是数组的赋值这里就默认是E的这种赋值了
		if (node->children[2]->tokenStr != "$") {
			value* result = executeExpr(node->children[2]->children[1]->children[0]);

			//此后还要进行类型相等与不等的区别对待，暂时不管
		}
	}
	else{
		//是数组
		//set size
		if (node->children[1]->children[1]->children[1]->children[0]->children[0]->tokenStr == "INT_NUM") {
			//数组大小必须是一个大于0整数
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
		
		ta.arrTable.push_back(news);//这里只存储变量类型和id,不存其他的东西。直接存到了arraytable里
		//如果在声明时有赋值则进行赋值,即查看IN  IN->ASSIGN L
		if (node->children[2]->children[0]->tokenStr != "$") {
			//IN不为空，要进行赋值
			/*L: = LEFT_BOUND MN RIGHT_BOUND
			MN : = N NB
			NB : = COMMA N NB
			NB : = $*/
			//数组的初始化只允许最基础的初始化，类似于{2,3,4},未初始化的默认为0
			//并且根据curType的值
			treeNode* tmp = node->children[2]->children[1]->children[1];//MN
			//第一个式子需要特殊对待，其他的模式就一样了
			if (curType == 0 ) {//int数组

					if (tmp->children[1]->children[0]->tokenStr != "$") {
						int i;
						stringstream convert;
						try{
						convert << tmp->children[0]->children[0]->VTcontent;
						convert >> i;
						news.addVecInt(i);
						tmp = tmp->children[1];//指向了第二个表达式的NB
						}
						catch (exception e) {
							//这里主要想解决的异常是i的值是int，如果定义了一个int数组但是赋值给的是float
							cout <<"in line "<<tmp->Line<< " have exceptions:you may have errors with type convert."<<endl;
						}
					}
					while (tmp->children[2]->children[0]->tokenStr!="$") {
						//到第二个表达式之后形式稳定了
						if (news.intA.size() >= news.getSize()) {
							//数组越界了，不可再初始化
							cout << "array is out of bound" << endl;
							abort();
						}
						int i;
						stringstream convert;
						try {
							convert << tmp->children[1]->children[0]->VTcontent;
							convert >> i;
							news.addVecInt(i);
							tmp = tmp->children[2];//指向了第二个表达式的NB
						}
						catch (exception e) {
							//这里主要想解决的异常是i的值是int，如果定义了一个int数组但是赋值给的是float
							cout << "in line " << tmp->Line << " have exceptions:you may have errors with type convert." << endl;
						}
					}
			}
			else {
			//double数组
				
				if (tmp->children[1]->children[0]->tokenStr != "$") {
					int i;
					stringstream convert;
					try {
						convert << tmp->children[0]->children[0]->VTcontent;
						convert >> i;
						news.addVecInt(i);
						tmp = tmp->children[1];//指向了第二个表达式的NB
					}
					catch (exception e) {
						//这里主要想解决的异常是i的值是int，如果定义了一个int数组但是赋值给的是float
						cout << "in line " << tmp->Line << " have exceptions:you may have errors with type convert." << endl;
					}
				}
				while (tmp->children[2]->children[0]->tokenStr != "$") {
					//到第二个表达式之后形式稳定了
					if (news.douA.size() >= news.getSize()) {
						//数组越界了，不可再初始化
						cout << "array is out of bound" << endl;
						abort();
					}
					double i;
					stringstream convert;
					try {
						convert << tmp->children[1]->children[0]->VTcontent;
						convert >> i;
						news.addVecDou(i);
						tmp = tmp->children[2];//指向了第二个表达式的NB
					}
					catch (exception e) {
						//这里主要想解决的异常是i的值是int，如果定义了一个double数组但是赋值给的是int
						cout << "in line " << tmp->Line << " have exceptions:you may have errors with type convert." << endl;
					}
				}
			}
			}
		}
}

void getLeaves(treeNode* node) {
	if (node->childNum == 0) {//当遇到了叶子节点
		expre[el].VTcontent = node->VTcontent;   //VTcontent就是具体的值对于ID来说就是a,apple之类的，如果是数字就是2,3.4之类具体的值,有一个缺点是所有的都当成string去存储了
		el++;//tokenstr就是终结符对应的type
	}
	int i = 0;
	for (i; i < node->childNum; i++) {
			getLeaves(node->children[i]);
		}
	}
	

bool isOperator(treeNode node) {
	//判断读进来的数是不是操作符
	if (node.tokenStr == "PLUS" || node.tokenStr == "MINUS" || node.tokenStr == "MUL" || node.tokenStr == "DIV" || node.tokenStr == "MOD"
		|| node.tokenStr == "GRT" || node.tokenStr == "LES" || node.tokenStr == "GRT_EQU" || node.tokenStr == "LES_EQU" || node.tokenStr == "EQU" || node.tokenStr == "NOT_EQU") {
		return true;
	}
}

value* executeExpr(treeNode* node){
	//这个函数的作用是计算表达式的值并进行赋值，表达式的值里面有普通表达式的值和布尔值的表达式，对应E和OE
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
	//思路是用表达式求值的算法，设置两个栈，然后求值后返回一个value值，所以看到E先要得到表达式，表达式就是以E为根节点的叶子节点，我们将它存在一个数组里
	//有两个栈，一个是操作符栈，一个是操作数栈
	stack<value*> numbers;//对于变量要得到它的值，如果是数字的话就直接是一个数字
	stack<tokenType> operators;
	int i = 0;
	for(i; i <= el+1; i++) {
		treeNode x = expre[i];
		if (i == el+1) {
			//如果最后一个元素已经处理完了的情况
			if (operators.empty()) {
				//空的话返回true
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
				if ( x.VTcontent== ")")//括号运算结束，弹出有括号，运算括号里面的内容
				{
					operators.pop();
				}
			}

		}
		else if (!isOperator(x)) {
			//如果x不是一个操作符的话
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
				//如果是一个标识符的话,先要去查找符号表中有没有这个的定义，其中会产生相应的错误处理，然后正常的话要返回标识符对应的值
				symbol* sym=ta.searchTa(x.VTcontent, level);
				if (sym != NULL) {
					if (sym->getType() == sym->INTTYPE) {
						//是一个int值
						tmp->setIntValue(sym->getIntV());

					}
					else {
						//是一个double值
						tmp->setDouValue(sym->getDouV());
					}
					numbers.push(tmp);
				}
				else {
					//错误处理
				}
			}
			
		}
		else if (isOperator(x)) {//如果输入的是一个符号
			//如果栈是空的的话，直接push
			if (operators.empty() == true) {
				operators.push(str2Token(x.tokenStr));
			}
			else {
				//栈内不空则比较优先级
				if (getPerior(operators.top()) >= getPerior(str2Token(x.tokenStr))) {
					//如果优先级相等或者top>进去
					if (operators.top() == LEFT_BRA) {
						//如果top是左括号的话，虽然优先级肯定比左括号低，但还是直接进去
						operators.push(str2Token(x.tokenStr));
					}
					else {
						//普通情况，就要进行运算
						tmp1 = numbers.top();
						numbers.pop();
						tmp2 = numbers.top();
						numbers.pop();
						op = operators.top();
						operators.pop();
						va = executeOp(op,tmp1, tmp2);
						numbers.push(va);
						if (x.VTcontent == ")")//括号运算结束，弹出有括号，运算括号里面的内容
						{
							operators.pop();//吧左括号搞出来
						}
					}

				}
				else if (getPerior(operators.top()) < getPerior(str2Token(x.tokenStr))) {
					//进去大于top
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
	//用trycatch handle异常
	try {
		if (node->children[1]->children[0]->tokenStr == "ASSIGN") {
			if (node->children[0]->children[1]->children[0]->tokenStr == "$") {
				//对于普通变量而言
				if (tmpsym->getType() == tmpsym->INTTYPE) {
					tmpsym->setInt(executeExpr(node->children[1]->children[1])->getIntValue());//L节点
				}
				else {
					tmpsym->setDou(executeExpr(node->children[1]->children[1])->getDouValue());//L节点
				}
			}
			else {
				//对于数组
				int index = 0;
				/*V:= I D
				I:= ID
				D:= LEFT_INDEX E RIGHT_INDEX
				F:= E
				F:= V
				F:= N
				F:= $
				*/
				//获取要索引的值
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
				//如果是一个int数组
				if (tmpsym->getType() == tmpsym->INTTYPE) {
					tmpsym->intA[index] = executeExpr(node->children[1]->children[1])->getIntValue();//L节点

				}
				else {
					tmpsym->douA[index]=executeExpr(node->children[1]->children[1])->getDouValue();//L节点
				}
				
			}
		}
		//如果不是assign的话就是简单的声明，不做任何处理
	}
	catch (exception e) {
		cout << "in line " << node->Line << ": the variable hasn't been defined." << endl;
	}

	 

}

//根据栈计算表达式的值
value* executeOp(tokenType opera, value* op1, value*  op2){
	value *va = new value();
	int type;
	//确定最后计算结果的类型，有一个为double则最后结果为double
	if (op1->getType() == 1 || op2->getType() == 1) {
		//1表示double类型
		type =1;
	}
	else {
		type = 0;
	}
	va->setType(type);
	//对两个操作符进行运算
	switch (opera) {
	case PLUS:
		try {
			if (type == 0) {//int
				va->setIntValue(op1->getIntValue() + op2->getIntValue());
			}
			else {
				//如果都不是整数则全部转换为double类型
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
			//如果都不是整数则全部转换为double类型
			va->setDouValue(op1->getDouValue() - op2->getDouValue());
		}
		break;
	case MUL:
		if (type ==0) {//int
			va->setIntValue(op1->getIntValue()*op2->getIntValue());
		}
		else {
			//如果都不是整数则全部转换为double类型
			va->setDouValue(op1->getDouValue()*op2->getDouValue());
		}
		break;
	case DIV:
		//检查除零错误
		if (op2->getDouValue() == 0.0||op2->getIntValue()==0) {
			cout << "除数不能为0" << endl;
		}
		if (type == 0) {
			va->setIntValue(op1->getIntValue() / op2->getIntValue());
		}
		else {
			//如果都不是整数则全部转换为double类型
			va->setDouValue(op1->getDouValue() / op2->getDouValue());
		}
		break;
	case MOD:
		if (op2->getDouValue() == 0.0 || op2->getIntValue() == 0) {
			cout << "除数不能为0" << endl;
		}
		if (op1->getType() == 1 || op2->getType() == 1) {//都是double
			va->setDouValue(0.0);
		}
		else {
			va->setIntValue(op1->getIntValue() - op1->getIntValue() / op2->getIntValue()*op2->getIntValue());
		}
	case GRT:
		if (op1->getDouValue() > op2->getDouValue()) {//以double值为准，进行比较,有个问题，你的double，，要是一开始就是int呢，，，
			va->setType(va->TRUETYPE);//error 这里有一点小问题，见上一行
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
	CF是一个新的程序块*/
	value* con1 = executeExpr(node->children[1]->children[1]->children[0]);
	value*con2= executeExpr(node->children[1]->children[1]->children[2]);
	value* res = executeOp(str2Token(node->children[1]->children[1]->children[1]->children[0]->VTcontent), con1, con2);
	//true和非0值使条件为真,运行CF
	if (res->getType() == res->TRUETYPE) {
		executeStateB(node->children[1]->children[3]->children[2]->children[1], level);//CF
	}
	else {
		if (node->children[1]->children[4]->children[0]->tokenStr!="$") {
			//如果有else
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
	//这个函数的作用是考虑覆盖情况
	level++;//层数递增
	for (int i = 0; i < node->childNum;i++) {
			executeStmt(node);
		
	}
	
}


//这个函数的作用是在符号表中查找相应的变量，如果找到了，返回相应的symbol节点，对于一般变量而言，
//没找到就报错，对于数组变量，就去sybolarr对象里面查找，没找到就报错（简单粗暴）
//所以这个已经不在声明树的范畴里面了，而是在后面BB的范畴里，针对AF节点
/* 
    AF: = V AC
	AC : = ASSIGN L SEMI
	AC : = SEMI
	*/
symbol* executeValue(treeNode* node, int level) {//注意在树上是不设置value节点的，
	symbol* tmp;
	//AF->V AC
	//AC:= ASSIGN L SEMI
	//AC: = SEMI
	//不管最后是否赋值，value的工作只是针对变量查找并返回
		if ((node->children[0]->children[1]->children[0]->tokenStr == "$"))
		{
			//D为空所以是给普通变量赋值并且是赋值操作，不是简单的再次声明一下
			tmp = ta.searchTa(node->children[0]->children[0]->children[0]->VTcontent, level);
		}
		else {
			//是数组变量
			tmp = ta.searchArr(node->children[0]->children[0]->children[0]->VTcontent, level);
		}
	
	return tmp;
}

