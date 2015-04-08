#pragma once
#ifndef _CATALOG_H
#define _CATALOG_H
#include "Glob_Var.h"
#include "Error.h"
#include "Record.h"
#include "Intepretor.h"
#include "Attributes.h"
#include "Table.h"
#include "Tuple.h"
#include <string>
#include <vector>
#include <queue>
#include <map>
using namespace std;

/*
	模式信息，即数据表的信息
	一般来说 size_t 是一个无符号整数类型，更确切的信息是 size_t 是 sizeof() 返回的类型。
	至于定义成 unsigned int 还是 unsigned long 还是别的什么都无关紧要了。
	使用 size_t 可以提高在代码级别的可移植性

	Catalog对于文件的存取不通过Buffer ，而是独立对Catalog file进行维护
	在构造函数中读取信息然后放入缓存，析构函数将信息写回文件。
*/
struct CatalogTable
{
	char CatalogTable_Flag;							 //标志位
	string CatalogTable_Name;						 //数据表名字
	char CatalogTable_AttribtuesNum;				 //属性的数目
	char CatalogTable_PrimaryAttributes;			 //数据中的主键
	unsigned long CatalogTable_IndexFlag;			 // 对每一位，0 表示该键无索引，1 表示该键有索引
	unsigned short CatalogTable_FirstAttributesIndex;// key 目录文件中，该表第一条属性信息的编号
	short CatablogTable_FirstIndex;					 // index 目录文件中，该表第一条索引信息的编号
};
struct CatalogAttributes 
{
	char CatalogAttributes_Flag;			//标志位
	string CatalogAttributes_Name;			//属性的名字
	char CatalogAttributes_Type;			//键类型，0 表示 int，1 表示 char(n)，2 表示 float
	char CatalogAttributes_Length;			//键长度
	short CatalogAttributes_NextAttributes;	//该表下一条键信息的编号，若无则置 -1
	bool CatalogAttributes_Primary;			//是否为主键
	bool CatalogAttributes_Null;			//是否可以为空
	bool CatalogAttributes_Unique;			//是否为Unique
};
struct CatalogIndex
{
	char CatalogIndex_Flag;					//标志位
	string CatalogIndex_Name;				//索引的名字
	unsigned short CatalogIndex_InTable;	//该索引所属表在Table目录文件中的编号
	char CatalogIndex_Key;					//该索引针对所属的表的键
	short CatalogIndex_Next;				//该表下一条索引信息的编号，没有为-1
};
//数据库中的数据表实例类
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
	static const unsigned char CATALOG_SPACE_USED = 0x80;
	static const unsigned char CATALOG_HAS_PRIMARY_KEY = 0x40;
	static const unsigned char CATALOG_HAS_INDEX = 0x20;
	static const unsigned char CATALOG_IS_PRIMARY_KEY = 0x10;
	static const unsigned char CATALOG_IS_UNIQUE = 0x08;
	static const unsigned char CATALOG_IS_NOT_NULL = 0x04;
	static const unsigned char CATALOG_IS_INDEX = 0x02;
	static const unsigned char CATALOG_HAS_NEXT = 0x01;
	size_t Table_Size(string &tablename);
	//功能需求
	void CatalogCreateTable(string & tablename, vector<Attributes> & catalogattributes);			//建立数据表的模式信息文件
	void CatalogCheckCreateTable(string &tablename, vector<Attributes> &	catalogattributes);		//Create数据表的检查
	void CatalogCheckInsertTuple(string &tablename, vector<Tuple> Tuple_Lists);						//检查属性是否正确
	void CatalogInsertTuple(string &tableneame, vector<Tuple> Tuple_Lists);							//数据表插入属性
	void CatalogCheckSelectTuple(queue<string> attributes, queue<string>tablelists);				//Select 的元组的检查

	/*
		文件存放
		把磁盘读取的目录文件存入此处的vector变量中，对目录的修改操作直接针对此处变量
	*/
	static vector<CatalogTable> TableCatalog;														//数据表存放容器
	static vector<CatalogAttributes>AttributesCatalog;												//属性的存放
	/*
		获取操作
	*/
	static Table & CatalogGet_Table(string tablename);												//得到 数据表
	static CatalogTable & CatalogGet_CatalogTable(string tablename);								//得到 数据表的模式信息
	static CatalogAttributes & CatalogGet_Attributes(string tablename,string attributesname);		//得到 数据表中的属性
	/*
		析构函数
	*/
	~Catalog();
	void WriteTable2File();															//将数据表写入文件，由析构函数调用
	void WriteAttributes2File();													//把数据属性写入文件，由析构函数调用
};

#endif