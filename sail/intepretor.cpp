//
//  intepretor.cpp
//  Sail
//
//  Created by Leviathan on 15/11/11.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "intepretor.h"
using namespace std;

int intepretor::string2int(string str)
{
    return atoi(str.c_str());
}

void intepretor::cmd_input()
{
    bool end=false;
    command.clear();
    cout<<"->";
    string cmd;
    while (getline(cin, cmd))
    {
        string token;
        for(auto c_char:cmd)
        {
            if((c_char>= 'a'&&c_char<'z') || (c_char>= 'A'&&c_char<= 'Z') || (c_char>= '0'&&c_char<='9'))
                token+=c_char;
            else if(c_char=='/'||c_char=='.'||c_char=='_')
                token+=c_char;
            else if(c_char==','||c_char=='('||c_char==')')
            {
                if (token!="") {
                    command.push_back(token);
                    token="";
                }
                token=c_char;
                command.push_back(token);
                token="";
            }
            else if(c_char==';')
            {
                if(token=="")
                    command.push_back(token);
                end=true;
            }
            else
            {
                if (token!="")
                {
                    command.push_back(token);
                    token="";
                }
            }
        }
        if (end) {
            command.push_back(token);
            break;
        }
        if (token!="")
        {
            command.push_back(token);
        }
        cout<<"->";
    }
    if ([](string cmd){return cmd=="quit";}(command[0])) {
        return;
    }
    else
       parse_cmd();
}


void intepretor::parse_cmd()
{
    if ([](string cmd){return cmd=="execute";}(command[0]))
        intepretor_execfile();
    else if ([](string cmd1,string cmd2){return cmd1=="create"&&cmd2=="database";}(command[0],command[1]))
        intepretor_createdb();
    else if([](string cmd1,string cmd2){return cmd1=="create"&&cmd2=="table";}(command[0],command[1]))
        intepretor_createtable();
    else if([](string cmd1,string cmd2){return cmd1=="insert"&&cmd2=="into";}(command[0],command[1]))
        intepretor_insert();
    /*
    else if([](string cmd1,string cmd2){return cmd1=="create"&&cmd2=="index";}(command[0],command[1]))
        createindex();
    else if([](string cmd){return  cmd=="select";}(command[0]))
        select();
    
     */
}

void intepretor::intepretor_execfile()
{
    bool end=false;
    ifstream file;
    string filename,cmd;
    filename=command[1];
    file.open(filename);
    command.clear();
    while (getline(file, cmd))
    {
        string token;
        for(auto c_char:cmd)
        {
            if((c_char>= 'a'&&c_char<'z') || (c_char>= 'A'&&c_char<= 'Z') || (c_char>= '0'&&c_char<='9'))
                token+=c_char;
            else if(c_char==','||c_char=='('||c_char==')'||c_char=='"')
            {
                if (token!="") {
                    command.push_back(token);
                    token="";
                }
                token=c_char;
                command.push_back(token);
                token="";
            }
            else if(c_char==';')
                end=true;
            else
            {
                if (token!="")
                {
                    command.push_back(token);
                    token="";
                }
            }
        }
        if (end) {
            command.push_back(";");
            break;
        }
        if (token!="")
        {
            command.push_back(token);
        }
    }
    parse_cmd();
}
void intepretor::intepretor_createdb()
{
    
}

void intepretor::intepretor_createtable()
{
    int index=-1;
    table newtable;
    state nowstate=CREATE;
    for (auto token=command.begin();token!=command.end();token++) {
        switch (nowstate) {
            case CREATE:
                nowstate=TABLE;
                break;
            case TABLE:
                if (*token=="table") {
                    nowstate=TABLE_NAME;
                }
                break;
            case TABLE_NAME:
                newtable.name=*token;
                nowstate=LEFT_BRACKET;
                break;
            case LEFT_BRACKET:
                if(*token=="(")
                    nowstate=COLUMN_NAME;
                break;
            case COLUMN_NAME:
                {
                    column newcolumn;
                    newcolumn.name=*token;
                    newtable.columns.push_back(newcolumn);
                    index++;
                    nowstate=COLUMN_TYPE;
                    break;
                }
            case COLUMN_TYPE:
                if (*token=="int") {
                    newtable.columns[index].type=INT;
                    newtable.columns[index].char_size=0;
                    newtable.columns[index].length=4;
                }
                else if(*token=="char")
                {
                    newtable.columns[index].type=CHAR;
                    ++token;
                    newtable.columns[index].char_size=atoi((*(++token)).c_str());
                    newtable.columns[index].length=newtable.columns[index].char_size;
                    ++token;
                }
                if (*(token+1)=="primary") {
                    nowstate=PRIMARY_KEY;
                }
                else if (*(token+1)==",")
                    nowstate=COMMA;
                else if(*(token+1)==")")
                    nowstate=RIGHT_BRACKET;
                break;
            case PRIMARY_KEY:
                if(*(token++)=="key")
                {
                    newtable.columns[index].primary=true;
                    ++token;
                }
                break;
            case COMMA:
                nowstate=COLUMN_NAME;
                break;
            case RIGHT_BRACKET:
                if (*(token+1)==";") {
                    nowstate=SEMICOLON;
                }
                break;
            case SEMICOLON:
                newtable.column_nums=newtable.columns.size();
                newtable.length=newtable.getlength();
                sail->api_createtable(newtable);
                break;
            default:
                error("intepretor","intepretor_createtable","syntax error");
                break;
        }
    }
    
}

void intepretor::intepretor_insert()
{
    vector<string> values;
    state nowstate=INSERT;
    string tablename;
    for (auto token=command.begin(); token!=command.end(); token++) {
        switch (nowstate) {
            case INSERT:
                if (*(token+1)=="into") {
                    nowstate=INTO;
                }
                break;
            case INTO:
                nowstate=TABLE_NAME;
                break;
            case TABLE_NAME:
                tablename=*token;
                if (*(token+1)=="values") {
                    nowstate=VALUES;
                }
                break;
            case VALUES:
                if(*(token+1)=="(")
                    nowstate=LEFT_BRACKET;
                break;
            case LEFT_BRACKET:
                nowstate=VALUE;
                break;
            case VALUE:
                if((*token)=="\""||(*token)=="'")
                {
                    token++;
                    values.push_back(*token);
                    token++;
                }
                else
                    values.push_back(*token);
                if(*(token+1)==")")
                    nowstate=RIGHT_BRACKET;
                else if(*(token+1)==",")
                    nowstate=COMMA;
                break;
            case COMMA:
                nowstate=VALUE;
                break;
            case RIGHT_BRACKET:
                if(*(token+1)==";")
                    nowstate=SEMICOLON;
                break;
            case SEMICOLON:
                {
                    table insert=sail->cataloginfo->catalog_gettable(tablename);
                    if (values.size()!=insert.column_nums) {
                        error("intepretor","intepretor_insert","column number not match");
                    }
                    record newrecord;
                    auto column_begin=insert.columns.begin();
                    auto value_begin=values.begin();
                    for (;column_begin!=insert.columns.end();column_begin++,value_begin++) {
                        if (column_begin->type==INT) {
                            value v(string2int(*value_begin));
                            newrecord.records.push_back(v);
                        }
                        else if(column_begin->type==CHAR)
                        {
                            value v(*value_begin,(*value_begin).length());
                            newrecord.records.push_back(v);
                        }
                    }
                    sail->api_insert(tablename, newrecord);
                }
                break;
            default:
                error("intepretor","intepretor_insert","syntax error");
                break;
        }
    }
}
