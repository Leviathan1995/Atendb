//
//  global.h
//  Sail
//
//  Created by Leviathan on 15/11/15.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#ifndef global_h
#define global_h

#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef char BYTE;
extern string catalog_file;
enum state
{
    CREATE,             //"create"
    TABLE,              //"table"
    TABLE_NAME,         // table name
    LEFT_BRACKET,       //"("
    RIGHT_BRACKET,      //")"
    COLUMN_NAME,        //column name
    COLUMN_TYPE,        //column type
    COLUMN_PRIMARY,     //column primary
    COLUMN_NULL,        //column null
    PRIMARY_KEY,        //primary key
    COMMA,              //","
    SEMICOLON,          //";"
    INSERT,             //"insert"
    INTO,               //"into"
    VALUES,             //"values"
    VALUE,              //insert into values( VALUE,...)
};

enum column_type
{
    INT,
    CHAR,
    ERROR_TYPE
};

enum file_type
{
    INDEX_FILE,
    RECORD_FILE
};

class column
{
public:
    column();
    column(string name,column_type type,size_t char_size,size_t length);
    column(string name,column_type type,size_t length);
    string name;
    size_t char_size;
    size_t length;     //byte number
    column_type type;
    bool null;
    bool primary;
    bool unique;
    bool is_null(){return null;}
    bool is_primary(){return  primary;}
    bool is_unique(){return unique;}
    void getlength();
};

class table
{
public:
    string name;
    int length;     //bytes
    size_t column_nums;
    vector<column> columns;
    int getlength();
    const vector<column> getcolumns();
    void insert_column(column newcolumn);
};


class value
{
public:
    value()=default;
    value(string s,size_t size):
    char_value(s),char_size(size){};
    value(int num):
    int_value(num){};
    int int_value;
    string char_value;
    size_t char_size;
    column_type type;
    
};

//record
class record
{
public:
    vector<value> records;
};

//data block
class block
{
public:
    block()=default;
    block(string tablename,int offet,file_type type);
    string tablename;
    int offset;
    file_type type;
    BYTE data[4096];
};

//global function

char * string2char(string str);
#endif /* global_h */
