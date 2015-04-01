#pragma once
#ifndef _ATTRIBUTES_H
#define _ATTRIBUTES_H
#include <string>
#include "Glob_Var.h"
using namespace std;
/*
	数据表中的属性
*/
class Attributes
{
public:
	Attributes();						//默认构造函数
	Attributes(string attributes_name, Attributes_Type attributes_type, int attributes_charnum, bool primary, bool unique);
	Attributes(string attributes_name, Attributes_Type attributes_type, bool primary, bool unique);
	~Attributes();
	char Flag;							//元组的标志位
	string Attributes_name;				//元组的名字
	Attributes_Type Attributes_type;	//元组的属性
	int Attributes_charnum;				//如果是char 类型，字符的数量
	bool Attributes_primary;			//是否为Primary
	bool Attributes_unique;				//是否为Unique
	bool Attributes_null;				//是否为Null
	int RequestSize;					//用户请求的长度
	int StoredLength;					//实际存储长度
	short NextAttributes;				//下一个属性的索引号
};
#endif