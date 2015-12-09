//
//  record_manager.cpp
//  Sail
//
//  Created by Leviathan on 15/12/3.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include "record_manager.h"
using namespace std;

int record_manager::record_insert(string tablename,table t,record newrecord,buffer *bufferinfo)
{
    if(bufferinfo->buffer_fileexist(tablename,RECORD_FILE)==false)
    {
        bufferinfo->buffer_createfile(tablename, RECORD_FILE);
        bufferinfo->buffer_newblock(tablename, RECORD_FILE);
    }
    block lastblock=bufferinfo->buffer_readlast(tablename, RECORD_FILE);
    BYTE * newdata=lastblock.data;
    int record_length=t.getlength();
    int record_num=4096/record_length;
    for (int num=0; num<record_num; num++) {
        BYTE * record_seek=newdata+num*record_length;
        if (record_seek[0]==0) {
            record_seek[0]=1;
            record_wrecord2block(record_seek,t,newrecord);
            bufferinfo->buffer_write(lastblock);
            return lastblock.offset;
        }
    }
    /*
      don't hava free block
    */
    int offset=bufferinfo->buffer_newblock(tablename, RECORD_FILE);
    block record_block=bufferinfo->buffer_read(tablename,RECORD_FILE, offset);
    BYTE * record_seek=record_block.data;
    record_seek[0]=1;
    record_wrecord2block(record_seek+1, t,newrecord);
    bufferinfo->buffer_write(record_block);
    return record_block.offset;
}

void record_manager::record_wrecord2block(BYTE *seek , table t,record newrecord)
{
    auto  value=newrecord.records.begin();
    auto  type=t.columns.begin();
    for (;value!=newrecord.records.end();value++,type++) {
        switch (type->type) {
            case CHAR:
                memcpy(seek,string2char(value->char_value),value->char_size);
                seek+=value->char_size;
                break;
            case INT:
                memcpy(seek,&value->int_value, sizeof(int));
                seek+=sizeof(int);
                break;
            default:
                break;
        }
    }
}
