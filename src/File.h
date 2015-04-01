#pragma once
#ifndef _FILE_H
#define _FILE_H
#include <string>
#include "Glob_Var.h"
#include "Catalog.h"
using namespace std;
class File
{
public:
	static File & Instance();										//文件实例化
	static bool Read(string &filename,int blocknum,char *& dst);	//读取文件
	static bool Write(string & filename,string & content,int & num);//写入文件中
	static bool Write(Block * block);								//写入文件中
	static void CreateFile(string &filename);						//创建文件
	static int ReadLast();
};
//实例化
File &File::Instance()
{
	static File File_Instance;
	return File_Instance;
}
#endif