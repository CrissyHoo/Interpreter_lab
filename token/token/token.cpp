#include "token.h"
#include "Utilize.h"
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
			tmp.line = line;

			switch (state)
			{
			case START: //��ʼ״̬
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
			
			case INNUM: //����״̬
				//���һ���Ѿ������ˣ��������е������һ��int����float��ֱ�ӱ��棬�����Կ���
				if (!isdigit(c))
				{
					//����������һλ��������,���� 23.
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
					//����������һλ������
					state = DONE;
					currentToken = INT_NUM;
				}
				break;
			case INREAL0:
				if (isdigit(c)) {
					if (ssIndex + 1 == ss.length()) {
						//����������һλ������
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
					//����Ѿ��������,�ҵ�ǰҲ�Ǹ�ʵ��
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
			case INSPECIAL:
				state = DONE;
				save = false;
				ssIndex--;
				break;
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

	//�Զ���ע�͵Ĵ�������ֻ����ʾ���Ƕ���ע�ͣ����ڲ�����洢ע�����Բ�������ʾ�ڷ��������
	if (state == INMULCOM1 || state == INMULCOM2) {
	printToken(currentToken, tokenString);

		}
	
	

}

