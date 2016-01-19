//
//  bitcask.cpp
//  Biu
//
//  Created by Leviathan on 15/12/23.
//  Copyright © 2015年 Leviathan. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <dirent.h>
#include "bitcask.h"
using namespace std;

bitcask::bitcask()
{
    _start=false;
    _activefile=0;
    _finish=false;
    _response="";
}

bitcask::~bitcask()
{
    merge();
    flush();
    _response+="bye bye!\n";
}

void bitcask::start()
{
    this->init();
}

void bitcask::init()
{
    this->_start=true;
    _response+="The Biu bitcask storage system  (Version 1.0.1) \n";
    _response+=cmd+"the bitcask is running...\n";
    long len;
    fstream hint;
    hint.open(filepath+"hint.bin",ios::binary|ios::app);
    if (!hint) {
        _response+=cmd+prompt+"the file hint.bin open failure!\n";
    }
    len=hint.tellg();
    if (len==0) {
        _response+=cmd+prompt+"create file hint.bin successful!\n";
    }
    else
    {
        /*
         load index to memory
         */
        _response+=cmd+"load index to memory...\n";
        bitcask_index search;
        fstream hint;
        hint.open(filepath+"hint.bin",ios::binary|ios::in);
        if (!hint) {
            _response+=cmd+prompt+"the file hint.bin open failure!\n";
        }
        while ((hint.read((char *)(&search), sizeof(search)))) {
            bitcask_index insert;
            insert.file_id=search.file_id;
            insert.value_pos=search.value_pos;
            insert.value_len=search.value_len;
            insert.timestamp=search.timestamp;
            insert.value_valid=search.value_valid;
            index[search.key]=insert;
        }
    }
    fstream filelog;
    filelog.open(filepath+"filelog.bin",ios::binary|ios::in);
    if (!filelog) {
        _response+=cmd+prompt+"the file filelog.bin open failure!\n";
    }
    filelog.read((char *)(&_activefile),sizeof(int));
    filelog.close();
    if (_activefile==0) {
        _response+=cmd+prompt+"create file filelog.bin successful!\n";
        _activefile=1;
        filelog.open(filepath+"filelog.bin",ios::binary|ios::app);
        filelog.write((char *)(&_activefile), sizeof(int));
        filelog.close();
        return ;
    }
    _start=true;
}

void bitcask::insert_data(string key,string value)
{
    bitcask_index search=read_index(key);
    if (search.key!="") {
        _response+=cmd+prompt+"the data "+key+" already exist!\n";
        return ;
    }
    //add data
    bitcask_data newdata;
    newdata.key=key;
    newdata.key_len=int(key.length());
    newdata.value=value;
    newdata.value_len=int(value.length());
    newdata.timestamp=time(0);
    
    //add index
    fstream datafile;
    bitcask_index newindex;
    newindex.key=key;
    newindex.file_id=fileprev+to_string(_activefile);
    datafile.open(filepath+newindex.file_id,ios::binary|ios::in|ios::app);
    if(!datafile)
        _response+=cmd+prompt+newindex.file_id+" open failure\n";
    newindex.value_pos=datafile.tellg();
    if (newindex.value_pos>filemax||filemax-newindex.value_pos<sizeof(newdata)) {
        _activefile++;
        newindex.value_pos=0;
        newindex.file_id=fileprev+to_string(_activefile);
    }
    newindex.timestamp=newdata.timestamp;
    newindex.value_len=sizeof(newdata);
    newindex.value_valid=true;
    datafile.close();
    write_data(newdata);
    write_index(newindex);
    //add to memory index array
    index[key]=newindex;
    _response+=cmd+"the data "+key+" insert successful\n";
}

void bitcask::write_data(bitcask_data newdata)
{
    string file=fileprev+to_string(_activefile);
    fstream datafile;
    datafile.open(filepath+file,ios::binary|ios::app);
    if(!datafile)
        _response+=cmd+prompt+file+" open file "+file+" failure!\n";
    datafile.write((char *)(&newdata),sizeof(newdata));
    datafile.close();
}

