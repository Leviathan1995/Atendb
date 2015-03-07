#ifndef _INTEPRETOR_H_
#define _INTEPRETOR_H
#include <string>
#include "Catalog.h"
#include "Glob_Var.h"
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
	bool Is_CreateTable(vector<string> Input);//是CreateTable命令
	bool Is_Quit(vector<string> Input);//是Quit命令
	void CreateTable(vector<string >Input);//CreateTable命令解析
	vector<string> Input;//用户输入的命令
};
#endif 