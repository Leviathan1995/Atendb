//
//  main.cpp
//  Sail
//
//  Created by Leviathan on 15/11/11.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include <iostream>
#include "api.h"
using namespace std;
int main()
{
    intepretor parse;
    cout<<"Welcome to sail dbms"<<endl;
    api sail(&parse);
    sail.parse->cmd_input();
}
