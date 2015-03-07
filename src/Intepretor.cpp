#include "Intepretor.h"
#include "Glob_Var.h"
#include "Catalog.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;
void Intepretor::CommandInput()
{
	while (true)
	{
		string command;
		while (cin >> command)
		{
			//去掉（），;
			bool End;//用户是否输入完成
			auto start = command.begin();
			for (auto i = command.begin(); i != command.end(); i++)
			{
				switch (*i)
				{
				case'(':
				case ')':
				case ';':
					if (start != i)
						Input.push_back(string(start, i));
					start = i;
					start++;
					End = true;
				case ',':

				default:
					break;
				}
				if (End == true)
					break;
			}
			if (Is_Quit(Input))
				exit(0);//退出
			else
				ParseCommand();

		}
	}
}
void Intepretor::ParseCommand()
{
	if (Is_CreateTable(Input))
		CreateTable(Input);
}
void Intepretor::CreateTable(vector<string>Input)
{
	Command_State state = Create;
	Table_Type table;
	Column_Type column;
	for (auto i = Input.begin(); i != Input.end(); i++)
	{
		switch (state)
		{
		case Create:
			state = Table; break;
		case Table:
			state = Table_Name; break;
		case Table_Name:
			state = Left_Query;
			table.Table_Name = *i;
			break;
		case Left_Query:
			if (*i != "(")
				throw Error();
			state = Column_Name;
			break;
		case Column_Name:
			state = Column_type;
			


		default:
			break;
		}
	}
}