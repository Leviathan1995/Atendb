#pragma once
#ifndef _BLOCK_H
#define _BLOCK_H
#include "Glob_Var.h"
#include "Buffer.h"
#include "Intepretor.h"
#include <string>
using namespace std;
class Block
{
public:
	Block();
	void Block_Update(string &filename, int blocknum, string &content);	//块的更新
	static string Block_GetKey(string & filename,int blocknum);			//获取块的键值
	string Block_Name;													//块的名字
	int Block_Num;														//块的偏移量
	bool Block_Pin;														//是否为常驻块
	bool Block_Dirty;													//脏读 标志位
	bool Write2File();													//写入文件中
	string Block_Content;
};
#endif