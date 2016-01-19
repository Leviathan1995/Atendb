//
//  biu_port.cpp
//  Biubitcask
//
//  Created by Leviathan on 15/12/22.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include <iostream>
#include <string>
#include "biu_comm.h"
#include "biu_api.h"
#include "bitcask.h"
using namespace std;
typedef unsigned char byte;

string bts(byte *buff,int length)
{
    string str;
    for (int index=1; index<length; index++) {
        str+=buff[index];
    }
    return str;
}

int stb(byte *buff,string response)
{
    for (int index=0; index<response.length(); index++) {
        buff[index]=response[index];
    }
    return response.length();
}

int main()
{
    int fn,length;
    string response;
    byte buff[1024];
    bitcask bit;
    while ((length=read_cmd(buff)) > 0)
    {
        fn = buff[0];
        string req=bts(buff,length);
        if (fn==1) {
            req="insert@"+req;
        }
        else if(fn==2)
        {
            req="update@"+req;
        }
        else if(fn==3)
        {
            req="delete@"+req;
        }
        else if(fn==4)
        {
            req="read@"+req;
        }
        else if(fn==5)
        {
            req="start@";
        }
        response=biu_api(req,bit);
        bit._response="";
        length=stb(buff,response);
        write_cmd(buff,length);
    }
    return 0;
}
