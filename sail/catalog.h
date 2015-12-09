//
//  Catalog.h
//  Sail
//
//  Created by Leviathan on 15/11/19.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#ifndef catalog_h
#define catalog_h

#include <iostream>
#include <map>
#include "error.h"
#include "global.h"
using namespace std;

class catalog
{
public:
    catalog();
    void catalog_load();
    void catalog_flush();
    void catalog_loadtable(string tablename,string path);
    map<string,table> catalog_table;
    void catalog_addtable(table newtable);
    table & catalog_gettable(string name);
};

#endif /* catalog_h */
