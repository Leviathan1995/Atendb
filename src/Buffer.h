#pragma once
#ifndef _BUFFER_H
#define _BUFFER_H
#include "Glob_Var.h"
#include "Intepretor.h"
#include "Block.h"
#include "File.h"
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
using namespace std;
/*
	缓存管理器
*/
#define Block_Size 4096	// 块 4KB
#define Block_Tol	  10000	// 块的数量
#define MAX_FILESIZE   256	// 文件的最大长度
class Buffer_Manager
{
public:
	Buffer_Manager();
	~Buffer_Manager();
	static Buffer_Manager & Instance();									  //缓冲管理器实例化
	void Buffer_ManagerWrite(string &filename, string &empty_block);	  //写入一个空块中

	Block Read(string &tablename, File_Type filetype, int offset);		  //读取
	bool File_Exist(string &tablename, File_Type filetyppe);			  //文件是否存在
	Block ReadLast(string & FileName, File_Type filetype);				  //返回块
	bool Write2Block(string& fileName,int blockNum,string& content);	  //写入块中
	bool InBuffer(string FileName, int BlockNum);						  //检测在不在缓冲区中
	map<string, Block*> MemBlock_Map;									  //记录使用的块
	list<Block *> MemBlock_Used;										  //已经使用过的块
	bool Replace(string& FileName,int blockNum,string& content);		  //替换算法
	static void CreateFile(string &tablename, File_Type filetype);        //创建文件
	bool IsFull();														  //缓冲区是否已满
	int ReadLastNumber(string & filename, string &str);					  //返回块号
	bool File2Block(string& fileName, int blockNum, string& Strout);	  //把文件写入块中
	bool newBlock(string& fileName,int blockNum,string& content);		  //申请新块
};
//缓冲管理区的实例化
inline Buffer_Manager & Buffer_Manager::Instance()
{
	static Buffer_Manager Buffer_Instance;
	return Buffer_Instance;
}
#endif