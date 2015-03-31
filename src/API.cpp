#include "API.h"
using namespace std;
API::API()
{
}
//Slect命令
void API::Select(string table_name, vector<Comparison>)
{
	Table_Type table_select = Catalog::Instance().Get_Table(table_name);
}
//Create建表
void API::CreateTable(string & tablename,vector<Column_Type> &Attributes)
{
	try
	{
		Catalog::Instance().CatalogCreateTable(tablename, Attributes);//直接传给模式信息类创建表
	}
	catch (string errsrt)
	{
		throw;
	}
	Record_Manager::Instance().CreateTable(tablename, Attributes);//传给磁盘管理系统创建表
}
//insert into 插入记录
void API::Insert_Into(string &tablename, Record R)
{
	Catalog::Instance().CatalogInsertColumn(tablename, R);
	Table_Type table = Catalog::Instance().Get_Table(tablename);
	/*
	Unique的检查
	for (auto i = table.Table_Column.begin(); i != table.Table_Column.end(); i++)
	{
		if (i->IsUnique)
		{
			int ColumnIndex=table
		}
	}
	*/
	int Offset = Record_Manager::Instance().Insert_Into(table, R);
	if (Offset=-1)
		throw string("Insert failed: violation of the constraint 'unique'.");
	/*
		构建索引
	*/
}