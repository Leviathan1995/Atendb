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
	static Record_Manager & Instance();																	//实例化
	bool Record_ManagerCreateTable(string &tablename, const vector<Attributes>& Table_Column);			//Create建表
	bool Record_ManagerInsert_Into(Table &table, vector<Tuple> Tuple_Lists);							//insert into 插入记录
	bool Record_ManagerHasExisted(Table &table, string &content, int num, int BlockNum);				//元组记录是否存在
	int Record_ManagerFindDirtyTuple(string &strout, int size);											//返回脏读的偏移量
};
//实例化
inline Record_Manager & Record_Manager::Instance()
{
	static Record_Manager Record_Instance;
	return Record_Instance;
}
#endif