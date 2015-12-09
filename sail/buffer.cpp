//
//  buffer.cpp
//  Sail
//
//  Created by Leviathan on 15/11/19.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "buffer.h"
#include "error.h"
using namespace std;

block buffer::buffer_read(string tablename, file_type type, int offset)
{
    block readblock;
    ifstream file;
    if (type==RECORD_FILE) {
        file.open(string2char(tablename+".d"),ios::binary|ios::in);
    }
    if (!file) {
        error("buffer","buffer_read","read block failure");
    }
    file.seekg(offset*4096,ios::beg);
    file.read(readblock.data,4096);
    file.close();
    readblock.offset=offset;
    readblock.tablename=tablename;
    readblock.type=type;
    return readblock;
}

void buffer::buffer_write(block b)
{
    fstream file;
    if (b.type==RECORD_FILE) {
        file.open(string2char(b.tablename+".d"), ios::out|ios::binary);
    }
    if (!file) {
        error("buffer","buffer_write","write block failure");
    }
    file.seekg(b.offset*4096, ios::beg);
    file.write(b.data, sizeof(BYTE));
    file.close();
}

void buffer::buffer_createfile(string name,file_type type)
{
    ofstream file;
    if(type==RECORD_FILE)
        file.open(string2char(name+".d"));
    if(!file)
        error("buffer","buffer_createfile","table file create failure");
}

bool buffer::buffer_fileexist(string name, file_type type)
{
    fstream file;
    if(type==RECORD_FILE)
        file.open(string2char(name+".d"));
    if(!file)
        return false;
    else
    {
        file.close();
        return true;
    }
}

int buffer::buffer_newblock(string tablename, file_type type)
{
    fstream file;
    if(type==RECORD_FILE)
        file.open(string2char(tablename+".d"), ios::binary|ios::out|ios::in);
    if(!file)
        error("buffer","buffer_newblock","create new block failure");
    file.seekg(0,ios::end);
    int offset=int(file.tellg()/4096);
    block newblock;
    file.write(newblock.data,4096);
    file.close();
    return offset;
}

block buffer::buffer_readlast(string tablename, file_type type)
{
    block lastblock;
    fstream file;
    if (type==RECORD_FILE) {
        file.open(string2char(tablename+".d"), ios::in|ios::binary|ios::ate);
    }
    if (!file) {
        error("buffer","buffer_readlast","open last block failure");
    }
    file.seekg(0, ios::end);
    int length=int(file.tellg());
    if (length==0) //empty file
    {
        int offset=buffer_newblock(tablename, type);
        return buffer_read(tablename, type, offset);
    }
    else
    {
        file.seekg(0,ios::end);
        int offset=int(file.tellg()/4096);
        file.seekg(-4096,ios::end);
        file.read(lastblock.data,4096);
        file.close();
        lastblock.offset=offset;
        lastblock.tablename=tablename;
        lastblock.type=type;
    }
    return lastblock;
}
