#include "Record_Manager.h"
//Create建表
bool Record_Manager::Record_ManagerCreateTable(string &tablename, const vector<Attributes>&Table_Column)
{
	string FileName = tablename + ".table";
	string Empty_Block(Block_Size, 0);						//申请一个新的块
	int Tuple_size = 0;										//数据表中元组需要分配的内存大小
	for (size_t i = 0; i < Table_Column.size(); i++)			//Table_Column.size()数据表的属性个数
		Tuple_size += Table_Column[i].Attributes_Length;
	Tuple_size++;
	int Offset = 0;//偏移量
	while ((Offset + Tuple_size) < Block_Size)
	{
		Empty_Block.replace(Offset + Tuple_size - 1, 1, "1");//替换
		Offset += Tuple_size;
	}
	Buffer_Manager::Instance().Buffer_ManagerWrite(FileName,Empty_Block);			//通过缓冲区管理写入文件
	return true;
}
//Insert into 插入记录
bool  Record_Manager::Record_ManagerInsert_Into(Table &table, vector<Tuple> Tuple_Lists)
{
	vector<pair<Tuple, int>> insert_tuple;
	string filename = table.Table_Name + ".table";
	char Dirty = '0';
	int Lengh = table.Table_Length() + 1;           //一条记录的长度
	string InsertContent = "";						//用户插入的记录字符串
	string str;
	int blocknum = Buffer_Manager::Instance().Buffer_ManagerReadLastNumber(table.Table_Name, str); //获得块号
	for (size_t i = 0; i < Tuple_Lists.size(); i++)
	{
		Tuple Insert_Tuple = Tuple_Lists[i];     //一条记录
		for (size_t j = 0; j < Insert_Tuple.Tuple_content.size(); i++) //一条记录中的一个字段
		{
			if (table.Table_AttributesList[j].Attributes_Unique()== true)//如果这个字段的属性是Unique
			{
				if (Record_ManagerHasExisted(table, Insert_Tuple.Tuple_content[j], j, blocknum) == true) //传递的参数为数据表 table,字段的内容，第几个属性，块号
					return false;
				Insert_Tuple.Tuple_content[j].resize(table.Table_AttributesList[j].Attributes_Length, 0);
				InsertContent += Insert_Tuple.Tuple_content[j];
			}
		}
		InsertContent += Dirty;
		for (int k = 0; k <= blocknum; k++) //查找现存块中的空间
		{
			string Strout;
			Buffer_Manager::Instance().Buffer_ManagerRead(filename, i, Strout);	//读取块中的数据
			int Offset = Record_ManagerFindDirtyTuple(Strout, Lengh);			//找到块中空余空间的偏移量
			if (Offset != -1)
			{
				Strout.replace(Offset, Lengh, InsertContent);
				Buffer_Manager::Instance().Buffer_ManagerWrite(filename, Strout, i);
				insert_tuple.push_back(make_pair(Insert_Tuple, i*Block_Size + Offset));
			}
			else
			{
				continue;//如果Offset=-1,说明该块中不存在已经删除的记录，继续从下一个块读
			}
		}
		int i;
		if (i > blocknum) //缓冲区的块已满
		{
			InsertContent.resize(Block_Size, 0);
			Buffer_Manager::Instance().Buffer_ManagerWrite(filename, InsertContent);
			insert_tuple.push_back(make_pair(Insert_Tuple,blocknum*Block_Size));
		}
	}
	API::Instance().FixIndex(table, insert_tuple);				//修复索引
	return true;
	
}
//元组是否已经存在,num代表第几个属性，content为字段内容
bool Record_Manager::Record_ManagerHasExisted(Table &table, string &content, int num, int blocknum)
{
	bool Has = false;
	vector<Tuple> SearchTuple = Record_ManagerSelectTuple(table,blocknum);//遍历搜寻的元组
	for (int i = 0; i <= SearchTuple.size(); i++)
	{
		if (SearchTuple[i].Tuple_content[num] == content)
		{
			Has = true;
			break;
		}
	}

}
//获得选择的元组
vector<Tuple> Record_Manager::Record_ManagerSelectTuple(Table & table,int blocknum)
{
	string filename = table.Table_Name + ".table";
	vector<Tuple> Selected;
	size_t size = table.Table_Length() + 1; //数据表一条记录的长度,加1是包含dirty标志位
	for (int i = 0; i < blocknum; i++)//遍历所有块
	{
		string strout;
		Buffer_Manager::Instance().Buffer_ManagerRead(filename, i, strout); //读出Block的内容,strout为块的所有内容
		size_t Pointer = 0;
		while ((Pointer + size) < strout.size()) //一条记录一条记录的获取
		{
			string substring = strout.substr(Pointer, size); //从Pointer开始，复制size大小的字符串
			vector<string> vec = Record_ManagerString2Tuple(table.Table_AttributesList, substring);//将字符串分割为元组
			Tuple * NewTuple = new Tuple(vec);  //将Vector<string> 构造为Vector<Tuple>类型
			Selected.push_back(*NewTuple);
			Pointer += size;
		}
	}
	return Selected;
}
//将string转为Tuple ，存放形式仍然为vector<string>
vector<string> Record_Manager::Record_ManagerString2Tuple(vector<Attributes> attribtues, string tuple_str)
{
	vector<Attributes>::iterator i;//数据表元素的智能指针
	vector<string> result;
	int ptr = 0;
	for (i = attribtues.begin(); i != attribtues.end(); i++)
	{
		string tmp = tuple_str.substr(ptr, (*i).Attributes_Length);
		ptr += (*i).Attributes_Length;
		result.push_back(tmp);
	}
	return result;
}
//
int Record_Manager::Record_ManagerFindDirtyTuple(string &strout, int size)
{
	int Offset = 0;
	while ((size_t)(Offset + size) < strout.size())
	{
		string str = strout.substr(Offset, size);	
		string Dirty = str.substr(size - 1, 1);		//size-1 后全部替换为1
		if (Dirty == "1")							//如果找到被修改的位置				
			return Offset;							//返回偏移量
		Offset += size;
	}
	return -1;
}