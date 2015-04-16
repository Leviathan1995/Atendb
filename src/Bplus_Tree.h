#pragma once
#ifndef _BPLUS_TREE_
#define _BPLUS_TREE_
#include <vector>
#include <string>
#include <list>
#include "Attributes.h"
#include "Glob_Var.h"
#include "Buffer.h"
#include "API.h"
#include "Catalog.h"
using namespace std;

//B+树
template<typename KEY>
class BPlusTree
{
public:
	class Node;
	BPlusTree(string & tablename, Attributes & attributes);
	~BPlusTree();
	void BPlusTree_Insert(string &attributes,int Offset);
	void BPlusTree_Remove();
	vector<int> BPlusTree_Find();
	bool BPlusTree_Existed();
	void BPlusTree_CreateRootInEmptyFile();
	vector<int> BPlusTree_FindEqual();
	vector<int> BPlusTree_FindNotEqual();
	vector<int> BPlusTree_FindLess();
	KEY String2Key(string & str);
	Node BPlusTree_TreeFetchRootFromFile();
	Node BPlusTree_Block2Node(int blocknum);
	void BPlusTree_UnpackHead(Node & node, string & content);
	void BPlusTree_UnpackBody(Node & node, string & content);
	bool BPlusTree_HasKey(Node & node, KEY & key);
	//数据成员
	string Index_FileName;
	Attributes_Type Index_attributes_type;
	size_t Index_Length;
	size_t Index_MaxSize;
	size_t Index_ValuePtr;
	class Node
	{
	public:
		int Node_BlockNum;
		list<KEY> Node_Key;
	};
};

static char M_HEAD_SIZE = 16;
static int M_ROOT_NUM = 0;
static string M_PREFIX = ".\\index\\";

//构造
template<typename KEY>
BPlusTree<KEY>::BPlusTree(string &tablename, Attributes &attributes):
Index_FileName(tablename), Index_attributes_type(attributes.Attributes_type), Index_Length(attributes.Attributes_Length),
Index_MaxSize((Block_Size - M_HEAD_SIZE - 4) / (attributes.Attributes_Length + 4) + 1)
{
	if (BPlusTree_Existed())
	{
		string Content;
		Buffer_Manager::Instance().Buffer_ManagerRead(Index_FileName, M_ROOT_NUM, Content);
		Index_ValuePtr = Intepretor::ForBytes2Int(Content, 9, 13);
	}
	else
		BPlusTree_CreateRootInEmptyFile();
}
	
#endif