#ifndef _CATALOG_H
#define _CATALOG_H
#include "Glob_Var.h"
#include "Intepretor.h"
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
	char Flag;//标志位
	string Column_TypeName;//字段名
	char coltype;//字段属性,0 表示 int，1 表示 char(n)，2 表示 float
	bool IsPrimary;//是不是主键
	bool IsNotNull;//是否可以为空
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
	char Flag;//标志位
	char PrimaryKey;//如果>32表示无主键
	char NumberKeys;//<=32
	vector<Column_Type> Table_Column;//数据表中的元组存放的地方
	string Table_Name;//表名
	int ColumnNum;//数据表具有的属性数目
	int RecordSize;//记录的长度
	int PrimaryNum;//多属性主键的个数
	unsigned long indexFlags; // 对每一位，0 表示该键无索引，1 表示该键有索引
	unsigned short firstKey; // key 目录文件中，该表第一条键信息的编号
	short firstIndex; // index 目录文件中，该表第一条索引信息的编号
	union Table_ptr
	{
		Column_Type * Key;//指向主键的指针
	};
	Column_Type GetColumn(string name);//通过字段的名字获得字段
	void InsertColumn(Column_Type column);//数据表插入属性

};
//数据库中的数据表实例类
class Catalog
{
public :
	// 标志位相关
	static const char CATALOG_SPACE_USED = 0x80;
	static const char CATALOG_HAS_PRIMARY_KEY = 0x40;
	static const char CATALOG_HAS_INDEX = 0x20;
	static const char CATALOG_IS_PRIMARY_KEY = 0x10;
	static const char CATALOG_IS_UNIQUE = 0x08;
	static const char CATALOG_IS_NOT_NULL = 0x04;
	static const char CATALOG_IS_INDEX = 0x02;
	static const char CATALOG_HAS_NEXT = 0x01;
	void CatalogCreateTable(string & Tablename,const vector<Column_Type> & Attributes);
	vector<Table_Type> TableCatalog;
	static map<string, Table_Type> Mem_Table;
	static Table_Type & Get_Table(string tablename);
	static Column_Type & Get_Column(string columnname);
	void SaveTable2File();
};
#endif