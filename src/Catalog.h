#pragma once
#ifndef _CATALOG_H
#define _CATALOG_H
#include "Glob_Var.h"
#include "Error.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

/*
	模式信息，即数据表的信息
*/
/*
	一般来说 size_t 是一个无符号整数类型，更确切的信息是 size_t 是 sizeof() 返回的类型。
	至于定义成 unsigned int 还是 unsigned long 还是别的什么都无关紧要了。
	使用 size_t 可以提高在代码级别的可移植性
*/
//字段
struct Column_Type
{
	int ID;
	char Flag;//标志位
	string Column_TypeName;//字段名
	char coltype;//字段属性,0 表示 int，1 表示 char(n)，2 表示 float
	bool IsPrimary;//是不是主键
	bool IsNotNull;//是否可以为空
	bool IsUnique;//是否为Unique
	int RequestSize;//用户请求的长度
	int StoredLength;//实际存储长度
	Column_Type * Next;//下一个字段
	short NextKey;//该表下一条键的信息，若没有则为-1
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
	unsigned long IndexFlags; // 对每一位，0 表示该键无索引，1 表示该键有索引
	unsigned short FirstKey; // key 目录文件中，该表第一条键信息的编号
	short FirstIndex; // index 目录文件中，该表第一条索引信息的编号
	union Table_ptr
	{
		Column_Type * Key;//指向主键的指针
	};
	Column_Type GetColumn(string name);//通过字段的名字获得字段
	void InsertColumn(Column_Type column);//数据表插入属性

};
//数据库中的数据表实例类
/*
	Catalog对于文件的存取不通过Buffer ，而是独立对Catalog file进行维护
	在构造函数中读取信息然后放入缓存，析构函数将信息写回文件。
*/
class Catalog
{
public :
	//实例化
	static Catalog & Instance()
	{
		static Catalog CatalogManagent;
		return CatalogManagent;
	}
	// 标志位相关
	static const char CATALOG_SPACE_USED = 0x80;
	static const char CATALOG_HAS_PRIMARY_KEY = 0x40;
	static const char CATALOG_HAS_INDEX = 0x20;
	static const char CATALOG_IS_PRIMARY_KEY = 0x10;
	static const char CATALOG_IS_UNIQUE = 0x08;
	static const char CATALOG_IS_NOT_NULL = 0x04;
	static const char CATALOG_IS_INDEX = 0x02;
	static const char CATALOG_HAS_NEXT = 0x01;
	//功能需求
	void CatalogCreateTable(string & Tablename, vector<Column_Type> & Attributes);//建立数据表
	vector<Table_Type> TableCatalog;//数据表的存放
	vector<Column_Type> ColumnCatalog;//属性的存放
	static map<string, Table_Type> Mem_Table;
	static Table_Type & Get_Table(string tablename);//得到 数据表
	static Column_Type & Get_Column(string tablename,string columnname);//得到 数据表中的属性
	void CheckTable(string &Tablename ,vector<Column_Type> & Attributes);//数据表的检查
	//析构函数
	~Catalog();
	void SaveTable2File();//将数据写入文件，由析构函数调用
};
#endif