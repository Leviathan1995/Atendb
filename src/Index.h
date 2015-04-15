#pragma once
#ifndef _INDEX_H
#define _INDEX_H
#include <string>
#include <vector>
#include "Attributes.h"
#include "Table.h"
using namespace std;

class Index_Manager
{
public:
	Index_Manager(){}
	Index_Manager(Index_Manager &);
	~Index_Manager(){}
	static Index_Manager & Instance(); //实例化
	void Index_CreateIndex(string & tablename,vector<Attributes> & attributes); //供API::CreateTable 调用
	void Index_CreataIndex(string & tablename, Attributes & attributes);//供API::CreateIndex 调用
	void Index_DropIndex(string &tablename,vector<string> & AttributesName);//供API:;DropTable 调用
	void Index_DropIndex(string & tablename,string & attritbuesname);//供API::DropIndex调用
	void Index_InsertIndex(string &tablename, Attributes & attributes, vector< pair<string, int>>&Info);//供API::InsertTuple调用
	void Index_DeleteIndex(string &tablename, Attributes &attributes, vector<pair<string, int>> & Info);//供API::DeleteTuple调用
	void Index_InsertCheck(Table &table, vector<string> & StrTuple);//供API::InsertTuple

};
inline Index_Manager & Instance()
{
	Index_Manager instance;
	return instance;
}
#endif