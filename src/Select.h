#pragma once
#ifndef _SELECT_H
#define _SELECT_H
#include "Catalog.h"
#include "Intepretor.h"
#include <vector>
#include <string>
//Select 中的where结构体
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

	static vector<string> SelLists;//即选择的属性
	static vector<string> TableLists;//select 中from的数据表列表
	static vector<WhereList> WhereLists;//slect 中where部分
	static void SelectionInput(string *Sel, string *table, WhereList *& wherelist, int wherenum);//获取用户输入的属性和数据表和where部分
	void Selection_Parse();//解析用户输入的Select命令
	void Print_SelectHead();//打印Select的属性头
	vector<Record> Mem_Record;
	vector<Column_Type> Mem_SelectColumn;//经过选择后的元组
};
#endif