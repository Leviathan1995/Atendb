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
#endif