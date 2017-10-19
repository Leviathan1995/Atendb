//
//  bitcask.h
//  Biu
//
//  Created by Leviathan on 15/12/24.
//  Copyright © 2015年 Leviathan. All rights reserved.
//


#ifndef BITCASK_H_
#define BITCASK_H_

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <pwd.h>
using namespace std;

#define filemax 4096 //4kb

const string fileprev="biu";
const string cmd_prompt=">>> Biu : ";
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


struct bitcask_data2 {
    time_t timestamp;
    long key_size;
    long value_size;
    std::string key;
    std::string value;
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

// the format store in hint file
struct bitcask_index2 {
    unsigned long crc;
    time_t timestamp;
    long file_id;
    long value_pos;
    long value_size;
    long key_size;
    std::string key;
};
//bitcask
class bitcask
{
public:

    //function
    bitcask();
    void start();
    void init();
    string getpath();
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
    void merge();
    void flush(); //flush index :hint.bin
    ~bitcask();

    //data
    unordered_map<string,bitcask_index> index;
    int _activefile;
    bool _start;
    bool _finish;
    string _response;
    string filepath;
};

#endif /* BITCASK_H_ */
