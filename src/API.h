#ifndef _API_H
#define _API_H
#include "Glob_Var.h"
#include "Intepretor.h"
#include <string>
using namespace std;

/*
	负责调度各种模块.
	接受用户的输入，交由Interpreter处理，通过处理结果，调用
	各个模块的功能实现用户的需求.
*/
struct  InputLink
{
	string Input;
	InputLink *next;
};
//处理输入的字符
class Deal_Input
{
public:
	Deal_Input();//对输入进行处理
	~Deal_Input();//析构函数
	void InitiaInput();//初始化输入字符串
	void OutputLink();//产生字符串集合
	InputLink *Head;//输入字串的头部
	InputLink *Now;//正在处理的输入字串
	int NowDealNumber;//正在处理的字符串的序号

};
class API
{
public:
	Catalog * Mem_Catalog;//实例化的数据表信息
	Selection Select(string table_name, vector<Comparison>);//选择命令
};
#endif