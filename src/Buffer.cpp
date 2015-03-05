#include "Buffer.h"
void M_Buffer::Start()
{
	if (MEM_PAGENUM < 3) throw 1033;//至少要求有 3 页,2页头信息(目录管理和索引管理各一页),另外 1 页记录存取
	this->F_First = NULL;
	this->F_Last = NULL;
	this->F_Current = NULL;
	this->FileCount = 0;
	this->MemPageClock = new M_Clock();
}