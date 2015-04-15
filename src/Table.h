#pragma once
#ifndef _TABLE_H
#define _TABLE_H
#include <string>
#include "Attributes.h"
#include <vector>
using namespace std;
class Table
{
public:
	Table(){}
	string Table_Name;								//数据表的名字
	vector<Attributes> Table_AttributesList;		//属性的集合
	size_t Table_Length();							//数据表的长度
	Attributes GetAttributes(string &attributesname);//得到属性
	size_t GetAttributesBegin(string &attributesname);//得到这个属性在模式信息表的中起点偏移量
	size_t GetAttributesEnd(string &attributesname);//得到这个属性在模式信息表的中终点偏移量
};
#endif