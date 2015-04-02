#pragma once
#ifndef _INSERT_INTO_H
#define _INSERT_INTO_H
#include "Catalog.h"
#include <string>
/*
	Insert into 命令接口
*/
//Insert_into 中的values的结构体
struct Insert_IntoStruct
{
	Attributes_Type Valuetype;
	string CharValues;
	int  IntValues;
	float FloatValues;
};
class Insert_Into
{
public:
	vector<Insert_IntoStruct> InsertIntoValues;//Insert into values 中的values的集合
	void Insert_IntoParse();//insert into 命令解析
};
#endif