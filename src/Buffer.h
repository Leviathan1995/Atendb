#ifndef _BUFFER_H
#define _BUFFER_H
#include <string>
using namespace std;
/*
	缓存管理器
*/
#define Block_Size 4096	// 块 4KB
#define Bolck_Num	  10000	// 块的数量
#define MAX_FILESIZE   256	// 文件的最大长度

class Buffer_Manager
{
public:
	Buffer_Manager();
	~Buffer_Manager();
	Buffer_Manager & Instance();//缓冲管理器实例化
};
class B_Block
{
public:
	bool Blcok_Ditry;
	int Block_Num;
	string FileName;
	string Content;
	B_Block();
	bool Dirty();
	bool Write2File();
	string GetKey();
	void Update(string & FileName, int Block_Num, string & Content);
};
class B_File
{
public:
	bool Read(string & FileName, int Block_Num, char *& Dst);
	bool Write(B_Block *Current);
};
inline Buffer_Manager & Buffer_Manager::Instance()
{
	Buffer_Manager Buffer_Instance;
	return Buffer_Instance;
}
#endif