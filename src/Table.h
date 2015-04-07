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
};
#endif