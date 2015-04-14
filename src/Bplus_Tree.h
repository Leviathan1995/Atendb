#pragma once
#ifndef _BPLUS_TREE_
#define _BPLUS_TREE_
#include <vector>
#include <string>
#include "Attributes.h"
using namespace std;

class BPlusTree
{
public:
	class Node;
	BPlusTree(string & tablename, Attributes & attributes);
	~BPlusTree();
	void BPlusTree_Insert();
	void BPlusTree_Remove();
	vector<int> BPlusTree_Find();
	bool BPlusTree_Existed();
	void BPlusTree_CreateRootInEmptyFile();
	Node BPlusTree_FindMinLeaf();
	vector<int> BPlusTree_FindEqual();
	vector<int> BPlusTree_FindNotEqual();
	vector<int> BPlusTree_FindLess();


};
#endif