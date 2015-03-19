#include "Catalog.h"
#include "Error.h"
void Catalog::CatalogCreateTable(string & Tablename, vector<Column_Type> & Attributes)
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
}
void Table_Type::InsertColumn(Column_Type column)
{
	for (auto i = Table_Column.begin(); i != Table_Column.end(); i++)
	{
		if (i->Column_TypeName == column.Column_TypeName)
			throw Error();//抛出错误，即属性已经存在
	}
	Table_Column.push_back(column);
}
Table_Type & Catalog::Get_Table(string table_name)
{
	if (Mem_Table.find(table_name) == Mem_Table.end())
		throw Error();
	return Mem_Table[table_name];
}

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