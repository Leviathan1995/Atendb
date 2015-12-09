//
//  API.h
//  Sail
//
//  Created by Leviathan on 15/11/15.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#ifndef api_h
#define api_h

#include <iostream>
#include <string>
#include "catalog.h"
#include "intepretor.h"
#include "global.h"
#include "buffer.h"
#include "record_manager.h"
using namespace std;

class intepretor;
class api
{
public:
    api();
    api(intepretor * parse_obj,catalog * catalog_obj,buffer * buffer_obj,record_manager * record_obj);
    intepretor *parseinfo;
    catalog *cataloginfo;
    buffer * bufferinfo;
    record_manager * recordinfo;
    void api_createtable(table newtable);
    void api_insert(string tablename,record newrecord);
};

#endif /* API_h */
