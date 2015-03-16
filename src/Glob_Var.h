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
	LESS_THAN,//<
	EQUAL,//=
	MORE_THAN,//>
	MORE_AND_EQUAL,//>=
	LESS_AND_EQUAL,//<=
	NOT_EQUAL,//!=  另 where子句不能使用！=运算符

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
	ColumnEndComma,//每个属性结束的逗号
	PrimaryKey_primary,//Primary Key中的Primary
	PrimaryKey_key,//Primary Key中的key
	PrimaryKey_LeftBrackets,//Primary Key的右括号
	PrimaryKey_RightBrackets,//Primary Key的右括号
	PrimaryKey_Comma,//Primary Key中各种属性分隔的逗号
	PrimaryKey_ColumnName,//约束为Primary的字段
	EndPrimaryKey,//Primary key约束结束的分号
	EndTable,//结束创表
	Select,//选择
	SelList,//选择的列表
	From,//select from 中的from
	FromList,//Select中From的列表
	Where,//Select中Where
	And ,//Select中的where中And
	EndSelect,//Select中的End
	Insert,//Insert into 中的Insert
	Into,//Insert into中的 Into
	InsertTable,//Insert into 的数据表
	Insert_Value,//Insert into values 的values
	Insert_Rightbracket,//Insert into values 的左括号
};
//字段属性
enum ColType
{
	Int,
	Char,
	Float,
};
#endif