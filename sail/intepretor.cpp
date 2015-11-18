//
//  intepretor.cpp
//  Sail
//
//  Created by Leviathan on 15/11/11.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include "intepretor.h"
#include "global.h"
using namespace std;

void intepretor::cmd_input()
{
    command.clear();
    cout<<"->";
    string cmd;
    while (getline(cin, cmd))
    {
        auto end=cmd.rbegin();
        if(*end!=';')
        {
            string token;
            for(auto c_char:cmd)
            {
                if((c_char>= 'a'&&c_char<'z') || (c_char>= 'A'&&c_char<= 'Z') || (c_char>= '0'&&c_char<'9'))
                    token+=c_char;
                else
                {
                    command.push_back(token);
                    token="";
                }
            }
            cout<<"->";
        }
        else
            break;
    }
    if ([](string cmd){return cmd=="quit";}(command[0])) {
        return;
    }
    else
       parse_cmd();
}


void intepretor::parse_cmd()
{
    if([](string cmd1,string cmd2){return cmd1=="create"&&cmd2=="table";}(command[0],command[1]))
        createtable();
    /*
    else if([](string cmd1,string cmd2){return cmd1=="create"&&cmd2=="index";}(command[0],command[1]))
        createindex();
    else if([](string cmd){return  cmd=="select";}(command[0]))
        select();
    else if([](string cmd1,string cmd2){return cmd1=="insert"&&cmd2=="into";}(command[0],command[1]))
        insert();
     */
}

void intepretor::createtable()
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
            case LEFT_BRACKET:
                if(*token=="(")
                    nowstate=COLUMN_NAME;
                break;
            case COLUMN_NAME:
                {
                    column newcolumn;
                    newtable.columns.push_back(newcolumn);
                    index++;
                    break;
                }
            case COLUMN_TYPE:
                if (*token=="int") {
                    newtable.columns[index].type=INT;
                }
                else if(*token=="char")
                {
                    newtable.columns[index].type=CHAR;
                    token++;
                    newtable.columns[index].char_size=atoi((*token).c_str());
                }
                if (*(token++)=="primary") {
                    nowstate=PRIMARY_KEY;
                }
                else if (*(token++)==",")
                    nowstate=COMMA;
                else if(*(token++)==")")
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
            default:
                break;
        }
    }
    
}
