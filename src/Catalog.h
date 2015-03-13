#ifndef _CATALOG_H
#define _CATALOG_H
#include "Glob_Var.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

/*
	模式信息，即数据表的信息
*/
//字段
class Column_Type
{
public:
	int ID;
	string Column_TypeName;//字段名
	ColType coltype;//字段属性
	bool IsPrimary;//是不是主键
	bool IsNull;//是否可以为空
	bool IsUnique;//是否为Unique
	int RequestSize;//用户请求的长度
	int StoredLength;//实际存储长度
	union Column_ptr
	{
		Column_Type * Next;//下一个字段
	};
};
//数据表
class Table_Type
{
public:
	Table_Type();//创建数据表
	vector<Column_Type> Table_Column;//数据表中的属性
	string Table_Name;//表名
	int ColumnNum;//数据表具有的字段数目
	int RecordSize;//记录的长度
	int PrimaryNum;//多属性主键的个数
	union Table_ptr
	{
		Column_Type * Key;//指向主键的指针
	};
	Column_Type GetColumn(string name);//通过字段的名字获得字段
	void InsertColumn(Column_Type column);//数据表插入属性

};
class Catalog
{
public :
	map<string, Table_Type> Mem_Table;
	Table_Type & Get_Table(string name);
};
#endif