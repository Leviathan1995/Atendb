#include "Block.h"
#include <sstream>
#include <strstream>
Block::Block()
{

}
//块的更新
void Block::Block_Update(string &filename, int blocknum, string &content)
{
	Block_Dirty = false;
	Block_Pin = false;
	this->Block_Name = filename;
	this->Block_Num = blocknum;
	this->Block_Content = content;
}
//通过文件名和块号获得对应的键值
string Block::Block_GetKey(string &filename, int blocknum)
{
	//将blocknum int 型转为string 型
	stringstream ss;
	string num;
	ss << blocknum;
	ss >> num;
	string Stringblocknum = ss.str();
	return (filename + string("@") + Stringblocknum);
}