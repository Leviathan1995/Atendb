#pragma once
#ifndef _RECORD_MANAGER_H
#define _RECORD_MANAGER_H
#include "Catalog.h"
#include "Buffer.h"
#include "Record.h"
#include <string>
#include <vector>
using namespace std;
/*
	实现数据文件的创建与删除
	记录的插入
	记录的删除
	记录的查找
	数据文件由一个或多个块组成，块大小与缓冲区大小相同。
	一个块包含一条或多条记录。
	只存储定长的记录，不支持跨块存储
*/
class Record_Manager
{
public:
	static Record_Manager & Instance();//实例化
	bool CreateTable(string &tablename, const vector<Column_Type>& Table_Column);//Create建表
	bool Insert_Into(Table_Type &table,Record R);//Insert into 插入记录
	void PrintHead();//打印记录的头部
	void Print();//打印记录
	void PrintEnd();//打印记录的尾部
	bool HasExisted(Table_Type &table, string content, int num, int BlockNum);
	void WriteRecord2Block(Byte *Positon, Record R);////将一条记录按顺序拷贝到p的位置
	Record Copy_block_to_record(Byte* Position, Table_Type table);
};
//实例化
inline Record_Manager & Record_Manager::Instance()
{
	static Record_Manager Record_Instance;
	return Record_Instance;
}
#endif