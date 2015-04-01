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
	Table();														//默认构造函数
	Table(string tablename);										
	char Flag;														//数据表标志位
	char Table_Attributesnum;										//数据表中属性的数量
	string Table_Name;												//数据表的名字
	vector<Attributes> Table_Attributes;							//数据表中属性集合
	Attributes & Get_Attributes(string & attributes_name);			//返回查找的数据表属性
	int Get_RecordSize();											//返回这个Table的一条记录的字节大小
	const vector<Attributes> Get_Attributes_List();				    //返回属性的列表
	short Table_FirstAttributes;									//第一条记录的索引号
	char Table_PrimaryAttributes;									//数据表中的主键
	unsigned long IndexFlags;										// 对每一位，0 表示该键无索引，1 表示该键有索引
	short FirstIndex;												// index 目录文件中，该表第一条索引信息的编号

};
#endif