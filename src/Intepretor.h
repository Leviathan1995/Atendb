#ifndef _INTEPRETOR_H_
#define _INTEPRETOR_H
#include <string>
#include "Catalog.h"
#include "Glob_Var.h"
#endif
//表示Create命令信息的结构体
struct Create_information
{
	char ColumnName[32];	//字段名.属性需要用定长或变长的字节序列表示，称作“字段”
	Byte_type Coltype;		//字段类型
	int length;				//字段长度
	Operator_type OperType; //关系运算符
	bool Isnull;			//有没有不为NULL的要求
	bool Isprimary;			//是不是Primary key
	SQL_Create *next;		//下一个字段
};
//Create命令的框架结构
class SQL_Create
{
public:
	int TotalColumn;
	Create_information *Head;//字段头
	char Tablename[32];
	SQL_Create(int, Create_information, char *);//构造函数，即初始化函数，将所有的信息整合成一张表
	~SQL_Create()
	{
		delete Head;
	}
};
