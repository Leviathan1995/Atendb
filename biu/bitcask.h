//
//  bitcask.h
//  Bius
//
//  Created by Leviathan on 15/12/23.
//  Copyright © 2015年 Leviathan. All rights reserved.
//


#ifndef bitcask_h
#define bitcask_h

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

#define filemax 4096 //4kb

const string filepath="/Users/Leviathan/Downloads/LibBiu/";
const string fileprev="biu";
const string prompt="Biu : ";
const string cmd=">>> ";
const int number=0;
//data
struct bitcask_data
{
    string key;
    int key_len;
    string value;
    int value_len;
    time_t timestamp;
    //crc
};

//index
struct bitcask_index
{
    string key;
    string file_id;
    long value_pos;
    int value_len;
    time_t timestamp;
    bool value_valid;
};

//bitcask
class bitcask
{
public:
    bitcask();
    bool start;
    bool finish;
    string response;
    unordered_map<string,bitcask_index> index;
    int activefile;
    void init();
    void insert_data(string key,string value);
    void write_data(bitcask_data newdata);
    void write_index(bitcask_index newindex);
    bitcask_data read_data(string key);
    void read_datainfo(string key);
    void read_file(string filename);
    bitcask_index read_index(string key);
    void delete_data(string key);
    void update_data(string key,string value);
    void update_index(bitcask_index upindex,string key);
    ~bitcask();
    void merge();
    void flush(); //flush index :hint.bin
};

#endif /* bitcask_h */
