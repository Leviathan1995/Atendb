#pragma once
#ifndef _BLOCK_H
#define _BLOCK_H
#include "Glob_Var.h"
#include <string>
using namespace std;
class Block
{
public:
	Block();
	Block(string blockname, int offset, File_Type filetype);
	~Block();
	string Block_Name;
	Byte Block_Data[4096];
	int Block_Offset;
	File_Type Block_FileType;
};
#endif