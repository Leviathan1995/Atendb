#pragma once
#ifndef _INTEPRETOR_H_
#define _INTEPRETOR_H
#include "Insert_Into.h"
#include "Attributes.h"
#include "Tuple.h"
#include "Catalog.h"
#include "Glob_Var.h"
#include "Record.h"
#include "Error.h"
#include "API.h"
#include "Select.h"
#include <vector>
#include <string>
#include <queue>
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
	bool Is_CreateIndex(vector<string> Input);			//是否为CreateIndex命令
	bool Is_Select(vector<string> Input);				//是否为Select命令
	bool Is_Insert(vector<string> Input);				//是否为Insert命令
	bool Is_Quit(vector<string> Input);					//是否为Quit命令
	void CreateTable_command(vector<string >Input);		//CreateTable命令解析
	void CreateIndex_command(vector<string> Input);		//CreateIndex命令解析
	void Select_command(vector<string> Input);			//Select命令解析
	void Insert_command(vector<string> Input);			//Insert命令解析
	void Quit_command(vector<string> Input);			//Quit命令解析
	vector<string> Input;//用户输入的命令的集合
	/*
		类型转换
	*/
	Attributes_Type Trasn2Attributestype(string type);//将string转换为字段类型
	static int String2Int(string s);//将string 转换为int
	static const char * String2Char(string s);//将string 转换为 char *
	float String2Float(string s);//将string 转换为float
	//运算符号选择
	static enum Operator_type  Op_Judge(string Op);
};

#endif 