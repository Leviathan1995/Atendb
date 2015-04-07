#include "Block.h"
Block::Block() :Block_Content(Block_Size, 0)
{
	Block_Dirty = false;
	Block_Num = -1;
}
Block::~Block()
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
	stringstream tmp;
	string num;
	tmp << blocknum;
	tmp >> num;
	return (filename + string("@") + num);
}