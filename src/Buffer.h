#ifndef _BUFFER_H
#define _BUFFER_H
#include <string>
#include <map>
#include <list>
using namespace std;
/*
	缓存管理器
*/
#define Block_Size 4096	// 块 4KB
#define Block_Tol	  10000	// 块的数量
#define MAX_FILESIZE   256	// 文件的最大长度
class Buffer_Manager
{
	class B_Block;
	class B_File;
public:
	Buffer_Manager();
	~Buffer_Manager();
	Buffer_Manager & Instance();//缓冲管理器实例化
	int  Write(string & FileName, string & Content, int BolckNum = -1);
	bool Write2Block(string& fileName,int blockNum,string& content);
	bool InBuffer(string FileName, int BlockNum);//检测在不在缓冲区中
	map<string, B_Block*> MemBlock_Map;//记录使用的块
	list<B_Block *> MemBlock_Used;//已经使用过的块
	bool Built_NewBlock(string FileName,int BlockNum,string & str);//建立新的块
	bool File2Block(string& fileName, int blockNum, string& strOut);
	bool IsFull();
	bool Replace(string& FileName,int blockNum,string& content);//替换算法
	class B_Block
	{
	public:
		string Block_Content;//块的目录
		/*
		文件系统检查缓冲池中是否有该文件中的块，
		有则检查这些块是否写过，写过这些块即为 dirty 块
		*/
		bool Block_Pin;
		bool Blcok_Ditry;
		int Block_Num;
		string FileName;
		string Content;
		B_Block();
		static string GetKey();
		bool IsPin();
		bool IsDirty();
		bool Write2File();
		void Update(string & FileName, int Block_Num, string & Content);
	};
	class B_File
	{
	public:
		static bool Read(string & FileName, int Block_Num, char *& Dst);
		static bool Write(B_Block *Current);
		static bool Write(string &FileName, string &Content, int & Num);
		static void CreateFile(string FileName);
		static bool File_NotIn();//文件已经被创建

	};
};
//缓冲管理区的实例化
inline Buffer_Manager & Buffer_Manager::Instance()
{
	Buffer_Manager Buffer_Instance;
	return Buffer_Instance;
}
#endif