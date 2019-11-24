#include "token.h"

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
			tmp.Line = line;

			switch (state)
			{
			case START: //开始状态
				if (isdigit(c))
				{
					state = INNUM;
					currentToken = INT_NUM;
				}
				else if (isLetter(c))
				{
					state = INID;
					currentToken = ID;
				}
				else if (c == '>')
				{
					currentToken = GRT;
					state = INGRT;
				}
				else if (c == '<')
				{
					currentToken = LES;
					state = INLES;
				}

				else if (c == '=') {
					currentToken = ASSIGN;
					
					state = INASSIGN;
				}
				else if (c == '+') {
					state = INPLUS;
					currentToken = PLUS;
					
				}
				else if (c == '-') {
					state = INMIN;
					currentToken = MINUS;
				}
				else if (c == '*') {
					state = INMUL;
					currentToken = MUL;
				}
				else if (c == '/') {
					state = INDIV;
					currentToken = DIV;
				}
				else if (c == '%') {
					state = INMOD;
					currentToken = MOD;
				}
				else if ((c == ' ') || (c == '\t') || (c == '\n'))
				{
					save = false;
				}

				else 
				{
					if (c == ';') {
						currentToken = SEMI;
						state = DONE;
						
					}
					else if (c == ',') {
						currentToken = COMMA;
					
					}
					else if (c == '(') {
						currentToken = LEFT_BRA;
						state = DONE;
						
					}
					else if (c == ')') {
						currentToken = RIGHT_BRA;
						state = DONE;
						
					}
					else if (c == '[') {
						currentToken = LEFT_INDEX;
						state = DONE;
						
					}
					else if (c == ']') {
						currentToken = RIGHT_INDEX;
						state = DONE;
						
					}
					else if (c== '{') {
						currentToken = LEFT_BOUND;
						state = DONE;
						
					}
					else if (c == '}') {
						currentToken = RIGHT_BOUND;
						state = DONE;
						
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
						
						
							state = INREAL0;
						
					}
					else {
						ssIndex--;
						save = false;
						state = DONE;
						currentToken = INT_NUM;
						
					}
				}
				
			
				break;
			case INREAL0:
				if (isdigit(c)) {
					
					
						state = INREAL;					
					
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
				currentToken = MOD;
				break;
			case INDIV:
				save = false;
				if (c != '/'&&c != '*') {
					ssIndex--;
					state = DONE;
					currentToken = DIV;

				}
				else if(c=='/') {
					
					state = INLINECOM;
					currentToken = LINE_NOTE;
				}
				else if (c == '*') {
					
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
					state = DONE;
					currentToken = EQU;

				}

				break;
			case INLES:
				if (c == '=') {
					state = DONE;
					currentToken = LES_EQU;
				}
				else if (c == '>') {
					state = DONE;
					currentToken = NOT_EQU;
				}
				else {
					state = DONE;
					currentToken = LES;
				}
				break;
			case INGRT:
				if (c == '=') {
					state=DONE;
					currentToken = GRT_EQU;

				}
				else {
					state = DONE;
					currentToken = GRT;
				}
				break;
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
			//case INSPECIAL:
			//	state = DONE;
			//	save = false;
			//	ssIndex--;
			//	break;
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
			if (currentToken != MULTI_NOTE && currentToken != LINE_NOTE&&tokenString!=" ") {
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

