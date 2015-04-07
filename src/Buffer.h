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
#include "Block.h"
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
	list<Block *> Buffer_ManagerUsedBlock;													//buffer中使用的块
	map<string, Block*>Buffer_ManagerBlockMap;												//buffer中的块对应字典
	static Buffer_Manager & Instance();														//缓冲管理器实例化
	int Buffer_ManagerWrite(string &filename, string &empty_block,int blocknum=-1);			//写入，返回写入的块号
	bool Buffer_ManagerInBuffer(string & fileame, int blocknum);							//检测在不在缓冲区中
	string Buffer_ManagerGetKey(string &filename, int blocknum);							//通过文件名和块号获得其键值
	bool Buffer_ManagerFile2Block(string& fileName, int blocknum, string& Strout);		    //把文件读入到缓冲区块中
	bool Buffer_ManagerNewBlock(string& fileName, int blocknum, string& content);			//申请新块
	bool Buffer_ManagerWrite2Block(string& fileName, int blockNum, string& content);		//写入到缓冲区的块中
	int Buffer_ManagerReadLastNumber(string & filename, string &str);						//返回块号
	bool Buffer_ManagerRead(string &filename, int blocknum, string &strout);				//读取
	bool Buffer_ManagerIsFull();															//缓冲区的块是否已满
	bool Buffer_ManagerReadBlock(string &filename,int blocknum,string &strout);				//从缓冲区的块中读取
	bool Buffer_ManagerReplace(string &filename, int blocknum, string &strout);				//块中已满，进行替换
};
//缓冲管理区的实例化
inline Buffer_Manager & Buffer_Manager::Instance()
{
	static Buffer_Manager Buffer_Instance;
	return Buffer_Instance;
}
#endif