#pragma once
#ifndef _ATTRIBUTES_H
#define _ATTRIBUTES_H
#include <string>
#include "Glob_Var.h"
using namespace std;
/*
	数据表中的属性
*/
class Attributes
{
public:
	size_t Attributes_Flag;
	size_t Attributes_Length;
	string Attributes_Name;
	Attributes_Type Attributes_type;
	bool Attributes_Null();
	bool Attributes_Primary();
	bool Attributes_Unique();
};
#endif