#include "Buffer.h"
#include <fstream>
#include <sstream>
#include <assert.h>
using namespace std;
/*
	缓冲区管理器
*/
Buffer_Manager::~Buffer_Manager()
{
	for (auto i = Buffer_ManagerUsedBlock.begin(); i != Buffer_ManagerUsedBlock.end(); i++)
	{
		if ((*i)->Block_Dirty)
		{
			File::Write(*i);
		}
	}
}
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
	return Buffer_ManagerBlockMap.count(Key) == 1;//存储块的Map结构中如果有就返回1

}
//从文件读取，写入到缓冲区的块中
bool Buffer_Manager::Buffer_ManagerFile2Block(string& fileName, const int blockNum, string& strOut)
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
		if (Buffer_ManagerIsFull())//如果缓冲区已经满了
			Buffer_ManagerReplace(fileName, blockNum, strOut);//利用替换算法 替换进来 
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
bool Buffer_Manager::Buffer_ManagerIsFull()
{
	return Buffer_ManagerUsedBlock.size() == Block_Size;
}
//返回块号
int Buffer_Manager::Buffer_ManagerReadLastNumber(string & filename, string &str)
{
	File::Instance().ReadLastNumber(filename, str);
}
//读取
bool Buffer_Manager::Buffer_ManagerRead(string &filename,int blocknum,string &strout)
{
	if (Buffer_ManagerInBuffer(filename, blocknum)) //检测是否在缓冲区内
		return Buffer_ManagerReadBlock(filename,blocknum,strout);//如果在缓冲区就从缓冲区读取
	else
		return Buffer_ManagerFile2Block(filename,blocknum,strout);//否则从文件中读取
}
//从缓冲区的块中读取,供Buffer_ManagerRead()调用
bool Buffer_Manager::Buffer_ManagerReadBlock(string &filename, int blocknum, string &strout)
{
	string Key = Buffer_ManagerGetKey(filename, blocknum);
	if (Buffer_ManagerBlockMap[Key]->Block_Pin == false)						 // 在该块非常驻块时，进行LRU策略维护
	{
		Buffer_ManagerUsedBlock.remove(Buffer_ManagerBlockMap[Key]);			//List Buffer_ManagerUsedBlock 进行维护
		Buffer_ManagerUsedBlock.push_back(Buffer_ManagerBlockMap[Key]);
	}
	strout = Buffer_ManagerBlockMap[Key]->Block_Content;
	return true;
}
//不在缓冲区内，从文件中读取
bool Buffer_Manager::Buffer_ManagerFile2Block(string &filename, int blocknum, string &strout)
{
	char *Dst = new char[Block_Size];
	//如果从文件中读取失败，返回False
	if (File::Read(filename, blocknum, Dst) == false)
	{
		delete[] Dst;
		return false;
	}
	else
	{
		strout = string(Dst, Block_Size);
		if (Buffer_ManagerIsFull())
			Buffer_ManagerReplace(filename,blocknum,strout);//如果缓冲区已经满了，就进行替换
		else
			Buffer_ManagerNewBlock(filename, blocknum, strout);//否则申请一个新的块，加入到缓冲区内
	}
	delete[] Dst;
	return true;
}
//缓冲区已满，进行替换
bool Buffer_Manager::Buffer_ManagerReplace(string &filename,int blocknum,string &strout)
{
	Block *ReplaceBlock;
	while ((ReplaceBlock = Buffer_ManagerUsedBlock.front())->Block_Pin)		//找到非 常驻块
	{
		Buffer_ManagerUsedBlock.pop_front();
		Buffer_ManagerUsedBlock.push_back(ReplaceBlock);
	}
	if (ReplaceBlock->Block_Dirty)					//如果被更改过，就将其写入文件中
		ReplaceBlock->Write2File();
	string Key = ReplaceBlock->Block_GetKey(filename,blocknum);
	string NewKey = Buffer_ManagerGetKey(filename, blocknum);
	ReplaceBlock->Block_Update(filename, blocknum, strout);			//在缓冲区内进行块的更新
	Buffer_ManagerBlockMap.erase(Buffer_ManagerBlockMap.find(Key));	//移出旧的块
	Buffer_ManagerBlockMap.insert(make_pair(NewKey, ReplaceBlock));	//添加新的块
	return true;
}
