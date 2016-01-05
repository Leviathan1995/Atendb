//
//  biu_api.cpp
//  Biu
//
//  Created by Leviathan on 15/12/22.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include <iostream>
#include <vector>
#include <string>
#include "bitcask.h"
using namespace std;

vector<string> intepretor(string req)
{
    vector<string> cmd;
    string token;
    for(auto c:req)
    {
        if((c>= 'a'&&c<'z') || (c>= 'A'&&c<= 'Z') || (c>= '0'&&c<='9'))
            token+=c;
        else if(c=='.'||c=='_')
            token+=c;
        else if(c=='@')
        {
            if (token!="") {
                cmd.push_back(token);
                token="";
            }
        }
    }
    if (token!="") {
        cmd.push_back(token);
        token="";
    }
    return cmd;
}

void exec(bitcask & bit,vector<string> cmd)
{
    if ([](string cmd){return cmd=="insert";}(cmd[0]))
        bit.insert_data(cmd[1],cmd[2]);
    else if([](string cmd){return cmd=="update";}(cmd[0]))
        bit.update_data(cmd[1], cmd[2]);
    else if([](string cmd){return cmd=="delete";}(cmd[0]))
        bit.delete_data(cmd[1]);
    else if([](string cmd){return cmd=="read";}(cmd[0]))
        bit.read_datainfo(cmd[1]);
}

string biu_api(string req,bitcask &bit)
{
    if (bit._start==false)
    {
        bit.init();
        vector<string> cmd;
        cmd=intepretor(req);
        exec(bit,cmd);
    }
    else
    {
        vector<string> cmd;
        cmd=intepretor(req);
        exec(bit,cmd);
    }
    return bit._response;
}

