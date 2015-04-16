#include "Index.h"
#include "Bplus_Tree.h"
void Index_Manager::Index_InsertIndex(string & tablename, Attributes & attributes, vector<pair<string, int>> &Info)
{
	switch (attributes.Attributes_type)
	{
	case Int:
	{
				BPlusTree<int> tree(M_PREFIX + tablename + string("@") + attributes.Attributes_Name, attributes);
				return Index_TreeInsert<int>(tree, Info);
	}
	case Char:
	{
				 BPlusTree<string> tree(M_PREFIX + tablename + string("@") + attributes.Attributes_Name, attributes);
				 return Index_TreeInsert<string>(tree, Info);
	}
	case Float:
	{
				  BPlusTree<float> tree(M_PREFIX + tablename + string("@") + attributes.Attributes_Name, attributes);
				  return Index_TreeInsert<float>(tree, Info); 
	}
	default:
		break;
	}
}