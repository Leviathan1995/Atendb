//
//  API.cpp
//  Sail
//
//  Created by Leviathan on 15/11/15.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include "api.h"
using namespace std;

api::api()
{
    
}

api::api(intepretor * parse_obj,catalog * catalog_obj,buffer * buffer_obj,record_manager * record_obj)
{
    this->parseinfo=parse_obj;
    this->cataloginfo=catalog_obj;
    this->bufferinfo=buffer_obj;
    this->recordinfo=record_obj;
}

//create table
void api::api_createtable(table newtable)
{
    cataloginfo->catalog_addtable(newtable);
    cataloginfo->catalog_flush();
    bufferinfo->buffer_createfile(newtable.name, RECORD_FILE);
}

//insert value
void api::api_insert(string tablename, record newrecord)
{
    table t=cataloginfo->catalog_gettable(tablename);
    auto column=t.getcolumns();
    for (auto i=column.begin(); i!=column.end(); i++) {
        // unique or primary
    }
    int offset=recordinfo->record_insert(tablename,t, newrecord, bufferinfo);
}
