#include "Select.h"
using namespace std;
//获取用户的输入
void Selection::SelectionInput(string *Sel, string *table, WhereList *& wherelist, int wherenum)
{
	int i = 0;
	while (Sel[i] != "$")
	{
		SelLists.push_back(Sel[i]);
		i++;
	}
	i = 0;
	while (table[i] != "$")
	{
		TableLists.push_back(table[i]);
		i++;
	}
	for (i = 0; i < wherenum; i++)
		WhereLists.push_back(wherelist[i]);
}
//解析Select命令
void Selection::Selection_Parse()
{
	Table_Type SelectTable;
	while (!TableLists.empty())
	{
		SelectTable = Catalog::Get_Table(TableLists.front());
		while (!SelLists.empty())
		{
			Column_Type SelectColumn;
			SelectColumn = Catalog::Get_Column(TableLists.front(), SelLists.front());
			Operator_type Select_Operator = Intepretor::Op_Judge(WhereLists.front().Where_Operator);
			switch (Select_Operator)
			{

			}
		}
	}
}
//打印Select的属性头
void Selection::Print_SelectHead()
{
	/*
	只打印Selection中的vector<string> SelLists;
	*/
}