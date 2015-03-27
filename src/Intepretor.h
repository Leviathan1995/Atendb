#ifndef _INTEPRETOR_H_
#define _INTEPRETOR_H
#include <string>
#include "Catalog.h"
#include "Glob_Var.h"
#include "Record.h"
#include "Error.h"
#include "API.h"
#include <vector>
#include <string>
using namespace std;
/*
	接受用户输入的SQL语句，并检验用户输入的SQL语句及格式
	检验其正确性。同时将符合格式要求的语句转换为内部形式，供
	主函数使用。而对不符合格式要求的语句，输出错误信息，供用
	户参考。
*/


class Intepretor
{
public:
	void CommandInput();//输入命令
	void ParseCommand();//解析命令
	/*
		命令
	*/
	bool Is_CreateTable(vector<string> Input);			//是否为CreateTable命令
	bool Is_Select(vector<string> Input);				//是否为Select命令
	bool Is_Insert(vector<string> Input);				//是否为Insert命令
	bool Is_Quit(vector<string> Input);					//是否为Quit命令
	void CreateTable_command(vector<string >Input);		//CreateTable命令解析
	void Select_command(vector<string> Input);			//Select命令解析
	void Insert_command(vector<string> Input);			//Insert命令解析
	vector<string> Input;//用户输入的命令的集合
	/*
		类型转换
	*/
	ColType Trasn2type(string type);//将string转换为类型
	int String2Int(string s);//将string 转换为int
	const char * String2Char(string s);//将string 转换为 char *
	
};
//Select 中的where结构体
struct WhereList
{
	string Attribute;
	string Where_Operator;
	union
	{
		const char *StrValue;
		int IntValue;
	};
};
//Select 命令的接口
class Selection
{
	friend class Intepretor;
public:

	static vector<string> SelLists;//即选择的属性
	static vector<string> TableLists;//select 中from的数据表列表
	static vector<WhereList> WhereLists;//slect 中where部分
	static void SelectionInput(string *Sel,string *table,WhereList *& wherelist,int wherenum);//获取用户输入的属性和数据表和where部分
	void Selection_Parse();//解析用户输入的Select命令
	void Print_SelectHead();//打印Select的属性头
	vector<Record> Mem_Record;
	vector<Column_Type> Mem_SelectColumn;//经过选择后的元组
	Table_Type Mem_Table;
};
//Insert_into 中的values的结构体
struct Insert_IntoStruct
{
	ColType Valuetype;
	string CharValues;
	int  IntValues;
	float FloatValues;
};
class Insert_Into
{
	friend class Intepretor;
public:
	vector<Insert_IntoStruct> InsertIntoValues;//Insert into values 中的values的集合
	void Insert_IntoParse();//insert into 命令解析
};
#endif 