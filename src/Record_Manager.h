#ifndef _RECORD_MANAGER_H
#define _RECORD_MANAGER_H
#include "Catalog.h"
#include "Buffer.h"
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
	bool CreateTable(string tablename, vector<Column_Type>& Table_Column);//建表
};
#endif