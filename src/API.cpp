#include "API.h"
using namespace std;
API::API()
{

}
//Create建表
void API::CreateTable(string & tablename, vector<Attributes> &Attributes)
{
	try
	{
		Catalog::Instance().CatalogCreateTable(tablename, Attributes);//直接传给模式信息类创建表
	}
	catch (string errsrt)
	{
		throw;
	}
	Record_Manager::Instance().Record_ManagerCreateTable(tablename, Attributes);//传给磁盘管理系统创建表
}
//Insert into 插入记录
void API::Insert_Into(string & tablename, vector<Tuple> Tuple_Lists)
{
	Catalog::Instance().CatalogCheckTuple(tablename, Tuple_Lists);			//对插入的记录进行检查
	Table table = Catalog::Instance().CatalogGet_Table(tablename);
	int Offset = Record_Manager::Instance().
}

//Slect命令
void API::Select(string table_name, vector<Comparison>)
{
	Table_Type table_select = Catalog::Instance().Get_Table(table_name);
}
