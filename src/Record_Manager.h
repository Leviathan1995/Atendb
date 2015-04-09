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
	在每一个".table"文件中，存放一张表的信息，支持定长的记录
	一个".table"文件可以分割成若干个块，每一个块4KB，从0开始编号
	不支持跨块存储，每个块末尾不够存放一条记录的空间被放弃，从下一个新块开始存放
*/
class Record_Manager
{
public:
	static Record_Manager & Instance();																	//实例化
	bool Record_ManagerCreateTable(string &tablename, const vector<Attributes>& Table_Column);			//Create建表
	bool Record_ManagerInsert_Into(Table &table, vector<Tuple> Tuple_Lists);							//insert into 插入记录
	bool Record_ManagerHasExisted(Table &table, string &content, int num, int BlockNum);				//元组记录是否存在
	int Record_ManagerFindDirtyTuple(string &strout, int size);											//返回脏读的偏移量
	vector<Tuple> Record_ManagerSelectTuple(Table & table, int blocknum);								//选择元组
	vector<string> Record_ManagerString2Tuple(vector<Attributes> attributes, string tuple_str);			//将记录转换为vector<string> 形式
	vector<Tuple> Record_Manager2Tuple(vector<string> tuple_str,Table &table);							//再将vector<string>形式的记录转换为tuple
};
//实例化
inline Record_Manager & Record_Manager::Instance()
{
	static Record_Manager Record_Instance;
	return Record_Instance;
}
#endif