#include "Buffer.h"
#include <fstream>
#include <sstream>
using namespace std;
/*
	缓冲区管理器
*/
int Buffer_Manager::Write(string & FileName,string & Content,int BlockNum)
{
	string Nouse;
	if (BlockNum != -1)
	{
		if (InBuffer(FileName, BlockNum) == false)//如果不在Buffer中，就把文件对应的块写入Buffer
			File2Block(FileName, BlockNum, Nouse);
		Write2Blcok(FileName, BlockNum, Content);
		return BlockNum;
	}
	else
	{
		int num;
		File::Write(FileName, Content, num);
		File2Bolck(FileName, num, Nouse);
		return num;
	}
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
	B_Block * cur;
	while ((cur = MemBlock_Used.front())->IsPin())
	{
		MemBlock_Used.pop_front();
		MemBlock_Used.push_back(cur);
	}
	if (cur->IsDirty())
		cur->Write2File();
	string Oldkey = cur->GetKey();
	string Newkey = B_Block::GetKey();
	cur->Update(FileName, BlockNum, Content);
	MemBlock_Map.erase(MemBlock_Map.find(Oldkey));
	MemBlock_Map.insert(make_pair(Newkey, cur));
	return true;
}
/*
	把文件写入块中
*/
bool Buffer_Manager::File2Block(string& fileName, int blockNum, string& Strout)
{
	char * Empty = new char[Block_Size];//读取文件传出的参数
	if (B_File::Read(fileName, blockNum, Empty) == false)//读失败(blockNum大于文件的块总数时)
	{
		delete[] Empty;
		return false;
	}
	else
	{
		Strout = string(Empty, Block_Size);
		/*
			如果缓冲区已经满了，
			使用LRU算法进行替换
		*/
		if (IsFull())
			Replace(fileName, blockNum, Strout);
		else
			Built_NewBlock();//否则建一个新的块
		delete[] Empty;
		return true;
	}
}
//缓冲区是否已满
bool Buffer_Manager::IsFull()
{
	return MemBlock_Used.size() == Block_Size;
}
//文件写入
bool Buffer_Manager::B_File::Write(B_Block * cur)
{
	ofstream Output(cur->FileName, ios::in | ios::out | ios::binary);
	if (File_NotIn())
	{
		B_File::CreateFile(cur->FileName);
		Output.open(cur->FileName, ios::in | ios::out | ios::binary);
	}
	/*
	seekp:设置输出文件流的文件流指针位置
	*/
	Output.seekp(Block_Size*cur->Block_Num);//定位写入文件的指针位置
	Output.write(cur->Block_Content.c_str(), Block_Size);
	Output.clear();
	return true;
}
bool Buffer_Manager::B_File::Write(string &FileName, string &Content, int & Num)
{
	ofstream Output(FileName, ios::in | ios::out | ios::binary);
	if (File_NotIn())
	{
		B_File::CreateFile(FileName);
		Output.open(FileName, ios::in | ios::out | ios::binary);
	}
	Output.seekp(0, ios_base::end);
	long End = Output.tellp();//表示内置指针的当前位置
	Num = (End / Block_Size);
	string ToWrite(Block_Size, 0);
	ToWrite = Content;
	Output.seekp(Num*Block_Size);
	Output.write(ToWrite.c_str(), Block_Size);
	Output.close();
	return true;
}