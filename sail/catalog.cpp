//
//  catalog.cpp
//  Sail
//
//  Created by Leviathan on 15/11/19.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "catalog.h"
using namespace std;

catalog::catalog()
{
    catalog_load();
}
void catalog::catalog_load()
{
    catalog_table.clear();
    ifstream file;
    file.open(catalog_file);
    if (!file) {
        error("catalog","load","can't open database file ,maybe not initialized!");
    }
    string tablename,path;
    while (file>>tablename) {
        file>>path;
        if (tablename.length()==0) {
            error("catalog","load","database file format error");
        }
        catalog_loadtable(tablename, path);
    }
    file.close();
}

void catalog::catalog_loadtable(string tablename,string path)
{
    table loadtable;
    loadtable.name=tablename;
    string column,type;
    size_t char_size = 0,length=0;
    column_type col_type=ERROR_TYPE;
    ifstream file;
    path="/Users/Leviathan/Documents/Xcode_Sail/Sail/Sail/"+path;
    file.open(string2char(path));
    if (!file) {
        error("catalog","loadtable","can't open define file");
    }
    while (file>>column) {
        file>>type;
        file>>length;
        if (atoi(type.c_str())==1) {
            col_type=CHAR;
        }
        else if(atoi(type.c_str())==0)
        {
            col_type=INT;
        }
        else
            error("catalog","catalog_loadtable","column type error");
        if (col_type==CHAR)
        {
            class column loadcolumn(column,col_type,char_size,length);
            loadtable.insert_column(loadcolumn);
        }
        else
        {
            class column loadcolumn(column,col_type,sizeof(int));
            loadtable.insert_column(loadcolumn);
        }
    }
    loadtable.column_nums=loadtable.columns.size();
    catalog_addtable(loadtable);
    file.close();
}

void catalog::catalog_flush()
{
    ofstream file;
    file.open(string2char(catalog_file),ios::app);
    if (!file) {
        error("catalog","flush","can't open database file");
    }
    for (auto pointer:catalog_table) {
        string tablename=pointer.first;
        file<<tablename<<" "<<tablename<<".def"<<"\n";
        ofstream tablefile;
        tablefile.open(string2char(pointer.first+".def"));
        if(!tablefile)
            error("catalog","flush","can't open define file");
        for (auto column:pointer.second.columns) {
            tablefile<<column.name<<" "<<column.type<<" "<<column.length<<" "<<"\n";
        }
        tablefile.close();
    }
    file.close();
}

void catalog::catalog_addtable(table newtable)
{
    if(catalog_table.find(newtable.name)!=catalog_table.end())
        error("catalog", "catalog_addtable()", "the table alreay exist");
    else
        catalog_table[newtable.name]=newtable;
}

table & catalog::catalog_gettable(string name)
{
    if(catalog_table.find(name)==catalog_table.end())
        error("catalog","catalog_gettable","no such table");
    return catalog_table[name];
}
