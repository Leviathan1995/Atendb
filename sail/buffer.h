//
//  buffer.h
//  Sail
//
//  Created by Leviathan on 15/11/19.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#ifndef buffer_h
#define buffer_h

#include <iostream>
#include "global.h"
using namespace std;

class buffer
{
public:
    block buffer_read(string tablename,file_type type,int offset);
    void buffer_write(block b);
    void buffer_createfile(string tablename,file_type type);
    bool buffer_fileexist(string tablename,file_type type);
    int buffer_newblock(string tablename,file_type type);
    block buffer_readlast(string tablename,file_type type);
};

#endif /* buffer_h */
