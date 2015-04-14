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