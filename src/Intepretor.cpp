#include "Intepretor.h"
#include "Glob_Var.h"
#include "Catalog.h"
#include "Error.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

using namespace std;
//将string类型的type转换为ColType
ColType Intepretor::Trasn2type(string type)
{
	if (type == "int")
		return ColType::Int;
	if (type == "char")
		return ColType::Char;
	if (type == "float")
		return ColType::Float;
	/*
			如果不属于这三种类型，需要抛出一个错误得救
	*/
}
int Intepretor::String2Int(string s)
{
	int n = atoi(s.c_str());
	return n;
}
//获得输入的命令
void Intepretor::CommandInput()
{
	while (true)
	{
		string command;
		while (cin >> command)
		{
			//去掉（），;
			bool End;//用户是否输入完成
			auto start = command.begin();
			for (auto i = command.begin(); i != command.end(); i++)
			{
				switch (*i)
				{
				case'(':
				case ')':
				case ';':
					if (start != i)
						Input.push_back(string(start, i));
					start = i;
					start++;
					End = true;
				case ',':

				default:
					break;
				}
				if (End == true)
					break;
			}
			if (Is_Quit(Input))
				exit(0);//退出
			else
				ParseCommand();

		}
	}
}
//解析命令
void Intepretor::ParseCommand()
{
	if (Is_CreateTable(Input))
		CreateTable_command(Input);
	if (Is_Select(Input))
		Select_command(Input);
}
bool Intepretor::Is_CreateTable(vector<string> input)
{
	if (input.size() >= 2 && input[0] == "create"&&input[1] == "table")
		return true;
	else
		return false;
}
//创建数据表
void Intepretor::CreateTable_command(vector<string>Input)
{
	Command_State state = Create;
	Table_Type table;
	Column_Type column;
	Column_Type columnprimary;//primary key 约束需要的字段名变量
	for (auto i = Input.begin(); i != Input.end(); i++)
	{
		switch (state)
		{
		case Create:
			state = Table; break;
		case Table:
			state = Table_Name; break;
		case Table_Name:
			state = Left_Query;
			table.Table_Name = *i;
			break;
		case Left_Query:
			if (*i != "(")
				throw Error();
			state = Column_Name;
			break;
		case Column_Name://字段名
			state = Column_type;
			column.Column_TypeName = *i;
			break;
		case Column_type://字段属性
			column.coltype = Trasn2type(*i);
			if (Trasn2type(*i) == Char)
				state = Char_LeftBrackets;
			else
				state = Unique;
			break;
		case Char_LeftBrackets://左括号
			if (*i != "(")
				throw Error();
			state = CharSize;
			break;
		case CharSize://Char类型大小
			column.RequestSize = String2Int(*i);
			state = Char_LeftBrackets;//char的左括号
			break;
		case Char_RightBrackets:
			if (*i != ")")
				throw Error();
			if (*(++i) == "not")
				state = Not_Null_not;
			break;
		case Not_Null_not:
			if (*i != "not")
				throw Error();
			state = Not_Null_null;
			break;
		case Not_Null_null:
			if (*i != "null")
				throw Error();
			column.IsNull = true;
			if (*(++i) == "unique")
				state = Unique;
			break;
		case Unique:
			if (*i != "unique")
				throw Error();
			column.IsUnique = true;
			if (*(++i) == ",")
				state = ColumnEndComma;
			break;
		case ColumnEndComma:
			state = Column_Name;
			table.InsertColumn(column);
			break;
		case PrimaryKey_primary:
			if (*i != "primary")
				throw Error();
			state = PrimaryKey_key;
			break;
		case PrimaryKey_key:
			if (*i != "key")
				throw Error();
			state = PrimaryKey_LeftBrackets;
			break;
		case PrimaryKey_LeftBrackets:
			if (*i != "(")
				throw Error();
			state = PrimaryKey_ColumnName;
			break;
		case PrimaryKey_ColumnName:
			columnprimary = table.GetColumn(*i);
			columnprimary.IsPrimary = true;
			if (*(++i) == ")")
				state = PrimaryKey_RightBrackets;
			if (*(++i) == ",")
				state = PrimaryKey_Comma;
			break;
		case PrimaryKey_Comma:
			state = PrimaryKey_ColumnName;
			break;
		case PrimaryKey_RightBrackets:
			if (*i != ")")
				throw Error();
			if (*(++i) == ";")
				state = EndPrimaryKey;
			break;
		case EndPrimaryKey:
			if (*(++i) == ")")
				state = Right_Query;
			else
				throw Error();
			break;
		case Right_Query:
			break;
		default:
			break;
		}
	}
}
//选择 命令
void Intepretor::Select_command(vector<string> Input)
{

}