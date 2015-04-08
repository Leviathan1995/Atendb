#include "Select.h"
using namespace std;
//解析用户的Select输入
vector<SelectRecord> & Selection::Select_Parse(queue<string> attributes, queue<string>tablelists, queue<string> wherelists)
{
	if (attributes.front() == "All") //select * from table where ...
	{
		while (tablelists.size()!=0)
		{
			string StrRecord;
			vector<string> record; //数据表中的记录动态数组
			Table table = Catalog::Instance().CatalogGet_Table(TableLists.front());		//得到数据表
			int table_blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, StrRecord);	//读取数据表的块号
			Buffer_Manager::Instance().Buffer_ManagerRead(table.Table_Name, table_blocknum, StrRecord);				//从缓冲区读取记录
			record=Record_Manager::Instance().Record_ManagerString2Tuple(table.Table_AttributesList, StrRecord);				//将字符串分割为元组,返回vector<stirng>
			if (wherelists.empty()) //select * from table .where部分为空
			{
				int table_attributes_size = table.Table_AttributesList.size(); //数据表属性的个数
				int i = 0;
				while (i<table_attributes_size)
				{
					SelectRecord selectrecord; //选择的一列记录
					selectrecord.AttributesName = table.Table_AttributesList[i].Attributes_Name; //获得数据表的属性名
					CatalogTable catalogtable = Catalog::Instance().CatalogGet_CatalogTable(tablelists.front());//得到数据表的模式信息
					int Tableattributesnum = catalogtable.CatalogTable_AttribtuesNum;//得到属性的数目
					for (int )
				}
			}
		}
	}
}