void bitcask::write_index(bitcask_index newindex)
{
    fstream hint;
    hint.open(filepath+"hint.bin",ios::binary|ios::app);
    if (!hint) {
        _response+=cmd+prompt+"the file hint.bin open failure!\n";
    }
    hint.write((char *)(&newindex), sizeof(newindex));
    hint.close();
}

bitcask_index bitcask::read_index(string key)
{
    bitcask_index search;
    for(auto indexinfo:index)
    {
        if (indexinfo.first==key) {
            {
                search.key=indexinfo.first;
                search.file_id=indexinfo.second.file_id;
                search.value_pos=indexinfo.second.value_pos;
                search.value_len=indexinfo.second.value_len;
                search.value_valid=indexinfo.second.value_valid;
                search.timestamp=indexinfo.second.timestamp;
                return search;
            }
        }
    }
    return search;
}

bitcask_data bitcask::read_data(string key)
{
    bitcask_data search_data;
    bitcask_index search_index=read_index(key);
    if (search_index.value_valid==true)
    {
        string file=search_index.file_id;
        fstream datafile;
        datafile.open(filepath+file,ios::binary|ios::in);
        if(!datafile)
            _response+=cmd+prompt+"open file "+file+" failure\n";
        datafile.seekg(search_index.value_pos,ios::beg);
        datafile.read((char *)(&search_data),sizeof(search_data));
        return search_data;
    }
    else
        _response+=cmd+prompt+"the data "+key+" does not exist!\n";
    return search_data;
}

void bitcask::read_datainfo(string key)
{
    bitcask_data data=read_data(key);
    bitcask_index index=read_index(key);
    if(index.value_valid==true)
    {
        _response+=cmd+"key :"+key+"\n";
        _response+=cmd+"value :"+data.value+"\n";
        _response+=cmd+"file id :"+index.file_id+"\n";
        _response+=cmd+"value pos :"+to_string(index.value_pos)+"\n";
        _response+=cmd+"value length :"+to_string(data.value_len)+"\n";
        _response+=cmd+"time :"+ctime(&data.timestamp);
    }
    else
        return;
}

void bitcask::delete_data(string key)
{
    bitcask_index delindex=read_index(key);
    if (delindex.key!="")
    {
        delindex.value_valid=false;
        index[key]=delindex;
        _response+=cmd+prompt+"the data "+key+" already delete!\n";
    }
    else
        _response+=cmd+"the data "+key+" does not exist!\n";
}

void bitcask::update_data(string key, string value)
{
    
    bitcask_index search=read_index(key);
    if (search.key=="") {
        _response+=cmd+prompt+"the data "+key+" does not exist!\n";
        return;
    }
    //update data
    fstream datafile,hintfile;
    bitcask_data updata;
    bitcask_index upindex=read_index(key);
    updata.key=key;
    updata.key_len=int(key.length());
    updata.value=value;
    updata.value_len=int(value.length());
    updata.timestamp=time(0);
    
    //update index
    upindex.file_id=fileprev+to_string(_activefile);
    datafile.open(filepath+upindex.file_id,ios::binary|ios::in|ios::app);
    if(!datafile)
        _response+=cmd+prompt+"the file "+upindex.file_id+" open failure!\n";
    upindex.value_pos=datafile.tellg();
    if (upindex.value_pos>filemax||filemax-upindex.value_pos<sizeof(updata)) {
        _activefile++;
    }
    upindex.timestamp=updata.timestamp;
    upindex.value_len=sizeof(updata);
    upindex.value_valid=true;
    datafile.close();
    write_data(updata);
    update_index(upindex,key);
    _response+=cmd+"the data "+key+" update successful\n";
}

void bitcask::update_index(bitcask_index upindex,string key)
{
    bitcask_index seaindex=read_index(key);
    seaindex.key=key;
    seaindex.file_id=upindex.file_id;
    seaindex.value_pos=upindex.value_pos;
    seaindex.value_len=upindex.value_len;
    seaindex.value_valid=upindex.value_valid;
    seaindex.timestamp=upindex.timestamp;
    index[key]=seaindex;
}

