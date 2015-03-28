#ifndef _ERROR_H
#define _ERROR_H
#include <string>
using namespace std;
/*
	错误代码字段
*/

#define NO_Error   0
#define Default     1
#define NO_SUCH_TABLE                                                                   1001
#define NO_SUCH_ATTRIBUTE                                                               1002
#define ONLY_CHAR_HAS_LENGTH															1003
#define CHAR_MUST_HAVE_LENGTH															1004
#define ALREADY_EXIST_SUCH_TABLE														1005
#define ALREADY_EXIST_SUCH_ATTRIBUTE										  			1006
#define CAN_NOT_CREATE_ATTRIBUTE														1007
#define CAN_NOT_CREATE_TABLE															1008
#define ALREADY_EXIST_SUCH_INDEX														1009
#define NO_SUCH_INDEX																	1010
#define CAN_NOT_OPEN_TABLE_FILE															1011
#define CAN_NOT_OPEN_DEF_FILE															1012
#define TAB_FILE_SYNTAX_ERROR															1013
#define DEF_FILE_SYNTAX_ERROR															1014
#define INDEX_NAME_CANT_BE_BLANK
class Error
{
public:
	int  Error_Code;
	string Function;//发生错误的函数
	string Module;//发生错误的模块
	string Error_string;//错误描述
	Error();//默认构造函数
	Error(int Error_Code, string Module, string Function, string Error_string);
};
#endif