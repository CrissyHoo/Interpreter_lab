#pragma once
//���������һЩͨ�õĺ���
#include <vector>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <map>
#include <stack>
using namespace std;

list<string> split(string &str,const string &pattern);

struct errorNode {          //����ڵ㣬������֯�ɴ�������
	string content;         //����
	int line;               //��������
};
