//
//  global.cpp
//  Sail
//
//  Created by Leviathan on 15/11/15.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include <iostream>
#include "global.h"
using namespace std;
string catalog_file="database";


/*
    value
*/

/*
    column
*/
column::column()
{
    
}

column::column(string name,column_type type,size_t char_size,size_t length)
{
    this->name=name;
    this->type=type;
    this->char_size=char_size;
    this->length=length;
}

column::column(string name,column_type type,size_t length)
{
    this->name=name;
    this->type=type;
    this->length=length;
}

void column::getlength()
{
    if(type==INT)
        length=sizeof(int);
    else if(type==CHAR)
        length=char_size*sizeof(char);
}

/* 
    table
*/
int table::getlength()
{
    length=0;
    for(auto column:columns)
    {
        length+=column.length;
    }
    return length;
}

const vector<column> table::getcolumns()
{
    return columns;
}

void table::insert_column(column newcolumn)
{
    columns.push_back(newcolumn);
}
/*
    global function
*/
char * string2char(string str)
{
    return const_cast<char *>(str.c_str());
}
