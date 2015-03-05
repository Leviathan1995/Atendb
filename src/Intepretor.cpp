#include "Intepretor.h"
#include "Glob_Var.h"
#include <iostream>
using namespace std;

//得到输入的命令
MSG Getcommand(SQL_Create **)
{
	MSG Commandtype = UNORMAL;//初始化为错误的命令
	
}

//处理输入字符的类的构造函数，对输入进行处理，产生一个有以字符串的集合
Deal_Input::Deal_Input()
{
	InitiaInput();//初始化输入字符串
	OutputLink();//产生字符串集合
}
//初始化输入字符串
void Deal_Input::InitiaInput()
{
	this->Head = NULL;
	this->Now = NULL;
	this->NowDealNumber = 0;
}