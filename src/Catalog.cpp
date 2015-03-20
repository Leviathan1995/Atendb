#include "Catalog.h"
#include "Error.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>//不加stdlib ofstream报错
#include "Intepretor.h"
using namespace std;
//建立数据表
void Catalog::CatalogCreateTable(string & Tablename,const vector<Column_Type> & Attributes)
{
	Table_Type Tabletc;
	Tabletc.Table_Name = Tablename;
	Tabletc.Flag = CATALOG_SPACE_USED;
	Tabletc.ColumnNum = Attributes.size();
	short NewTableIndex = -1;//数据表的标签索引号
	for (size_t i = 0; i < TableCatalog.size(); i++)
	{
		if (!(TableCatalog[i].Flag&CATALOG_SPACE_USED))
		{
			NewTableIndex = i;
			TableCatalog[i] = Tabletc;
			break;
		}
		if (NewTableIndex == -1)
		{
			TableCatalog.push_back(Tabletc);
			NewTableIndex = TableCatalog.size() - 1;
		}
	}
	Column_Type Kc;
	short KeyIndex = -1, PreviousKeyIndex = 0;
	for (size_t i = 0; i < Attributes.size(); i++)
	{
		Kc.Flag = CATALOG_SPACE_USED;
		if (Attributes[i].IsPrimary)
		{
			Kc.Flag |= CATALOG_IS_PRIMARY_KEY;
			Kc.Flag |= CATALOG_IS_UNIQUE;
			Kc.Flag |= CATALOG_IS_NOT_NULL;
		}
		if (Attributes[i].IsUnique)
			Kc.Flag |= CATALOG_IS_UNIQUE;
		if (Attributes[i].IsNotNull)
			Kc.Flag |= CATALOG_IS_NOT_NULL;
		Kc.Column_TypeName = Attributes[i].Column_TypeName;
		switch (Attributes[i].coltype)
		{
		case Int:
			Kc.coltype = 0;
		default:
			break; 
		}
	}
}
//插入元组
void Table_Type::InsertColumn(Column_Type column)
{
	for (auto i = Table_Column.begin(); i != Table_Column.end(); i++)
	{
		if (i->Column_TypeName == column.Column_TypeName)
			throw Error();//抛出错误，即属性已经存在
	}
	Table_Column.push_back(column);
}
//得到数据表
Table_Type & Catalog::Get_Table(string table_name)
{
	if (Mem_Table.find(table_name) == Mem_Table.end())
		throw Error();
	return Mem_Table[table_name];
}
//得到属性
static Column_Type & Get_Column(string tablename,string columnname)
{
	Table_Type TableInstance;
	TableInstance=Catalog::Get_Table(columnname);
	for (auto i = TableInstance.Table_Column.begin(); i != TableInstance.Table_Column.end(); i++)
	{
		if (i->Column_TypeName == columnname)
			return *i;
	}
}
//把数据写入文件
void Catalog::SaveTable2File()
{
	ofstream Fout("catalog/table.dat", ios::binary);
	Fout.seekp(0, ios::beg);
	for (size_t i = 0; i < TableCatalog.size(); i++)
	{
		Fout.write(&TableCatalog[i].Flag, 1);
		Fout.write(TableCatalog[i].Table_Name.c_str(), TableCatalog[i].Table_Name.length());
		Fout.write(&TableCatalog[i].NumberKeys, 1);
		Fout.write(&TableCatalog[i].PrimaryKey, 1);
		Fout.write((char *)&TableCatalog[i].indexFlags, sizeof(long));
		Fout.write((char *)&TableCatalog[i].firstKey, sizeof(short));
		Fout.write((char *)&TableCatalog[i].firstIndex, sizeof(short));
	}
	Fout.close();
}