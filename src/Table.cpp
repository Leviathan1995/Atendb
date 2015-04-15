#include "Table.h"
//返回数据表模式信息长度 
size_t Table::Table_Length()
{
	size_t Length = 0;
	for (auto i = Table_AttributesList.begin(); i != Table_AttributesList.end(); i++)
		Length += i->Attributes_Length;
	return Length;
}
//得到数据表中的指定属性
Attributes Table::GetAttributes(string & attribtuesname)
{
	for (auto i = Table_AttributesList.begin(); i != Table_AttributesList.end(); i++)
	{
		if (i->Attributes_Name == attribtuesname)
			return *i;
	}
}
//得到这个属性在模式信息表的中起点偏移量
size_t Table::GetAttributesBegin(string &attributesname)
{
	size_t Begin = 0;
	for (auto i = Table_AttributesList.begin(); i != Table_AttributesList.end(); i++)
	{
		if (i->Attributes_Name == attributesname)
			return Begin;
		Begin += i->Attributes_Length;
	}
	return 0;
}
//得到这个属性在模式信息表的中终点偏移量
size_t Table::GetAttributesEnd(string &attributesname)
{
	size_t End = 0;
	for (auto i = Table_AttributesList.begin(); i != Table_AttributesList.end(); i++)
	{
		if (i->Attributes_Name == attributesname)
			return End+=i->Attributes_Length;
		End+= i->Attributes_Length;
	}
	return 0;
}