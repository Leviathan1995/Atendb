#ifndef _GLOB_VAR_H_
#define _GLOB_VAR_H_
/*
	全局变量
*/
enum MSG {
	CREATE, SELECT, INSERT, UPDATE, DELETE, DROP, Quit,
	SHOWDB, SHOWTABLE, USE, HELP, UNORMAL, NEW, Default, DROPDB
};
enum Operator_type
{
	B,//>
	BE,//>=
	L,//<
	LE,//<=
	NE,//!=
	Between,//between
	ALL//and
};
enum Command_State
{
	Create,
	Table,
	Table_Name,
	Left_Query,
	Right_Query,
	Column_Name,
	Column_type,
	Char_LeftBrackets,//char的左括号
	Char_RightBrackets,//char的右括号
	Unique,//超码约束
	Not_Null_not,//禁止空值中的not
	Not_Null_null,//禁止空值中的null
	CharSize,//字段中Char属性的大小
	EndComma,//每个属性结束的逗号
	PrimaryKey_primary,//Primary Key中的Primary
	PrimaryKey_key,//Primary Key中的key
	PrimaryKey_LeftBrackets,//Primary Key的右括号
	PrimaryKey_RightBrackets,//Primary Key的右括号
	PrimaryKey_ColumnName,//约束为Primary的字段
	EndTable,//结束创表
};
enum ColType
{
	Int,
	Char,
	Float,
};
#endif