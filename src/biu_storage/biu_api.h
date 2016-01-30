//
//  biu_api.h
//  Biu
//
//  Created by Leviathan on 15/12/23.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#ifndef BIU_API_H_
#define BIU_API_H_

#include <iostream>
#include <vector>
#include <string>
#include "bitcask.h"
using namespace std;

string biu_api(string req,bitcask &bit);
void exec(bitcask & bit,vector<string> cmd);
vector<string> intepretor(string req);


#endif /* BIU_API_H_ */
