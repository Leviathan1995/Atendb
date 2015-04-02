#include "Buffer.h"
#include <fstream>
#include <sstream>
#include <assert.h>
using namespace std;
/*
	缓冲区管理器
*/
//写入一个空块中
int Buffer_Manager::Buffer_ManagerWrite(string &filename, string &empty_block,int blocknum)
{
	string  Nouse;
	if (blocknum != -1)											//指定块号的写入
	{
		if (Buffer_ManagerInBuffer(filename, blocknum) == false)//如果不在现存的块中
			Buffer_ManagerFile2Block(filename, blocknum, Nouse);//把文件对应的块读入缓冲区中
		Buffer_ManagerWrite2Block(filename, blocknum, empty_block);
		return blocknum;
		
	}
}
//文件是否在缓冲区中
bool Buffer_Manager::Buffer_ManagerInBuffer(string &fileName, int blocknum)
{
	string Key = Buffer_ManagerGetKey(fileName,blocknum);//获得文件名，块号对应的键值
	return MemBlock_Map.count(Key) == 1;//存储块的Map结构中如果有就返回1

}
//把文件读取到缓冲区的块中
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
			Buffer_ManagerNewBlock(fileName, blockNum, strOut);
	}
}
//建立新的块
bool Buffer_Manager::Buffer_ManagerNewBlock(string & tablename, int blocknum, string & content)
{
	Block * block = new Block();
	block->Block_Update(tablename, blocknum, content);
	Buffer_ManagerUsedBlock.push_back(block);													//将新块插入buffer中的块序列
	Buffer_ManagerBlockMap.insert(make_pair(block->Block_GetKey(tablename, blocknum), block));	//维护Map
	return true;
}
//直接写入块中
bool Buffer_Manager::Buffer_ManagerWrite2Block(string & filename, int blocknum, string & Content)
{
	/*
	用于程序调试
	assert()是一个诊断宏，用于动态辨识程序的逻辑错误条件。其原型是： void assert(int expression);
	如果宏的参数求值结果为非零值，则不做任何操作（no action）；如果是零值，用宽字符打印诊断消息，然后调用abort()。
	*/
	assert(Buffer_ManagerInBuffer(filename, blocknum) == true);
	string key = Block::Block_GetKey(filename, blocknum);
	if (Buffer_ManagerBlockMap[key]->Block_Content != Content)
	{
		Buffer_ManagerBlockMap[key]->Block_Content;
		Buffer_ManagerBlockMap[key]->Block_Dirty = true;				//修改某一数据，并将其写回磁盘
	}
	return true;
}
//缓冲区是否已满
bool Buffer_Manager::IsFull()
{
	return Buffer_ManagerUsedBlock.size() == Block_Size;
}


{
	FILE *FileP = NULL;
	if (filetype == File_Type::RECORD)
		FileP = fopen(Intepretor::String2Char(tablename + ".bol"), "rb");
	else if (filetype == File_Type::INDEX)
		FileP = fopen(Intepretor::String2Char(tablename + ".ind"), "rb");
	if (FileP == NULL)
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

//读取
Block Buffer_Manager::Read(string &tablename, File_Type filetype, int offset)
{
	if (InBuffer(tablename, offset))
		return ReadBlock();//在buffer中，从buffer中
	else
		return File2Block();//不在buffer中，从文件中读


}

bool Buffer::newBlock(const std::string& fileName, const int blockNum, const std::string& content);