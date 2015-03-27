#ifndef _API_H
#define _API_H
#include "Glob_Var.h"
#include "Record_Manager.h"
#include "Buffer.h"
#include "Catalog.h"
#include <string>
using namespace std;

/*
	负责调度各种模块.
	接受用户的输入，交由Interpreter处理，通过处理结果，调用
	各个模块的功能实现用户的需求.
*/
class API
{
	friend class Intepretor;
public:
	API();
	static API & Instance();	//实例化
	Intepretor MiniSQLIntepretor;//Intepretor对象
	Catalog * Mem_Catalog;//实例化的数据表信息
	//命令操作
	void CreateTable(string & Tablename,vector<Column_Type> & Attributes);
	Selection Select(string table_name, vector<Comparison>);//选择命令
};
static API & Instance()
{
	static API API_Instance;
	return API_Instance;
}
#endif