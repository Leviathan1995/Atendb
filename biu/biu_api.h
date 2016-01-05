//
//  biu.h
//  Biubitcask
//
//  Created by Leviathan on 15/12/23.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#ifndef _BIU_API_H
#define _BIU_API_H

#include <iostream>
#include <vector>
#include <string>
#include "bitcask.h"
using namespace std;

string biu_api(string req,bitcask &bit);
void exec(bitcask & bit,vector<string> cmd);
vector<string> intepretor(string req);


#endif /* _BIU_API_H */
