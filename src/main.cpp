#include "MiniSQL.h"
#include "API.h"
#include "Buffer.h"
#include "Catalog.h"
#include <string>
#include  <iostream>
using namespace std;
int main()
{
	API MiniSQLApi;
	Catalog CatalogManagent;//模式信息管理的对象
	MiniSQLApi.MiniSQLIntepretor.CommandInput();//接受用户输入的命令
	MiniSQLApi.MiniSQLIntepretor.ParseCommand();//进行命令解析
	return 0;
}