#include "Catalog.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>//不加stdlib ofstream报错
using namespace std;
//析构函数
Catalog::~Catalog()
{
	SaveTable2File();//将数据表写入文件
}
//建立数据表
void Catalog::CatalogCreateTable(string & Tablename,vector<Column_Type> & Attributes)
{
	Table_Type Tabletc;
	Tabletc.Table_Name = Tablename;//数据表名字
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
			Kc.StoredLength = 4;
			break;
		case Char:
			Kc.coltype = 1;
			Kc.StoredLength = Attributes[i].RequestSize;
		case Float:
			Kc.coltype = 2;
			Kc.StoredLength = 4;
		default://默认看作是长度为255的字符串
			Kc.coltype = 1;
			Kc.StoredLength = 255;
			break; 
		}
		Kc.Next = NULL;//下一个属性先置为空
		KeyIndex++;
		while (KeyIndex < (int)ColumnCatalog.size() && (ColumnCatalog[KeyIndex].Flag &CATALOG_SPACE_USED))
			KeyIndex++;
		if (KeyIndex >= (int)ColumnCatalog.size())
			//索引大于属性存放数组的大小 需要开辟空间
			ColumnCatalog.push_back(Kc);
		else
			ColumnCatalog[KeyIndex] = Kc;
		if (i == 0)
			//如果当前是第一条属性，修改TableCatalog的FirstKey
			TableCatalog[NewTableIndex].FirstColumn = KeyIndex;
		else
			ColumnCatalog[PreviousKeyIndex].NextColumn = KeyIndex;
		PreviousKeyIndex = KeyIndex;
		//如果这个属性是主键，修改标志和Primarykey
		if (Kc.Flag &CATALOG_HAS_PRIMARY_KEY)
		{
			TableCatalog[NewTableIndex].Flag |= CATALOG_HAS_PRIMARY_KEY;
			TableCatalog[NewTableIndex].PrimaryColumn = i;
		}
	}
}
//数据表检查
void Catalog::CheckTable(string & Tablename, vector<Column_Type> & Attributes)
{
	for (size_t i = 0; i < TableCatalog.size(); i++)
	{
		if ((TableCatalog[i].Flag & CATALOG_SPACE_USED) && !strcmp(TableCatalog[i].Table_Name.c_str(), Tablename.c_str()))
			cout << "Table alredy Exits";
		break;
	}
	//判断属性的数量是否合法
	if (Attributes.size()<1 || Attributes.size()>32)
		throw string("Atttibutes Error:Too few or too many ");
	for (size_t i = 0; i < Attributes.size(); i++)
	{
		//判断记录的长度是否合法
		if (Attributes[i].StoredLength<1 || Attributes[i].StoredLength>255)
			throw string("Attributes Error: Illegal length of the Attribute");
	}
}
//插入的属性进行检查
void Catalog::CheckColumn(string & tablename, Record R)
{
	bool TableFind;//插入记录的数据表是否存在
	short NumberOfkeys = 0, FirstKeyindex = 0;
	for (size_t i = 0; i < TableCatalog.size(); i++)
	{
		if ((TableCatalog[i].Flag&CATALOG_SPACE_USED) && !strcmp(Intepretor::String2Char(TableCatalog[i].Table_Name), tablename.c_str()))
		{
			TableFind = true;
			NumberOfkeys = TableCatalog[i].NumberColumns;//数据表中的属性的数量
			FirstKeyindex = TableCatalog[i].FirstIndex;
			break;
		}
	}
	if (!TableFind)
		throw Error(1001, "Catalog", "Check_Column", "数据表不存在");
	if (R.Mem_Element.size() > NumberOfkeys)//插入的元素数量大于数据表中已规定的
		throw string("Insert into ValueIllegal: Illegal number of columns");
	short CurrentKeyindex = FirstKeyindex;
	for (size_t i = 0; i < R.Mem_Element.size(); i++)
	{
		switch (ColumnCatalog[i].coltype)
		{
		case 0:
			break;
		case 1:
			if ((int)R.Mem_Element[i].Mem_CharNum>ColumnCatalog[CurrentKeyindex].RequestSize)
				throw string("ValueIllegal: The value to insert is longer than the length of the key.");
			break;
		case 2:
			break;
		default:
			break;
		}
		CurrentKeyindex = ColumnCatalog[CurrentKeyindex].NextColumn;
	}
}
//插入元组
void Catalog::CatalogInsertColumn(string tablename,Record R)
{
	
}
//得到数据表
Table_Type & Catalog::Get_Table(string table_name)
{
	if (Mem_Table.find(table_name) == Mem_Table.end())
		throw Error(1001, "Catalog", "Insert Column", "语法错误!");
	return Mem_Table[table_name];
}
//得到属性
Column_Type & Catalog::Get_Column(string tablename,string columnname)
{
	Table_Type TableInstance;
	TableInstance=Catalog::Get_Table(columnname);
	for (auto i = TableInstance.Table_Column.begin(); i != TableInstance.Table_Column.end(); i++)
	{
		if (i->Column_TypeName == columnname)
			return *i;
	}
}
//获取字段的长度
size_t Catalog::Table_Size(string &tablename)
{
	size_t length = 0;
	Table_Type t = Get_Table(tablename);
	for (auto Len = t.Table_Column.begin(); Len != t.Table_Column.end(); Len++)
	{
		length += Len->StoredLength;
	}
	return length;
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
		Fout.write(&TableCatalog[i].NumberColumns, 1);
		Fout.write(&TableCatalog[i].PrimaryColumn, 1);
		Fout.write((char *)&TableCatalog[i].IndexFlags, sizeof(long));
		Fout.write((char *)&TableCatalog[i].FirstColumn, sizeof(short));
		Fout.write((char *)&TableCatalog[i].FirstIndex, sizeof(short));
	}
	Fout.close();
}