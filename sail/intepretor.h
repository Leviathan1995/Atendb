//
//  intepretor.hpp
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
using namespace std;

class intepretor
{
public:
    vector<string> command;
    void cmd_input();
    void parse_cmd();
    void createtable();
    void createindex();
    void select();
    void insert();
};

#endif /* intepretor_h */
