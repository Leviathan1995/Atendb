#include "Record_Manager.h"
//Create建表
bool Record_Manager::Record_ManagerCreateTable(string &tablename, const vector<Attributes>&Table_Column)
{
	string FileName = tablename + ".table";
	string Empty_Block(Block_Size, 0);						//申请一个新的块
	int Tuple_size = 0;										//数据表中元组需要分配的内存大小
	for (int i = 0; i < Table_Column.size(); i++)			//Table_Column.size()数据表的属性个数
		Tuple_size += Table_Column[i].RequestSize;
	Tuple_size++;
	int Offset = 0;//偏移量
	while ((Offset + Tuple_size) < Block_Size)
	{
		Empty_Block.replace(Offset + Tuple_size - 1, 1, "1");//替换
		Offset += Tuple_size;
	}
	Buffer_Manager::Instance().Write(FileName,Empty_Block);
	return true;
}
//Insert into 插入记录
bool Record_Manager::Insert_Into(Table_Type &table, Record R)
{
	if (Buffer_Manager::Instance().File_Exist(table.Table_Name, File_Type::RECORD) == false)
	{
		Buffer_Manager::Instance().CreateFile(table.Table_Name, File_Type::RECORD);
		Buffer_Manager::Instance().New_Block(table.Table_Name, File_Type::RECORD);
	}
	Block block = Buffer_Manager::Instance().ReadLast(table.Table_Name, File_Type::RECORD);
	Byte * blockdata = block.Block_Data;
	int record_size = table.RecordSize + 1;
	int Total_Num = 4096 / record_size;
	for (int i = 0; i < Total_Num; i++)
	{
		Byte * P_Record = blockdata + i*record_size;
		if (P_Record[0] == 0)
		{
			P_Record[0] = 1;
			WriteRecord2Block(P_Record + 1, R);
			Buffer_Manager::Instance().Write(block);
			return block.Block_Offset;
		}
	}
	int Offset = Buffer_Manager::Instance().New_Block(table.Table_Name, File_Type::RECORD);
	block = Buffer_Manager::Instance().Read(table.Table_Name, File_Type::RECORD, Offset);
	Byte * P_Record = block.Block_Data;
	P_Record[0] = 1;
	Copy_block_to_record(P_Record, table);
	File.Write(block);
}
//把记录写入块中
void Record_Manager::WriteRecord2Block(Byte * Position,Record R)
{
	for (vector<Element>::iterator I = R.Mem_Element.begin(); i != R.Mem_Element.end(); i++)
	{
		switch (I->Mem_ColType)
		{
		case Char:
			memcpy(Position, I->Mem_Char.c_str(), I->Mem_CharNum);
			Position += I->Mem_CharNum;
			break;
		case Float:
			memcpy(Position, &I->Mem_Float, sizeof(float));
			Position += sizeof(float);
			break;
		case Int:
			memcpy(Position, &I->Mem_Int, sizeof(int));
			Position += sizeof(int);
			break;
		default:
			break;
		}
	}
}
//返回块中的记录
Record Copy_block_to_record(Byte* Position, Table_Type table)
{
	vector<Column_Type> Attributes = table.Table_Column;
	Record R;
	int Target_Int;
	float Target_Float;
	char Target_Char[255];
	for (vector<Column_Type>::iterator i = Attributes.begin(); i != Attributes.end(); i++)
	{
		Element e;
		Target_Int = *((int*)p);
		Target_Float = *((float*)p);
		memset(Target_Char, 0, 255);
		//字段属性,0 表示 int，1 表示 char(n)，2 表示 float
		switch (i->coltype)
		{
		case 0:
			e.Mem_ColType =Int;
			e.Mem_Int = Target_Int;
			Position += 4;
			break;
		case 1:
			memcpy(Target_Char, Position, i->M_Char_Num);
			e.Mem_ColType = Float;
			Position += 4;
			break;
		case 2:
			e.Mem_ColType = Float;
			e.Mem_Float = Target_Float;
			Position += 4;
			break;
		default:
			break;
		}
		R.Mem_Element.push_back(e);
	}
	return R;

}
void Record_Manager::PrintHead()
{

}