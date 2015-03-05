#ifndef _INTEPRETOR_H_
#define _INTEPRETOR_H
#endif
//表示Create命令中一列信息的结构体
struct Create_information
{
	char ColumnName[32];
};
//Create命令
class SQL_Create
{
public:
	int TotalColumn;
	char Tablename[32];

};