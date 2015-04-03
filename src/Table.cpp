#include "Table.h"
size_t Table::Table_Length()
{
	size_t Length = 0;
	for (auto i = Table_AttributesList.begin(); i != Table_AttributesList.end(); i++)
		Length += i->Attributes_Length;
	return Length;
}