#pragma once
//这里包含了一些通用的函数
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

struct errorNode {          //错误节点，用来组织成错误链表
	string content;         //内容
	int line;               //所在行数
};
