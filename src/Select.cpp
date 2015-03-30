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
	string WhereAttributesName;//where中选择属性的名字
	Column_Type WhereAttributes;// where中选择属性
	vector<Record> SelectRecord;//Select判断后的记录
	Record WhereRecord;//select命令where部分经选择后的记录
	while (!TableLists.empty())//TableLists为用户选择的数据表集合，即From
	{
		SelectTable = Catalog::Get_Table(TableLists.front());//获取用户选择的数据表
		while (!SelLists.empty())//SelLists为用户选择的属性集合
		{
			Column_Type SelectColumn;
			SelectColumn = Catalog::Get_Column(TableLists.front(), SelLists.front());//用户选择的属性
			Operator_type Select_Operator = Intepretor::Op_Judge(WhereLists.front().Where_Operator);//取出where中运算符
			switch (Select_Operator)
			{
			case EQUAL:
				WhereAttributesName = WhereLists.front().Attribute;//where中选择属性的名字
				WhereAttributes = Catalog::Get_Column(SelectTable.Table_Name, WhereAttributesName);// where中选择属性
			case LESS_THAN:
			case MORE_THAN://>
			case MORE_AND_EQUAL://>=
			case LESS_AND_EQUAL://<=
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