#include "API.h"
using namespace std;
Selection API::Select(string table_name, vector<Comparison>)
{
	Table_Type table_select = Mem_Catalog->Get_Table(table_name);
}