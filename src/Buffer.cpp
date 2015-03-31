#include "Buffer.h"
#include <fstream>
#include <sstream>
#include <assert.h>
using namespace std;
/*
	缓冲区管理器
*/

//写入块中
bool Buffer_Manager::Write2Block(string & FileName, int Blocknum, string & Content)
{
	assert(InBuffer(FileName, Blocknum) == true);
	string key = B_Block::GetKey();
	if (MemBlock_Map[key]->Content != Content)
	{
		MemBlock_Map[key]->Content = Content;
		MemBlock_Map[key]->Blcok_Ditry = true;
	}
	return true;
}
/*
	文件是否在缓冲区中
*/
bool Buffer_Manager::InBuffer(string FileName, int BlockNum)
{
	string Key = B_Block::GetKey();//获得文件名，块号对应的键值
	return MemBlock_Map.count(Key) == 1;//存储块的Map结构中如果有就返回1

}
/*
	内存中块的置换
	LRU ->即最少使用页面算法
*/
bool Buffer_Manager::Replace(string & FileName, int BlockNum, string & Content)
{
	Block * cur;
	while ((cur = MemBlock_Used.front())->IsPin())
	{
		MemBlock_Used.pop_front();
		MemBlock_Used.push_back(cur);
	}
	if (cur->IsDirty())
		cur->Write2File();
	string Oldkey = cur->GetKey();
	string Newkey = Block::GetKey();
	cur->Update(FileName, BlockNum, Content);
	MemBlock_Map.erase(MemBlock_Map.find(Oldkey));
	MemBlock_Map.insert(make_pair(Newkey, cur));
	return true;
}
//缓冲区是否已满
bool Buffer_Manager::IsFull()
{
	return MemBlock_Used.size() == Block_Size;
}
//文件写入
void Buffer_Manager::Write(Block block)
{
	FILE* fp = NULL;
	if (block.Block_Type== File_Type::RECORD)
		fp = fopen(Intepretor::String2Char(block.Block_Name + ".bol"), "rb");
	else if (block.Block_Type == File_Type::INDEX)
		fp = fopen(Intepretor::String2Char(block.Block_Name + ".ind"), "rb");
	if (fp == NULL)
	{
		throw string("block type error");
	}
	fseek(fp, block.Block_Offset * 4096, SEEK_SET);
	fwrite(block.Block_Data, sizeof(Byte), 4096, fp);
	fclose(fp);
}
//返回块
Block Buffer_Manager::ReadLast(string & tablename,File_Type filetype)
{
	B_Block block;
	FILE* fp = NULL;
	if (filetype== File_Type::RECORD)
		fp = fopen(Intepretor::String2Char(tablename + ".bol"), "rb");
	else if (filetype == File_Type::INDEX)
		fp = fopen(Intepretor::String2Char(tablename + ".bol"), "rb");
	if (fp == NULL)
	{
		throw string("no such file");
	}
	fseek(fp, 0, SEEK_END);
	//if (ftell(fp) == 0)
	//	throw error();

	fseek(fp, -4096, SEEK_END);
	int offset = ftell(fp) / 4096;
	fread(block.Block_Data , sizeof(Byte), 4096, fp);
	fclose(fp);
	block.Block_Offset = offset;
	block.Block_Name = tablename;
	block.Block_Type = filetype;
	return block;
}
//返回块号
int Buffer_Manager::ReadLastNumber(string & filename, string &str)
{
	ifstream In(filename, ios::binary);
	In.seekg(-4096, ios_base::end);
	const long Target = In.tellg();
	char Dst[Block_Size];
	In.read(Dst, Block_Size);
	str = string(Dst, Block_Size);
	return (Target / Block_Size);
}
//查找文件是否存在
bool Buffer_Manager::File_Exist(string  &tablename, File_Type filetype)
{
	FILE *FileP = NULL;
	if (filetype == File_Type::RECORD)
		FileP = fopen(Intepretor::String2Char(tablename + ".bol"), "rb");
	else if (filetype == File_Type::INDEX)
		FileP = fopen(Intepretor::String2Char(tablename + ".ind"), "rb");
	if (FileP == NULL)
		return false;
	else
		return true;
}
//创建文件
void Buffer_Manager::CreateFile(string & tablename, File_Type filetype)
{
	FILE * FileP = NULL;
	if (filetype == File_Type::RECORD)
		FileP = fopen(Intepretor::String2Char(tablename + ".bol"), "wb");
	else if (filetype == File_Type::INDEX)
		FileP = fopen(Intepretor::String2Char(tablename + ".ind"), "wb");
	if (FileP == NULL)
		throw string("File type error");
	fclose(FileP);
}
//建立新的块
int Buffer_Manager::New_Block(string & tablename, File_Type filetype)
{
	FILE *FileP = NULL;
	if (filetype == File_Type::RECORD)
		FileP = fopen(Intepretor::String2Char(tablename + ".bol"), "rb");
	else if (filetype == File_Type::INDEX)
		FileP = fopen(Intepretor::String2Char(tablename + ".ind"), "rb");
		if (FileP== NULL)
		{
			throw string("build new block failness");
		}
		fseek(FileP, 0, SEEK_END);
		int offset = ftell(FileP) / 4096;
		Block block;
		fwrite(block.Block_Data, sizeof(Byte), 4096, FileP);
		fclose(FileP);
		return offset;
}
//读取
Block Buffer_Manager::Read(string &tablename, File_Type filetype, int offset)
{
	Block block;
}