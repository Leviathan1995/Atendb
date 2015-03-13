#ifndef _ELEMMENT_H
#define _ELEMMETN_H
#include <string>
#include "Glob_Var.h"
#include "Catalog.h"
using namespace std;
class Element
{
public:
	Element();
	~Element();

	int Mem_Int;
	float Mem_Float;
	string Mem_Char;
	int Mem_CharNum;
	
	ColType Mem_ColType;//int or float or char 
	bool operator<(Element e);
	bool operator==(Element e);
	bool operator>(Element e);
	bool operator>=(Element e);
	bool operator<=(Element e);
	bool operator!=(Element e);
	
	Element(string s, int Mem_charnum);
	Element(float f);
	Element(int i);
};
class Comparison
{
public:
	Comparison();
	Comparison(Column_Type Column, Operator_type Op, Element Elem);
	ColType Mem_Col;
	Operator_type Oper;
	Element Elem;
};
#endif