#pragma once
#ifndef _SELECT_H
#define _SELECT_H
#include "Catalog.h"
#include "Intepretor.h"
#include <vector>
#include <string>
//Select 中的where结构体
struct SelectRecord			//Select后的记录
{
	string AttributesName;		//属性名
	vector<string> selectrecords;//属性值
};
struct WhereList
{
	string Attribute;//选择的属性
	string Where_Operator;//运算符号
	union
	{
		const char *StrValue;
		int IntValue;
	};
};
//Select 命令的接口
class Selection
{
public:

	static queue<string> SelLists;																//即选择的属性
	static queue<string> TableLists;															//select 中from的数据表列表
	static queue<WhereList> WhereLists;															//slect 中where部分
	static vector<SelectRecord> SelectRecords;													//经过Select后的元组
	static vector<SelectRecord> & Select_Parse(queue<string> attributes,queue<string> tableLists,queue<WhereList> whereLists);//解析用户输入的Select 语句
};
#endif