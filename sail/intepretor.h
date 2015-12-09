//
//  intepretor.h
//  Sail
//
//  Created by Leviathan on 15/11/11.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#ifndef intepretor_h
#define intepretor_h

#include <iostream>
#include <vector>
#include <string>
#include "api.h"
#include "global.h"
#include "error.h"
using namespace std;

class api;
class intepretor
{
public:
    api * sail;
    vector<string> command;
    int string2int(string str);
    void cmd_input();
    void parse_cmd();
    void intepretor_execfile();
    void intepretor_createdb();
    void intepretor_createtable();
    void intepretor_createindex();
    void intepretor_select();
    void intepretor_insert();
};

#endif /* intepretor_h */
