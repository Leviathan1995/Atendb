//
//  biu.cpp
//  Biubitcask
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
    for(auto c:req)
    {
        string token;
        if((c>= 'a'&&c<'z') || (c>= 'A'&&c<= 'Z') || (c>= '0'&&c<='9'))
            token+=c;
        else if(c=='.'||c=='_')
            token+=c;
        else if(c==','||c=='('||c==')'||c=='"'||c=='\'')
        {
            if (token!="") {
                cmd.push_back(token);
                token="";
            }
            token=c;
            cmd.push_back(token);
            token="";
        }
    }
    return cmd;
}

string biu_api(string req)
{
    bitcask bit;
    bit.init();
    //bit.insert_data("shq", "lsy");
    //bit.insert_data("pp", "uu");
    /*
     for (int i=140; i<145; i++) {
     string d="shq"+to_string(i);
     string s="lsy"+to_string(i);
     bit.insert_data(d,s);
     }
     cout<<bit.response;
     //*/
    bit.read_datainfo("shq143");
    //bit.delete_data("shq141");
    //bit.update_data("shq20", "lsy i love you");
    return 0;
    string response;
    vector<string> cmd;
    cmd=intepretor(req);
    bitcask bit;
    bit.init();
    if ([](string cmd){return cmd=="insert";}(cmd[0]))
        bit.insert_data(cmd[1],cmd[2]);
    else if([](string cmd){return cmd=="update";}(cmd[0]))
        bit.update_data(cmd[1], cmd[2]);
    else if([](string cmd){return cmd=="delete";}(cmd[0]))
        bit.update_data(cmd[1], cmd[2]);
    else if([](string cmd){return cmd=="read";}(cmd[0]))
        bit.update_data(cmd[1], cmd[2]);
    return response;
}




