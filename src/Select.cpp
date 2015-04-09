#include "Select.h"
using namespace std;
//解析用户的Select输入
vector<SelectRecord> & Selection::Select_Parse(queue<string> attributes, queue<string>tablelists, queue<string> wherelists)
{

	if (attributes.front() == "All") //select * from table where ...
	{
		while (tablelists.size() != 0)
		{
			string StrRecord;
			vector<string> record; //数据表中的记录vector数组
			Table table = Catalog::Instance().CatalogGet_Table(TableLists.front());		//得到数据表
			int table_blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, StrRecord);	//读取数据表的块号
			Buffer_Manager::Instance().Buffer_ManagerRead(table.Table_Name, table_blocknum, StrRecord);					//从缓冲区读取记录
			record = Record_Manager::Instance().Record_ManagerString2Tuple(table.Table_AttributesList, StrRecord);		//将字符串分割为元组,返回vector<stirng>
			vector<Tuple> Tuple_Record = Record_Manager::Instance().Record_Manager2Tuple(record, table);				//获得vector<tuple>
			if (wherelists.empty()) //select * from table .where部分为空
			{
				int table_attributes_size = table.Table_AttributesList.size(); //数据表属性的个数
				int i = 0;
				while (i < table_attributes_size)
				{
					SelectRecord selectrecord; //选择的一列记录
					selectrecord.AttributesName = table.Table_AttributesList[i].Attributes_Name; //获得数据表的属性名
					CatalogTable catalogtable = Catalog::Instance().CatalogGet_CatalogTable(tablelists.front());//得到数据表的模式信息
					int Tableattributesnum = catalogtable.CatalogTable_AttribtuesNum;//得到属性的数目
					for (size_t k = 0; k < record.size(); k++)
					{
						selectrecord.selectrecords.push_back(record[i + k]); //获取第i个属性的记录列
						k += Tableattributesnum;
					}
					SelectRecords.push_back(selectrecord); //收集第i个属性的记录列
				}
			}
			else
			{
				int wherelistsNum = 0;
				for (int i = 0; i < wherelists.size();i++)
				{
					SelectRecord selectrecord; //选择的一列记录
					string WhereAttributes = wherelists.front();			//where 后的属性
					wherelists.pop();
					wherelists.push(WhereAttributes);
					Operator_type Op = Intepretor::Op_Judge(wherelists.front());//where中的属性后运算符
					wherelists.push(wherelists.front());
					wherelists.pop();
					string WhereValues = wherelists.front();					//where中的运算符后的值
					wherelists.pop();
					wherelists.push(WhereValues);
					i += 3;
					int WhereAttributesNumber = Catalog::Instance().Catalog_GetAttributesNumber(WhereAttributes, table); //获取属性的序列值，即第几个属性
					if (WhereAttributesNumber != -1) //数据表中有这个属性
					{
						int Tuple_Num = Tuple_Record.size();//记录的条数
						int Sign = 0;
						CatalogTable catalogtable = Catalog::Instance().CatalogGet_CatalogTable(tablelists.front());//得到数据表的模式信息
						int Tableattributesnum = catalogtable.CatalogTable_AttribtuesNum;//得到属性的数目
						for (int i = 0; i < Tableattributesnum; i++)
						{
							while (Sign < Tuple_Num)
							{
								selectrecord.AttributesName = table.Table_AttributesList[i].Attributes_Name;
								switch (Op)
								{
								case LESS_THAN:
									if (Intepretor::String2Int(Tuple_Record[Sign].Tuple_content[WhereAttributesNumber]) < Intepretor::String2Int(WhereValues))
										selectrecord.selectrecords.push_back(Tuple_Record[Sign].Tuple_content[i]);
									Sign++;
									break;
								case EQUAL:
									if (Intepretor::String2Int(Tuple_Record[Sign].Tuple_content[WhereAttributesNumber]) == Intepretor::String2Int(WhereValues))
										selectrecord.selectrecords.push_back(Tuple_Record[Sign].Tuple_content[i]);								
									Sign++;
									break;
								case MORE_THAN:
									if (Intepretor::String2Int(Tuple_Record[Sign].Tuple_content[WhereAttributesNumber]) > Intepretor::String2Int(WhereValues))
										selectrecord.selectrecords.push_back(Tuple_Record[Sign].Tuple_content[i]);
									Sign++;
									break;
								case MORE_AND_EQUAL:
									if (Intepretor::String2Int(Tuple_Record[Sign].Tuple_content[WhereAttributesNumber]) <= Intepretor::String2Int(WhereValues))
										selectrecord.selectrecords.push_back(Tuple_Record[Sign].Tuple_content[i]);
									
									Sign++;
									break;
								case LESS_AND_EQUAL:
									if (Intepretor::String2Int(Tuple_Record[Sign].Tuple_content[WhereAttributesNumber]) >= Intepretor::String2Int(WhereValues))
										selectrecord.selectrecords.push_back(Tuple_Record[Sign].Tuple_content[i]);									
									Sign++;
									break;
								default:
									break;
								}
							}
							SelectRecords.push_back(selectrecord);
						}
					}
					WhereAttributesNumber++;
				}
			}
			tablelists.pop();
		}
	}
	else //非 select * form...
	{
		int SelectAttributesnumber = attributes.size(); //Select属性的个数
		int SelectTablenumber = tablelists.size();  //Select数据表的个数
		int TableNum = 0;
		int AttributesNum = 0;
		string StrRecord;
		vector<string> record; //数据表中的记录vector数组
		Table table = Catalog::Instance().CatalogGet_Table(TableLists.front());		//得到数据表
		int table_blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, StrRecord);	//读取数据表的块号
		Buffer_Manager::Instance().Buffer_ManagerRead(table.Table_Name, table_blocknum, StrRecord);					//从缓冲区读取记录
		record = Record_Manager::Instance().Record_ManagerString2Tuple(table.Table_AttributesList, StrRecord);		//将字符串分割为元组,返回vector<stirng>
		vector<Tuple> Tuple_Record = Record_Manager::Instance().Record_Manager2Tuple(record, table);				//获得vector<tuple>
		while (TableNum<SelectTablenumber)
		{
			Table table = Catalog::Instance().CatalogGet_Table(tablelists.front());		//得到数据表
			SelectRecord selectrecord; //选择的一列记录
			selectrecord.AttributesName = attributes.front(); //得到用户select属性的名字
			string WhereAttributes = wherelists.front();			//where 后的属性
			wherelists.pop();
			wherelists.push(WhereAttributes);
			Operator_type Op = Intepretor::Op_Judge(wherelists.front());//where中的属性后运算符
			wherelists.push(wherelists.front());
			wherelists.pop();
			string WhereValues = wherelists.front();					//where中的运算符后的值
			wherelists.pop();
			wherelists.push(WhereValues);
			while (AttributesNum < attributes.size())
			{
				int WhereAttributesNumber = Catalog::Instance().Catalog_GetAttributesNumber(WhereAttributes, table); //获取属性的序列值，即第几个属性
				int SelectAttributesNumber = Catalog::Instance().Catalog_GetAttributesNumber(attributes.front(), table);//获得需要的属性的序列值
				if (WhereAttributesNumber != -1)
				{
					int Tuple_Num = Tuple_Record.size();//记录的条数
					int Sign = 0;
					CatalogTable catalogtable = Catalog::Instance().CatalogGet_CatalogTable(tablelists.front());//得到数据表的模式信息
					int Tableattributesnum = catalogtable.CatalogTable_AttribtuesNum;//得到属性的数目
					for (int i = SelectAttributesNumber; i < Tableattributesnum; i++)
					{
						switch (Op)
						{
						case LESS_THAN:
							break;
						case EQUAL:
							break;
						case MORE_THAN:
							break;
						case MORE_AND_EQUAL:
							break;
						case LESS_AND_EQUAL:
							break;
						case NOT_EQUAL:
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
}