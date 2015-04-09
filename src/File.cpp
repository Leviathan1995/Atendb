#include "File.h"
#include <fstream>
//写入文件
bool Write(string & filename, string & content, int & num)
{
	ofstream Out(filename, ios::in | ios::out | ios::binary);	//  写追加到文件尾
	if (!Out)//如果文件不存在
	{
		//文件不存在，即创建文件
		File::CreateFile(filename);
		Out.open(filename, ios::in | ios::out | ios::binary);
	}
	Out.seekp(0, ios_base::end);									
	long End = Out.tellp();
	num = (End / Block_Size);                                                                // 将(块)尾号作为传出参数
	string toBeWrite(Block_Size, 0);                                                     // ensure the rest of the 'toBeWrite' is 0
	toBeWrite = content;
	Out.seekp(num * Block_Size);
	Out.write(toBeWrite.c_str(), Block_Size);
	Out.close();

	return true;
}
//返回块号
int File::ReadLastNumber(string &filename,string &str)
{
	ifstream In(filename, ios::binary);
	In.seekg(-4096, ios_base::end);
	const long Target = In.tellg();
	char Dst[Block_Size];
	In.read(Dst, Block_Size);
	str = string(Dst, Block_Size);
	return (Target / Block_Size);
}
bool File::Read(string & filename, int blocknum, char *& dst)
{
	ifstream In(filename, ios::binary | fstream::ate);
	if (!In)
	{
		File::CreateFile(filename);
		In.open(filename, ios::binary | fstream::ate);
	}
	if ((size_t)In.tellg() <= blocknum*Block_Size)// 文件中不存在该块(可能出现的错误读操作) ==, <=?
	{
		In.close();
		return false;
	}
	In.seekg(blocknum*Block_Size);
	In.read(dst, Block_Size);
	In.close();
	return true;
}