void bitcask::merge()
{
    /*
     merge data in file
     function: delete data in file
     */
    int beans=1;
    long value_pos;
    vector<bitcask_data> data_array;
    for (;beans<=_activefile;beans++) {
        string file=fileprev+to_string(beans);
        fstream datafile;
        datafile.open(filepath+file,ios::binary|ios::in);
        if (!datafile) {
            _response+=cmd+prompt+"the data file "+file+" open failure!\n";
        }
        bitcask_data beans_data;
        datafile.seekg(0,ios::beg);
        //load to memory
        while (datafile.read((char *)(&beans_data),sizeof(beans_data))) {
            bitcask_index beans_index=read_index(beans_data.key);
            if (beans_index.value_valid==true&&beans_data.timestamp==beans_index.timestamp)
                data_array.push_back(beans_data);
        }
        datafile.close();
        //write to file
        datafile.open(filepath+file,ios::binary|ios::out);
        if (!datafile) {
            _response+=cmd+prompt+"the data file "+file+" open failure!\n";
        }
        datafile.seekg(0, ios::beg);
        for(auto data:data_array)
        {
            value_pos=datafile.tellg();
            datafile.write((char *)(&data),sizeof(data));
            bitcask_index seaindex=read_index(data.key);
            seaindex.value_pos=value_pos;
            index[data.key]=seaindex;
        }
        datafile.close();
        data_array.clear();
    }
    /*
     merge file
     */
    if (
        _activefile>=2)
    {
        while (_activefile>1)
        {
            string file=fileprev+to_string(_activefile);
            fstream datafile;
            datafile.open(filepath+file,ios::binary|ios::in);
            if (!datafile) {
                _response+=cmd+"the data file "+file+" open failure!\n";
            }
            bitcask_data beans_data;
            datafile.seekg(0,ios::beg);
            //load to memory
            while (datafile.read((char *)(&beans_data),sizeof(beans_data))) {
                data_array.push_back(beans_data);
            }
            datafile.close();
            //write to file
            for (int pos=1; pos<_activefile; pos++) {
                string mergefile=fileprev+to_string(pos);
                fstream datafile;
                datafile.open(filepath+mergefile,ios::binary|ios::in|ios::app);
                if (!datafile) {
                    _response+=cmd+prompt+"the data file "+file+" open failure!\n";
                }
                long mergefile_end=datafile.tellg();
                bitcask_data merge_data=data_array.back();
                bitcask_index merge_index=read_index(merge_data.key);
                while(mergefile_end>filemax||filemax-mergefile_end>sizeof(merge_data)) {
                    datafile.write((char *)(&merge_data), sizeof(merge_data));
                    merge_data=data_array.back();
                    merge_index=read_index(merge_data.key);
                    merge_index.value_pos=mergefile_end;
                    merge_index.file_id=mergefile;
                    index[merge_data.key]=merge_index;
                    mergefile_end+=sizeof(merge_data);
                    data_array.pop_back();
                    merge_data=data_array.back();
                }
                datafile.close();
            }
            if (data_array.size()==0) {
                _activefile--;
            }
            else
            {
                fstream newdatafile;
                newdatafile.open(filepath+file,ios::binary|ios::out);
                if (!newdatafile) {
                    _response+=cmd+prompt+"the data file "+file+" open failure!\n";
                }
                newdatafile.seekg(0, ios::beg);
                for(auto data:data_array)
                    newdatafile.write((char *)(&data), sizeof(data));
                newdatafile.close();
                break;
            }
        }
    }
}


void bitcask::flush()
{
    //write index file to index file hint.bin
    fstream hint;
    hint.open(filepath+"hint.bin",ios::binary|ios::out);
    if (!hint) {
        _response+=cmd+prompt+"the file hint.bin open failure!\n";
    }
    hint.seekg(0,ios::beg);
    for (auto indexinfo:index) {
        if(indexinfo.second.value_valid==true)
            hint.write((char *)(&indexinfo.second), sizeof(indexinfo.second));
    }
    hint.close();
    //write active file number to file filelog.bin
    fstream filelog;
    filelog.open(filepath+"filelog.bin",ios::binary|ios::out);
    if (!filelog) {
        _response+=cmd+prompt+"the filelog.bin open failuer!\n";
    }
    filelog.write((char *)(&_activefile), sizeof(int));
}
