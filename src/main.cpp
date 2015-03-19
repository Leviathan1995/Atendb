#include "MiniSQL.h"
#include "API.h"
#include "Buffer.h"
#include <string>
#include  <iostream>
using namespace std;
int main()
{
	API MiniSQLApi;
	Buffer_Manager & Instance();//缓冲管理器实例化
	MiniSQLApi.MiniSQLIntepretor.CommandInput();//接受用户输入的命令
	MiniSQLApi.MiniSQLIntepretor.ParseCommand();//进行命令解析
	return 0;
}