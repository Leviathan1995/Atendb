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
	Catalog::Instance().CatalogCheckInsertTuple(tablename, Tuple_Lists);			//对插入的记录进行检查
	Table table = Catalog::Instance().CatalogGet_Table(tablename);
	int Offset = Record_Manager::Instance().Record_ManagerInsert_Into(table, Tuple_Lists);
	if (Offset == false)	//未插入成功
		throw Error(0, "API ", "Insert_Into", "Insert failed : violation of the constraint 'unique'");
}
//Select 选择元组
void API::Select(queue<string> attributes, queue<string>tablelists, queue<WhereList> wherelists)
{
	
}
