//
//  biu.h
//  Biubitcask
//
//  Created by Leviathan on 15/12/23.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#ifndef biu_h
#define biu_h

#include <iostream>
#include <vector>
#include <string>
#include "bitcask.h"
using namespace std;

string biu_api(string req,bitcask &bit);
void exec(bitcask & bit,vector<string> cmd);
vector<string> intepretor(string req);


#endif /* biu_h */
