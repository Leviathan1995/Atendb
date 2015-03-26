#include "Record_Manager.h"
//建表
bool Record_Manager::CreateTable(string tablename, vector<Column_Type>&Table_Column)
{
	string FileName = tablename + ".table";
	string Empty_Block(Block_Size, 0);//申请一个新的块
	int tuple_size = 0;//数据表中元组需要分配的内存大小
	for (int i = 0; i < Table_Column.size(); i++)//Table_Column.size()数据表的属性个数
		tuple_size += Table_Column[i].RequestSize;
	tuple_size++;
	int Offset = 0;//偏移量
	while ((Offset + tuple_size) < Block_Size)
	{
		Empty_Block.replace(Offset + tuple_size - 1, 1, "1");//替换
		Offset += tuple_size;
	}
	Buffer_Manager::Instance().Write(FileName,Empty_Block);
	return true;
}
void Record_Manager::PrintHead()
{

}