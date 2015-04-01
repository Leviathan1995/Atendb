#include "Catalog.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>//不加stdlib ofstream报错
using namespace std;
//析构函数
Catalog::~Catalog()
{
	WriteTable2File();//将数据表写入文件
}
//数据表检查
void Catalog::CatalogCheckTable(string & Tablename, vector<CatalogAttributes> & Attributes)
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
		//判断记录的申请长度是否合法
		if (Attributes[i].CatalogAttributes_Length<1 || Attributes[i].CatalogAttributes_Length>255)
			throw string("Attributes Error: Illegal length of the Attribute");
	}
}
//建立数据表
void Catalog::CatalogCreateTable(string & Tablename,vector<CatalogAttributes> & attributes)
{
	Table Tabletc;
	Tabletc.Table_Name = Tablename;						//数据表名字
	Tabletc.Flag = CATALOG_SPACE_USED;					//设定标志位
	Tabletc.Table_Attributesnum = attributes.size();
	short NewTableIndex = -1;							//数据表的标签索引号
	for (int i = 0; i < TableCatalog.size(); i++)		//在现有的数据中搜寻
	{
		if (!(TableCatalog[i].Flag&CATALOG_SPACE_USED))	//数据表为空
		{
			NewTableIndex = i;
			TableCatalog[i] = Tabletc;
			break;
		}
		if (NewTableIndex == -1)						//没有空数据表供其使用
		{
			TableCatalog.push_back(Tabletc);
			NewTableIndex = TableCatalog.size() - 1;	//新的数据表的索引号
		}
	}
	Attributes NewTableAttributes;
	short AttributesIndex = -1, PreviousAttributesIndex = 0;
	for (int i = 0; i < attributes.size(); i++)
	{
		NewTableAttributes.Flag = CATALOG_SPACE_USED;
		if (attributes[i].CatalogAttributes_Primary)
		{
			NewTableAttributes.Flag |= CATALOG_IS_PRIMARY_KEY;
			NewTableAttributes.Flag |= CATALOG_IS_UNIQUE;
			NewTableAttributes.Flag |= CATALOG_IS_NOT_NULL;
		}
		if (attributes[i].CatalogAttributes_Unique)
			NewTableAttributes.Flag |= CATALOG_IS_UNIQUE;
		if (attributes[i].CatalogAttributes_Null)
			NewTableAttributes.Flag |= CATALOG_IS_NOT_NULL;
		NewTableAttributes.Attributes_name = attributes[i].CatalogAttributes_Name;
		switch (attributes[i].CatalogAttributes_Type)
		{
		case Int:
			NewTableAttributes.Attributes_type =Int;
			NewTableAttributes.StoredLength = 4;
			break;
		case Char:
			NewTableAttributes.Attributes_type = Char;
			NewTableAttributes.StoredLength = attributes[i].CatalogAttributes_Length;
		case Float:
			NewTableAttributes.Attributes_type = Float;
			NewTableAttributes.StoredLength = 4;
		default:
			break; 
		}
		NewTableAttributes.NextAttributes = NULL;//下一个属性置为空
		AttributesIndex++;
		while (AttributesIndex < (int)AttributesCatalog.size() && (AttributesCatalog[AttributesIndex].Flag &CATALOG_SPACE_USED))//先寻找有没有空的记录空间
				AttributesIndex++;
		if (AttributesIndex >= (int)AttributesCatalog.size())		//索引大于属性存放数组的大小 需要开辟空间
			AttributesCatalog.push_back(NewTableAttributes);
		else
			AttributesCatalog[AttributesIndex] = NewTableAttributes;//否则把记录放入空的记录空间
		if (i == 0)													//如果当前是第一条属性，修改TableCatalog的FirstKey
			TableCatalog[NewTableIndex].Table_FirstAttributes = AttributesIndex;
		else
			AttributesCatalog[PreviousAttributesIndex].NextAttributes = AttributesIndex;// 否则修改此表上一条属性的NextAttributes;
		PreviousAttributesIndex = AttributesIndex;
		//如果这个属性是主键，修改标志和Primarykey
		if (NewTableAttributes.Flag &CATALOG_HAS_PRIMARY_KEY)
		{
			TableCatalog[NewTableIndex].Flag |= CATALOG_HAS_PRIMARY_KEY;
			TableCatalog[NewTableIndex].Table_PrimaryAttributes = i;
		}
	}
}
//插入的属性进行检查
void Catalog::CatalogCheckColumn(string & tablename, Record R)
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
void Catalog::WriteTable2File()
{
	ofstream Fout("catalog/table.dat", ios::binary);
	Fout.seekp(0, ios::beg);
	for (int i = 0; i < TableCatalog.size(); i++)
	{
		Fout.write(&TableCatalog[i].Flag, 1);														//先写入标志位
		Fout.write(TableCatalog[i].Table_Name.c_str(), TableCatalog[i].Table_Name.length());		//再写入数据表名字
		Fout.write(&TableCatalog[i].Table_Attributesnum, 1);										//数据表中属性的数量		
		Fout.write(&TableCatalog[i].Table_PrimaryAttributes, 1);									//数据中的主键
		Fout.write((char *)&TableCatalog[i].IndexFlags, sizeof(long));								//索引的标志位
		Fout.write((char *)&TableCatalog[i].Table_FirstAttributes, sizeof(short));					//第一条属性
		Fout.write((char *)&TableCatalog[i].FirstIndex, sizeof(short));								//该表第一条索引信息的编号
	}
	Fout.close();
}