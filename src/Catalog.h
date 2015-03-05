#ifndef _CATALOG_H
#define _CATALOG_H
/*
	模式信息.
	即每张表的定义，每张表每个字段的定义
*/
//字段类型
union	Byte_type
{
	int Intvalue; 
	char Charvalue[256];
	float Floatvalue;
};
#endif