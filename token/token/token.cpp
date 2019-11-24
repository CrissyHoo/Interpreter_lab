#include "token.h"

#define MAXTOKENLEN 1000
char tokenString[MAXTOKENLEN + 1];

list<errorNode> err;//����һ���洢������Ϣ��list
errorNode errt;
bool hasError;
list<tokenInfo> resultTok; //����������洢��һ��list�У������﷨����ʹ��

tokenInfo tmp ;
ofstream token_ana("token.txt");



//�ж��ǲ�����ĸ
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


//�ж��ǲ�������
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

static struct //�����ֽṹ�壬�������
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


static stateType stateSave = START; //����DFA״̬
stateType state = stateSave;        //DFA״̬
void getToken(string ss) {
	int tokenStringIndex = 0;           // ƥ���ַ������±�
	tokenType currentToken=INIT;              //��¼�µ�ǰ��token
	
	bool save = true;
	int ssIndex = 0; // �����ַ���ss���±�
	
	while (ssIndex < ss.length())
	{
		while (ssIndex <ss.length() && state != DONE)
		{
			
			char c = ss[ssIndex];
			
			
			save = true;
			tmp.Line = line;

			switch (state)
			{
			case START: //��ʼ״̬
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
			
			case INNUM: //����״̬
				//���һ���Ѿ������ˣ��������е������һ��int����float��ֱ�ӱ��棬�����Կ���
				if (!isdigit(c))
				{
					//����������һλ��������,���� 23.
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
			case INID: //��ʶ��״̬
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
				
				if ((c == '*')&&(ssIndex!=ss.length()-1)) {//����*���������һ����Ԥ�����е�*/���
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
			case DONE: //����״̬
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

	//�Զ���ע�͵Ĵ�������ֻ����ʾ���Ƕ���ע�ͣ����ڲ�����洢ע�����Բ�������ʾ�ڷ��������
	if (state == INMULCOM1 || state == INMULCOM2) {
	printToken(currentToken, tokenString);

		}
	
	

}

