#include "MiniSQL.h"
#include "API.h"
#include "Buffer.h"
#include "Catalog.h"
#include <string>
#include  <iostream>
using namespace std;
int main()
{
	Intepretor MiniSQLIntepretor;
	cout << "----------Welcome to MiniSQL----------";
	cout << "|                                    |";
	cout << "|                                    |";
	cout << "|                                    |";
	cout << "|			by:Leviathan1995		  |";
	cout << "--------------------------------------";
	MiniSQLIntepretor.CommandInput();//接受用户输入的命令
	MiniSQLIntepretor.ParseCommand();//进行命令解析
	return 0;
}