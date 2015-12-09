//
//  main.cpp
//  Sail
//
//  Created by Leviathan on 15/11/11.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "api.h"
#include "buffer.h"
#include "catalog.h"
#include "record_manager.h"
using namespace std;
int main()
{
    /*
        test
    fstream file;
    char data2[4096];
    file.open("execute /Users/Leviathan/Documents/Xcode_Sail/Sail/Sail/stu.d",ios::binary|ios::out);
    file.read(data2, 4096);
    for (auto asc:data2) {
        cout<<asc;
    }
    file.close();
    */
    intepretor parseinfo;
    catalog cataloginfo;
    buffer bufferinfo;
    record_manager recordinfo;
    cout<<"Welcome to sail dbms"<<endl;
    api sail(&parseinfo,&cataloginfo,&bufferinfo,&recordinfo);
    parseinfo.sail=&sail;
    sail.parseinfo->cmd_input();
    return 0;
}
