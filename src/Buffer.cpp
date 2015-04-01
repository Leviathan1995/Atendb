#include "Buffer.h"
#include <fstream>
#include <sstream>
#include <assert.h>
using namespace std;
/*
	缓冲区管理器
*/

//直接写入块中
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
int Buffer_Manager::Write(string& filename,string& content,int blocknum = -1)
{
	string Nouse;
	if (blocknum != -1)//指定块号的写入
	{
		if (InBuffer(filename, blocknum) == false)
			File2Block(filename, blocknum, Nouse);	// 不在buffer中，将文件中对应的块读入buffer (传入的noUse是为与read接口兼容)
		Write2Block(filename, blocknum, content);	// 直接将content写入buffer中的指定的blocknum块
		return blocknum;
	}
	else
	{
		int num;
		File::Write(filename,content,num);//先写入文件中
		File2Block();//再把文件写入缓冲区的块中
		return num;
	}
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
	if (InBuffer(tablename, offset))
		return ReadBlock();//在buffer中，从buffer中
	else
		return File2Block();//不在buffer中，从文件中读


}
//把文件写入块中
bool Buffer_Manager::File2Block(string& fileName, const int blockNum, string& strOut)
{
	char *Dst = new char[Block_Size];//申请一个块大小的内存
	if (File::Read(fileName, blockNum, Dst) == false)
	{
		delete[] Dst;
		return false;
	}
	else
	{
		strOut = string(Dst, Block_Size);
		if (IsFull())//如果缓冲区已经满了
			Replace(fileName, blockNum, strOut);//利用替换算法 替换进来 
		else
			New_Block(fileName, blockNum, strOut);
	}
}
bool Buffer::newBlock(const std::string& fileName, const int blockNum, const std::string& content);