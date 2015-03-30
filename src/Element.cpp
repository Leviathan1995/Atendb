#include "Elemment.h"
/*
	记录的构造函数
*/
Element::Element(string s,int Mem_charnum)
{
	Mem_ColType = Char;
	Mem_Char = s;
	Mem_CharNum = Mem_charnum;
}
Element::Element(float F)
{
	Mem_ColType = Float;
	Mem_Float = F;
}
Element::Element(int i)
{
	Mem_ColType = Int;
	Mem_Int = i;
}