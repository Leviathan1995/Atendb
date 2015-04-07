#include "Record_Manager.h"
//Create建表
bool Record_Manager::Record_ManagerCreateTable(string &tablename, const vector<Attributes>&Table_Column)
{
	string FileName = tablename + ".table";
	string Empty_Block(Block_Size, 0);						//申请一个新的块
	int Tuple_size = 0;										//数据表中元组需要分配的内存大小
	for (size_t i = 0; i < Table_Column.size(); i++)			//Table_Column.size()数据表的属性个数
		Tuple_size += Table_Column[i].Attributes_Length;
	Tuple_size++;
	int Offset = 0;//偏移量
	while ((Offset + Tuple_size) < Block_Size)
	{
		Empty_Block.replace(Offset + Tuple_size - 1, 1, "1");//替换
		Offset += Tuple_size;
	}
	Buffer_Manager::Instance().Buffer_ManagerWrite(FileName,Empty_Block);			//通过缓冲区管理写入文件
	return true;
}
//Insert into 插入记录
bool  Record_Manager::Record_ManagerInsert_Into(Table &table, vector<Tuple> Tuple_Lists)
{
	vector<pair<Tuple, int>> insert_tuple;
	int i, j, k;
	string filename = table.Table_Name + ".table";
	char Dirty = '0';
	int Lengh = table.Table_Length() + 1;
	string InsertContent = "";						//用户插入的记录字符串
	string str;
	int blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, str);
	for (size_t i = 0; i < Tuple_Lists.size(); i++)
	{
		Tuple Insert_Tuple = Tuple_Lists[i];
		for (size_t j = 0; j < Insert_Tuple.Tuple_content.size(); i++)
		{
			if (table.Table_AttributesList[i].Attributes_Unique()== true)
			{
				if (Record_ManagerHasExisted(table, Insert_Tuple.Tuple_content[i], i, blocknum) == true)
					return false;
				Insert_Tuple.Tuple_content[i].resize(table.Table_AttributesList[i].Attributes_Length, 0);
				InsertContent += Insert_Tuple.Tuple_content[i];
			}
		}
		InsertContent += Dirty;
		for (k = 0; k <= blocknum; k++) //查找现存块中的空间
		{
			string Strout;
			Buffer_Manager::Instance().Buffer_ManagerRead(filename, i, Strout);	//读取块中的数据
			int Offset = Record_ManagerFindDirtyTuple(Strout, Lengh);			//找到块中空余空间的偏移量
			if (Offset != -1)
			{
				Strout.replace(Offset, Lengh, InsertContent);
				Buffer_Manager::Instance().Buffer_ManagerWrite(filename, Strout, i);
				insert_tuple.push_back(make_pair(Insert_Tuple, i*Block_Size + Offset));
			}
			else
			{
				continue;//如果Offset=-1,说明该块中不存在已经删除的记录，继续从下一个块读
			}
		}
		if (i > blocknum) //缓冲区的块已满
		{
			InsertContent.resize(Block_Size, 0);
			Buffer_Manager::Instance().Buffer_ManagerWrite(filename, InsertContent);
			insert_tuple.push_back(make_pair(Insert_Tuple,blocknum*Block_Size));
		}
	}
	API::Instance().FixIndex(table, insert_tuple);				//修复索引
	return true;
	
}
//元组是否已经存在
bool Record_Manager::Record_ManagerHasExisted(Table &table, string &content, int num, int blocknum)
{

}
//
int Record_Manager::Record_ManagerFindDirtyTuple(string &strout, int size)
{
	int Offset = 0;
	while ((Offset + size) < strout.size())
	{
		string str = strout.substr(Offset, size);	
		string Dirty = str.substr(size - 1, 1);		//size-1 后全部替换为1
		if (Dirty == "1")							//如果找到被修改的位置				
			return Offset;							//返回偏移量
		Offset += size;
	}
	return -1;
}