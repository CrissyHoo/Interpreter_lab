#include "Utilize.h"
using namespace std;

list<string> split(string &str,const string &pattern) {
	//���ķ��Ҳ��Ĳ���ʽ�ָ��һ��һ���ķ���,ͨ��find�������ҵ�λ�ý��зָ�
	list<string> res;
	if (str == "")
		return res;
	//���ַ���ĩβҲ����ָ����������ȡ���һ��
	string strs = str + pattern;
	size_t pos = strs.find(pattern);
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		res.push_back(temp);
		//ȥ���ѷָ���ַ���,��ʣ�µ��ַ����н��зָ�
		strs = strs.substr(pos + pattern.length(), strs.size());
		pos = strs.find(pattern);
	}

	return res;
}