#include "Catalog.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>//不加stdlib ofstream报错
using namespace std;
//析构函数
Catalog::~Catalog()
{
	WriteTable2File();//将数据表写入文件
	WriteAttributes2File();//将数据属性写入文件
}
//把数据表写入文件
void Catalog::WriteTable2File()
{
	ofstream Fout("catalog/table.dat", ios::binary);
	Fout.seekp(0, ios::beg);
	for (int i = 0; i < TableCatalog.size(); i++)
	{
		Fout.write(&TableCatalog[i].CatalogTable_Flag, 1);														//先写入标志位
		Fout.write(TableCatalog[i].CatalogTable_Name.c_str(), TableCatalog[i].CatalogTable_Name.length());		//再写入数据表名字
		Fout.write(&TableCatalog[i].CatalogTable_AttribtuesNum, 1);												//数据表中属性的数量		
		Fout.write(&TableCatalog[i].CatalogTable_PrimaryAttributes, 1);											//数据中的主键
		Fout.write((char *)&TableCatalog[i].CatalogTable_IndexFlag, sizeof(long));								//索引的标志位
		Fout.write((char *)&TableCatalog[i].CatalogTable_FirstAttributesIndex, sizeof(short));					//第一条属性在"attritbutes.dat"中的下标
		Fout.write((char *)&TableCatalog[i].CatablogTable_FirstIndex, sizeof(short));							//该表第一条索引信息在"index.dat"编号
	}
	Fout.close();
}
//把数据属写入文件
void Catalog::WriteAttributes2File()
{
	ofstream Fout("catalog/attributes.dat", ios::binary);
	Fout.seekp(0, ios::beg);
	for (size_t i = 0; i < AttributesCatalog.size(); i++)
	{

		Fout.write(&AttributesCatalog[i].CatalogAttributes_Flag, 1);		//属性的标志位
		Fout.write(AttributesCatalog[i].CatalogAttributes_Name.c_str(), AttributesCatalog[i].CatalogAttributes_Name.length());//属性的名字
		Fout.write(&AttributesCatalog[i].CatalogAttributes_Type, 1);		//属性的类型
		Fout.write((char *)&AttributesCatalog[i].CatalogAttributes_Length, 1);	//属性的长度
		Fout.write((char *)&AttributesCatalog[i].CatalogAttributes_NextAttributes, sizeof(short));		//下一条属性在该表的下标
	}
}
//数据表检查
void Catalog::CatalogCheckCreateTable(string & Tablename, vector<Attributes> & Attributes)
{
	for (size_t i = 0; i < TableCatalog.size(); i++)
	{
		if ((TableCatalog[i].CatalogTable_Flag & CATALOG_SPACE_USED) && !strcmp(TableCatalog[i].CatalogTable_Name.c_str(), Tablename.c_str()))
			cout << "Table alredy Exits";
		break;
	}
	//判断属性的数量是否合法
	if (Attributes.size()<1 || Attributes.size()>32)
		throw string("Atttibutes Error:Too few or too many ");
	for (size_t i = 0; i < Attributes.size(); i++)
	{
		//判断记录的申请长度是否合法
		if (Attributes[i].Attributes_Length<1 || Attributes[i].Attributes_Length>255)
			throw string("Attributes Error: Illegal length of the Attribute");
	}
}
//建立数据表
void Catalog::CatalogCreateTable(string & Tablename,vector<Attributes> & attributes)
{
	CatalogTable Tabletc;
	Tabletc.CatalogTable_Name = Tablename;						//数据表名字
	Tabletc.CatalogTable_Flag = CATALOG_SPACE_USED;				//设定标志位
	Tabletc.CatalogTable_AttribtuesNum = attributes.size();		//属性的数量
	short NewTableIndex = -1;									//数据表的索引号
	for (int i = 0; i < TableCatalog.size(); i++)				//在现有的数据中搜寻
	{
		if (!(TableCatalog[i].CatalogTable_Flag&CATALOG_SPACE_USED))	//数据表为空
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
	CatalogAttributes NewTableAttributes;
	short AttributesIndex = -1, PreviousAttributesIndex = 0;
	for (int i = 0; i < attributes.size(); i++)
	{
		NewTableAttributes.CatalogAttributes_Flag = CATALOG_SPACE_USED;
		if (attributes[i].Attributes_Primary())
		{
			NewTableAttributes.CatalogAttributes_Flag|= CATALOG_IS_PRIMARY_KEY;
			NewTableAttributes.CatalogAttributes_Flag |= CATALOG_IS_UNIQUE;
			NewTableAttributes.CatalogAttributes_Flag |= CATALOG_IS_NOT_NULL;
		}
		if (attributes[i].Attributes_Unique())
			NewTableAttributes.CatalogAttributes_Flag|= CATALOG_IS_UNIQUE;
		if (attributes[i].Attributes_Null())
			NewTableAttributes.CatalogAttributes_Flag|= CATALOG_IS_NOT_NULL;
		NewTableAttributes.CatalogAttributes_Name = attributes[i].Attributes_Name;
		switch (attributes[i].Attributes_type)
		{
		case Int:
			NewTableAttributes.CatalogAttributes_Type=Int;
			NewTableAttributes.CatalogAttributes_Length = 4;
			break;
		case Char:
			NewTableAttributes.CatalogAttributes_Type = Char;
			NewTableAttributes.CatalogAttributes_Length = attributes[i].Attributes_Length;
		case Float:
			NewTableAttributes.CatalogAttributes_Type = Float;
			NewTableAttributes.CatalogAttributes_Length = 4;
		default:
			break; 
		}
		NewTableAttributes.CatalogAttributes_NextAttributes = NULL;//下一个属性置为空
		AttributesIndex++;
		while (AttributesIndex < (int)AttributesCatalog.size() && (AttributesCatalog[AttributesIndex].CatalogAttributes_Flag &CATALOG_SPACE_USED))//先寻找有没有空的记录空间
				AttributesIndex++;
		if (AttributesIndex >= (int)AttributesCatalog.size())		//索引大于属性存放数组的大小 需要开辟空间
			AttributesCatalog.push_back(NewTableAttributes);
		else
			AttributesCatalog[AttributesIndex] = NewTableAttributes;//否则把记录放入空的记录空间
		if (i == 0)													//如果当前是第一条属性，修改TableCatalog的FirstKey
			TableCatalog[NewTableIndex].CatablogTable_FirstIndex = AttributesIndex;
		else
			AttributesCatalog[PreviousAttributesIndex].CatalogAttributes_NextAttributes = AttributesIndex;// 否则修改此表上一条属性的NextAttributes;
		PreviousAttributesIndex = AttributesIndex;
		//如果这个属性是主键，修改标志和Primarykey
		if (NewTableAttributes.CatalogAttributes_Flag &CATALOG_HAS_PRIMARY_KEY)
		{
			TableCatalog[NewTableIndex].CatalogTable_Flag |= CATALOG_HAS_PRIMARY_KEY;
			TableCatalog[NewTableIndex].CatalogTable_PrimaryAttributes= i;
		}
	}
}
//得到数据表的模式信息
CatalogTable &Catalog::CatalogGet_CatalogTable(string tablename)
{
	for (auto i = TableCatalog.begin(); i != TableCatalog.end(); i++)
	{
		if (i->CatalogTable_Name == tablename)
			return *i;
	}
	
}
//得到数据表
Table & Catalog::CatalogGet_Table(string tablename)
{
	Table * table = new Table;
	table->Table_Name = tablename;
	//查找表
	short FirstAttribtuesIndex = 0;
	unsigned long IndexFlag = 0;
	bool Find = false;
	for (int i = 0; i < TableCatalog.size(); i++)
	{
		if (TableCatalog[i].CatalogTable_Flag&CATALOG_SPACE_USED&&!strcmp(Intepretor::String2Char(TableCatalog[i].CatalogTable_Name), tablename.c_str()))
		{
			Find = true;
			IndexFlag = TableCatalog[i].CatalogTable_IndexFlag;							//获得该表的索引标志
			FirstAttribtuesIndex = TableCatalog[i].CatalogTable_FirstAttributesIndex;	//第一条属性的索引
		}
	}
	if (!Find)
		throw Error(0, "Interpreter", "Insert into", "语法错误!");
	//获得该表的所有属性
	short CurrentAttributesIndex = FirstAttribtuesIndex;
	while (CurrentAttributesIndex != -1)
	{
		//获得单个属性的信息
		Attributes attritbues;
		attritbues.Attributes_Name = AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Name;
		switch (AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Type)
		{
		case Int:
			attritbues.Attributes_type = Int;
			break;
		case Char:
			attritbues.Attributes_type = Char;
			break;
		case Float:
			attritbues.Attributes_type = Float;
			break;
		default:
			break;
		}
		attritbues.Attributes_Length = AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Length;
		attritbues.Attributes_Flag = 0;
		attritbues.Attributes_Flag |= (AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Flag&	CATALOG_IS_PRIMARY_KEY) ? 1 : 0;
		attritbues.Attributes_Flag |= (AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Flag&	CATALOG_IS_UNIQUE) ? 2 : 0;
		attritbues.Attributes_Flag |= (AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Flag& CATALOG_IS_NOT_NULL) ? 4 : 0;
		attritbues.Attributes_Flag |= (AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Flag& CATALOG_IS_INDEX) ? 8 : 0;
		table->Table_AttributesList.push_back(attritbues);
		CurrentAttributesIndex = AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_NextAttributes;
	}
	return *table;
}
//得到 数据表中的属性的列号，也就是第几个属性,如果没有这个属性就返回-1
int Catalog::Catalog_GetAttributesNumber(string & attributesname, Table & table)
{
	int Number = 0;
	for (auto i = table.Table_AttributesList.begin(); i != table.Table_AttributesList.end(); i++)
	{
		if (i->Attributes_Name == attributesname)
		{
			Number++;
			return Number;
		}
		Number++;
	}
	return -1;
}
//插入的记录进行检查
void Catalog::CatalogCheckInsertTuple(string & tablename, vector<Tuple> Tuple_Lists)
{
	Table table = Catalog::CatalogGet_Table(tablename);
	bool TableFind;//插入记录的数据表是否存在
	short NumberOfAttributes = 0, FirstAttributexindex = 0;
	for (int i = 0; i < TableCatalog.size(); i++)
	{
		if ((TableCatalog[i].CatalogTable_Flag&CATALOG_SPACE_USED) && !strcmp(Intepretor::String2Char(TableCatalog[i].CatalogTable_Name), tablename.c_str()))
		{
			TableFind = true;
			NumberOfAttributes = TableCatalog[i].CatalogTable_AttribtuesNum;			//数据表中的属性的数量
			FirstAttributexindex = TableCatalog[i].CatalogTable_FirstAttributesIndex;	//第一条属性的索引
			break;
		}
	}
	if (!TableFind)
		throw Error(1001, "Catalog", "Check_Attributes", "数据表不存在");
	short CurrentKeyindex =FirstAttributexindex;
	for (int  i = 0; i < Tuple_Lists.size(); i++)
	{
		Tuple tuple = Tuple_Lists[i];
		for (int j = 0; j < tuple.Tuple_content.size();j++)
		switch (table.Table_AttributesList[j].Attributes_type)
		{
		case Int:
			break;
		case Char:
			if (tuple.Tuple_content[j].length()>table.Table_AttributesList[j].Attributes_Length)	//大于用户申请的长度
				throw string("ValueIllegal: The value to insert is longer than the length of the key.");
			break;
		case Float:
			break;
		default:
			break;
		}
	}
}
//对Select 的元组和属性进行检查
void Catalog::CatalogCheckSelectTuple(queue<string> attributes, queue<string>tablelists)
{
	bool Find = false;
	int size = tablelists.size();
	for (int j = 0; j < size; j++)
	{
		string tablename = tablelists.front();
		for (auto i = TableCatalog.begin(); i != TableCatalog.end(); i++)
		{
			if (tablename == i->CatalogTable_Name)
			{
				break;
				Find = true;
			}
		}
		if (!Find)
			throw Error(1001, "Catalog", "Select tuple", "No such table");
	}

	Find = false;
	size = attributes.size();
	for (int j = 0; j < size; j++)
	{
		string tablename = attributes.front();
		for (auto i = AttributesCatalog.begin(); i != AttributesCatalog.end(); i++)
		{
			if (tablename == i->CatalogAttributes_Name)
			{
				break;
				Find = true;
			}
		}
		if (!Find)
			throw Error(1001, "Catalog", "Select tuple", "No such Attribtues");
	}

}
//检查用户准备建立的索引
void Catalog::CatalogCheckCreateIndex(string &indexname, string &tablename, string &attributesname)
{
	short FirstAttributesIndex = 0, FirstIndexIndex = 0;
	unsigned long IndexFlags = 0;
	bool TableFind = false, HasIndex = false;
	for (size_t i = 0; i < TableCatalog.size(); i++)
	{
		if ((TableCatalog[i].CatalogTable_Flag & CATALOG_SPACE_USED) && !strcmp(Intepretor::String2Char(TableCatalog[i].CatalogTable_Name), tablename.c_str()))
		{
			TableFind = true;
			//获得该表的索引标志位
			IndexFlags = TableCatalog[i].CatalogTable_IndexFlag;
			//获得该表第一条属性，索引（如果有）的位置
			FirstAttributesIndex = TableCatalog[i].CatalogTable_FirstAttributesIndex;
			if (TableCatalog[i].CatalogTable_Flag &CATALOG_HAS_INDEX)
			{
				HasIndex = true;
				FirstIndexIndex = TableCatalog[i].CatablogTable_FirstIndex;
			}
			break;
		}
	}
	if (!TableFind)
		throw Error(0, "Catalog", "Create index", "no such table");
	//检查属性是否存在，属性是否是Unique
	bool AttributesFind = false;
	short CurrentAttributesIndex = FirstAttributesIndex, AttribtuesNumber = -1;
	while (CurrentAttributesIndex != -1 && !TableFind)
	{
		AttribtuesNumber++;
		if (!strcmp(Intepretor::String2Char(AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Name), attributesname.c_str()))
		{
			AttributesFind = true;
			if (!(AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_Flag &CATALOG_IS_UNIQUE))
				throw Error(0, "Catalog", "Create index", "Attribtues is not unique");
			break;
		}
		CurrentAttributesIndex = AttributesCatalog[CurrentAttributesIndex].CatalogAttributes_NextAttributes;
	}
	if (!AttributesFind)
		throw Error(0, "Catalog", "Create index", "no such attribtues");
	//检查索引是否冲突
	if ((IndexFlags >> AttribtuesNumber) & 1)
		throw Error(0, "Catalog", "Create index", "The index is already exist");
	//检查indexname
	for (size_t i = 0; i < IndexCatalog.size(); i++)
	{
		if ((IndexCatalog[i].CatalogIndex_Flag & CATALOG_SPACE_USED) && !strcmp(Intepretor::String2Char(IndexCatalog[i].CatalogIndex_Name), indexname.c_str()))
		{
			throw Error(0, "Catalog", "Create index", "An index of the same name already exists.");
		}
	}
}
