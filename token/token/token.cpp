#include "token.h"
#include "Utilize.h"
#define MAXTOKENLEN 1000
char tokenString[MAXTOKENLEN + 1];

list<errorNode> err;//这是一个存储错误信息的list
errorNode errt;
bool hasError;
list<tokenInfo> resultTok; //讲分析结果存储在一个list中，便于语法分析使用
tokenInfo tmp ;
ofstream token_ana("token.txt");



//判断是不是字母
bool isLetter(char c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
		return true;
	}
	else
	{
		return false;
	}
}


//判断是不是数字
bool isDigit(char c) 
{
	if (c >= '0' && c <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

static struct //保留字结构体，用于输出
{
	const char *str;
	tokenType tok;
} reservedWords[5] = {
	{"if", IF},
	{"else", ELSE},

	{"while",WHILE},
	{"int",INT},
	{"float",REAL}
};

static tokenType reservedLookup(char *s)
{
	for (int i = 0; i < 5; i++)
		if (!strcmp(s, reservedWords[i].str))
			return reservedWords[i].tok;
	return ID;
}

void printToken(tokenType token, const char tokenString[]) {
	switch (token) {
	case IF:
	case ELSE:
	case INT:
	case REAL:
	case WHILE:
		token_ana  << "Reserved Word: " << tokenString << endl;
		break;
	
	case LEFT_BRA:
	case RIGHT_BRA:
	case LEFT_INDEX:
	case RIGHT_INDEX:
	case LEFT_BOUND:
	case RIGHT_BOUND:
	case SIN_QUE:
	case DOU_QUE:
	case COMMA:
	case SEMI:
	
		token_ana << "Bound Symbol: " << tokenString << endl;
		break;

	case PLUS:
	case MINUS:
	case MUL:
	case DIV:
	case MOD:
	case ASSIGN:
	case LES:
	case LES_EQU:
	case GRT:
	case GRT_EQU:
	case NOT_EQU:
	case EQU:
		token_ana << "Operator: " << tokenString << endl; break;
	
	case INT_NUM:
		token_ana << "integer:" << tokenString << endl; break;
	case REAL_NUM:
		token_ana << "real number:" << tokenString << endl; break;
		break;

	case ID:
		token_ana << "Identifier: " << tokenString<< endl;
		break;
	case LINE_NOTE:
		token_ana << "line comment:" << tokenString << endl;
		break;
	case MULTI_NOTE:
		token_ana << "multipul lines comment:" << tokenString << endl;
		break;
	case ERROR:
		hasError = true;
		token_ana << "ERROR token:" << tokenString << endl;
		errt.content = tokenString;
		errt.line = line;
		err.push_back(errt);
		
		
		
		break;
	default: 
		token_ana << "Unknown Token:" << tokenString << endl;
	}
}

static stateType stateSave = START; //保存DFA状态
stateType state = stateSave;        //DFA状态
void getToken(string ss) {
	int tokenStringIndex = 0;           // 匹配字符串的下标
	tokenType currentToken=INIT;              //记录下当前的token
	
	bool save = true;
	int ssIndex = 0; // 输入字符串ss的下标
	
	while (ssIndex < ss.length())
	{
		while (ssIndex <ss.length() && state != DONE)
		{
			
			char c = ss[ssIndex];
			
			
			save = true;
			tmp.line = line;

			switch (state)
			{
			case START: //开始状态
				if (isdigit(c))
				{
					state = INNUM;
				}
				else if (isLetter(c))
				{
					state = INID;
				}
				else if (c == '>')
				{
					//currentToken = MORE;
					state = INGRT;
				}
				else if (c == '<')
				{
					//currentToken = LESS;
					state = INLES;
				}

				else if (c == '=') {
					currentToken = ASSIGN;
					
					state = INASSIGN;
				}
				else if (c == '+') {
					state = INPLUS;
					
				}
				else if (c == '-') {
					state = INMIN;
				}
				else if (c == '*') {
					state = INMUL;
				}
				else if (c == '/') {
					state = INDIV;
				}
				else if (c == '%') {
					state = INMOD;
				}
				else if ((c == ' ') || (c == '\t') || (c == '\n'))
				{
					save = false;
				}

				else 
				{
					if (c == ';') {
						currentToken = SEMI;
						
					}
					else if (c == ',') {
						currentToken = COMMA;
					
					}
					else if (c == '(') {
						currentToken = LEFT_BRA;
						
					}
					else if (c == ')') {
						currentToken = RIGHT_BRA;
						
					}
					else if (c == '[') {
						currentToken = LEFT_INDEX;
						
					}
					else if (c == ']') {
						currentToken = RIGHT_INDEX;
						
					}
					else if (c== '{') {
						currentToken = LEFT_BOUND;
						
					}
					else if (c == '}') {
						currentToken = RIGHT_BOUND;
						
					}
					
					else {
						currentToken = ERROR;
					
						
					}

					state = DONE;
				}
			
				break;
			
			case INNUM: //数字状态
				//如果一行已经读完了，发现这行的最后是一个int或者float，直接保存，不可以跨行
				if (!isdigit(c))
				{
					//如果该行最后一位不是数字,例如 23.
					if (c == '.') {
						if (ssIndex + 1 == ss.length()) {
							state = DONE;
							currentToken = ERROR;
						}
						else {
							state = INREAL0;
						}
					}
					else {
						ssIndex--;
						save = false;
						state = DONE;
						currentToken = INT_NUM;
						
					}
				}
				
				if (ssIndex + 1 == ss.length()) {
					//如果该行最后一位是数字
					state = DONE;
					currentToken = INT_NUM;
				}
				break;
			case INREAL0:
				if (isdigit(c)) {
					if (ssIndex + 1 == ss.length()) {
						//如果该行最后一位是数字
						state = DONE;
					}
					else {
						state = INREAL;					
					}
					currentToken = REAL_NUM;
				}
				else {
					save = false;
					ssIndex--;
					currentToken = ERROR;
					state = DONE;
				}
				break;
			case INREAL:
				if ((ssIndex + 1 == ss.length())&&isdigit(c)){
					//如果已经到最后了,且当前也是个实数
					state = DONE;
					currentToken = REAL_NUM;
				}
				if (!isdigit(c)) {
					state = DONE;
					save = false;
					ssIndex--;
					currentToken = REAL_NUM;
				}
				break;
			case INID: //标识符状态
				if (isLetter(c)||isdigit(c)||(c=='_'))
				{
					state = INID;
					currentToken = ID;
				}
				else {
					ssIndex--;
					save = false;
					state = DONE;
					currentToken = ID;
				}
				break;
			
			case INPLUS:
				state = DONE;
				save = false;
				ssIndex--;
				currentToken = PLUS;
				break;
			case INMIN:
				state = DONE;
				save = false;
				ssIndex--;
				currentToken = MINUS;
				break;
			case INMUL:
				state = DONE;
				save = false;
				ssIndex--;
				currentToken = MUL;
				break;
			case INMOD:
				state = DONE;
				save = false;
				ssIndex--;
				currentToken = DIV;
				break;
			case INDIV:
				if (c != '/'&&c != '*') {
					save = false;
					state = DONE;
					currentToken = DIV;

				}
				else if(c=='/') {
					save = false;
					state = INLINECOM;
					currentToken = LINE_NOTE;
				}
				else if (c == '*') {
					save = false;
					state = INMULCOM1;
					currentToken = MULTI_NOTE;
				}
				break;
			case INASSIGN:
				if (c != '=') {
					ssIndex--;
					state = DONE;
					save = false;
					currentToken = ASSIGN;
				}
				else {
					state = INSPECIAL;
					currentToken = EQU;

				}

				break;
			case INLES:
				if (c == '=') {
					state = INSPECIAL;
					currentToken = LES_EQU;
				}
				else if (c == '>') {
					state = INSPECIAL;
					currentToken = NOT_EQU;
				}
				else {
					state = DONE;
					currentToken = LES;
				}
			case INGRT:
				if (c == '=') {
					state=INSPECIAL;
					currentToken = GRT_EQU;

				}
				else {
					state = DONE;
					currentToken = GRT;
				}
			case INLINECOM:
				save = false;
				if (ssIndex == ss.length() - 1) {
					state = DONE;
					currentToken = LINE_NOTE;
				}
				break;
			case INMULCOM1:
				save = false;
				currentToken = MULTI_NOTE;
				
				if ((c == '*')&&(ssIndex!=ss.length()-1)) {//并且*不是在最后一个，预防隔行的*/情况
					state = INMULCOM2;
					
				}
				
				break;
			case INMULCOM2:
				save = false;
				currentToken = MULTI_NOTE;
				if (c == '/') {
					state = DONE;
					
				}
				else {
					state = INMULCOM1;
				}
				break;
			case INSPECIAL:
				state = DONE;
				save = false;
				ssIndex--;
				break;
			case DONE: //接受状态
				break;
			default:
				token_ana << "happen a never state" << endl;
				break;
			}
			if ((save) && (tokenStringIndex <= MAXTOKENLEN))
			{
				tokenString[tokenStringIndex++] = (char)c;
			}
			if (state == DONE||state==INMULCOM1||state==INMULCOM2)
			{
				
				tokenString[tokenStringIndex] = '\0';
				if (currentToken == ID) {
					currentToken = reservedLookup(tokenString);
					if (tokenString[tokenStringIndex - 1] == '_') {
						currentToken = ERROR;
					}
				}
			}
			ssIndex++;
			
		}
		
		if (state == DONE)
		{
			if (currentToken != MULTI_NOTE && currentToken != LINE_NOTE) {
				tmp.content = tokenString;
				tmp.type = currentToken;
				resultTok.push_back(tmp);
			}
			printToken(currentToken, tokenString);
			state = START;
		}
		
		tokenStringIndex = 0;
	}

	//对多行注释的处理，作用只是提示这是多行注释，由于并不会存储注释所以并不会显示在分析结果中
	if (state == INMULCOM1 || state == INMULCOM2) {
	printToken(currentToken, tokenString);

		}
	
	

}

