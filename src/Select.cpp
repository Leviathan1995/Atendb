#include "Select.h"
using namespace std;
//解析用户的Select输入
vector<SelectRecord> & Selection::Select_Parse(queue<string> attributes, queue<string>tablelists, queue<WhereList> wherelists)
{
	if (attributes.front() == "All") //select * from table where ...
	{
		while (tablelists.size() != 0)
		{
			Table table = Catalog::Instance().CatalogGet_Table(TableLists.front());		//得到数据表
			if (wherelists.empty()) //select * from table .where部分为空
			{
				int table_attributes_size = table.Table_AttributesList.size();
				while (table_attributes_size--)
				{
					SelectRecord selectrecord; //选择的一列记录
					selectrecord.AttributesName = table.Table_AttributesList[table_attributes_size].Attributes_Name; //获得数据表的属性名
					string record;				//单个记录值
					int table_blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, record);	//读取块号
					Buffer_Manager::Instance().Buffer_ManagerRead(table.Table_Name, table_blocknum, record);				//从缓冲区读取记录
				}
			}
		}
	}
}