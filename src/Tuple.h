#pragma once
#ifndef _TUPLE_H
#define _TUPLE_H
#include "Table.h"
#include <string>
#include <vector>
using namespace std;
/*
		数据表中的记录
*/
class Tuple
{
public:
	Tuple(){}
	Tuple(Table & table, string &content, int begin);
	Tuple(vector<string> & tuplecontent);
	vector<string> Tuple_content;							//元组的内容
};
#endif