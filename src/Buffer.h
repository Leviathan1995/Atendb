#ifndef _BUFFER_H
#define _BUFFER_H
//在C++中引用C语言中的函数和变量，在包含C语言头文件时，需进行下列处理：
extern "C"
{
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
}
#define FILE_PAGESIZE 4096	// 块 4KB
#define MEM_PAGENUM	  1000	// 块的数量
#ifndef MAX_FILESIZE
#define MAX_FILESIZE   256	// 文件的最大长度
#endif

//页头信息，标识文件页
typedef struct
{
	unsigned long PageID;		//页编号
	bool Mem_Fixed;				//是否常驻内存
	void InitPage(unsigned long pageID, bool mem_fixed);//初始化操作
}Page_Head;

//文件内存地址
class F_FileAdd
{
public :
	unsigned long FilePageID; //页编号
	unsigned int PageOffset;  //页内偏移量
	void InitiaFile();		  //初始化（0.0）
	void *MemAdd() const;	  //根据页编号和偏移量，取出当前文件该地址在内存中的地址,同时测试页偏移量是否溢出
	void *MemAdd(class M_File &) const;//根据页编号和偏移量，取出文件该地址在内存中的地址,同时测试页偏移量是否溢出
	void *MemAdd(class M_File *) const;//根据页编号和偏移量，取出文件该地址在内存中的地址,同时测试页偏移量是否溢出
	bool operator>=(F_FileAdd& other) const; // _F_FileAddr >= 操作
	bool operator> (F_FileAdd& other) const; // _F_FileAddr >  操作
	bool operator==(F_FileAdd& other) const; // _F_FileAddr == 操作
	bool operator!=(F_FileAdd& other) const; // _F_FileAddr != 操作
	bool operator==(const int zero) const;   // _F_FileAddr == 操作
	void ShiftOffset(int offset);			 //在当前页中滑动offset量(可正可负)
};

//文件头信息
typedef struct
{
	F_FileAdd DelFirst;             // 第一条被删除记录地址
	F_FileAdd DelLast;              // 最后一条被删除记录地址  
	F_FileAdd NewInsert;            // 文件末尾可插入新数据的地址
	unsigned long ulPageTotal;        // 目前文件中共有页数
	void InitialFileCond();           // 初始化
}File_Head;

//内存页
class M_Page
{
	friend class M_Page_Info;
	friend class M_FileAdd;
	friend class M_Clock;//采用“时钟”的策略
private:
	unsigned int M_FileID;//所属文件编号（动态分配）
	unsigned long M_FilePageID;//在文件中的页的ID
	void *PageStart;//调入内存后的页首地址
	Page_Head *PtrHead;//页头信息
	M_Page();//成员初始化，开辟内存空间
	~M_Page();//释放内存空间
	//从文件调入页到开辟好的内存空间
	void LoadFromFile(unsigned int fileID, unsigned long filepageID);
	//把内存中的页写回到文件中
	void BacktoFile()const;
};

//内存页管理信息
class M_PageInfo
{
	friend class M_Clock;
	friend class F_FileAdd;
	friend class M_Buffer;
	bool IsLastUsed;//最近一次访问内存是否被使用，用于Clock算法
	bool IsModiFied;//从调入内存开始，是否被修改，用于决定是否需要写文件
	class M_Page * PtrtoPage;//所分配的内存页对象
	M_PageInfo();//成员初始化
	~M_PageInfo();//析构
	void UpdatePageInfo(unsigned int fileID, unsigned long filepageID); //页替换，开辟
	Page_Head * GetPtrHead() const;//获得页头信息地址
	File_Head * GetPtrFileHead() const;//获得文件头信息地址
	unsigned int GetFileID() const;//获得所分配的内存页目前内容所属的文件编号
	void SetFileID(unsigned int fileID); //设置新的文件编号（抛弃页时设为0）
	unsigned long GetFilePageID() const;//获得所分配的内存页目前内容在文件中的页编号
};
#endif