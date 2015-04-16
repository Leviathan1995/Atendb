#include "Bplus_Tree.h"
#include <string>
using namespace std;

//将string 转换为KEY模板类型
template<typename KEY>
KEY BPlusTree<KEY>::String2Key(string &str)
{
	KEY ret;
	stringstream ss;
	ss << str;
	ss >> ret;
	return ret;
}
//B+树的插入
template<typename KEY>
void BPlusTree<KEY>::BPlusTree_Insert(string &attributesname, int offset)
{
	KEY key = String2Key(attributesname);
	Node & leaf = BPlusTree_Find(BPlusTree_TreeFetchRootFromFile(), key);
	if ()
}
//
template<typename KEY>
bool BPlusTree<KEY>::BPlusTree_HasKey(Node & node, KEY & key)
{
	for (auto i = node.Node_Key.begin(); i != node.Node_Key.end(); i++)
	{
		if ()
	}
}
//
template<typename KEY>
typename BPlusTree<KEY>::Node BPlusTree<KEY>::BPlusTree_FetchRootFromFile()
{
	return Block2Node(0);
}
//
template<typename KEY>
typename BPlusTree<KEY>::Node BPlusTree<KEY>::BPlusTree_Block2Node(int blocknum)
{
	string Content;
	Buffer_Manager::Instance().Buffer_ManagerRead(Index_FileName, blocknum, Content);
	Node ret;
	BPlusTree_UnpackHead(ret, Content);
	BPlusTree_UnpackBody(ret, Content);
	re.Node_BlockNum = blocknum;
	return ret;
}