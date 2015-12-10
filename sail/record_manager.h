//
//  record_manager.h
//  Sail
//
//  Created by Leviathan on 15/12/3.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#ifndef record_manager_h
#define record_manager_h

#include <iostream>
#include "global.h"
#include "buffer.h"
using namespace std;

class table;
//record manager
class record_manager
{
public:
    int record_insert(string tablename,table t,record r,buffer * buffer_obj);
    void record_wrecord2block(BYTE * seek,table t,record r);
};

#endif /* record_manager_h */
