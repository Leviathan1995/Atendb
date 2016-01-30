//
//  biu_comm.h
//  Biu
//
//  Created by Leviathan on 16/1/5.
//  Copyright © 2016年 Leviathan. All rights reserved.
//

#ifndef BIU_COMM_H_
#define BIU_COMM_H_

#include <iostream>
using namespace std;
typedef unsigned char byte;


int read_cmd(byte *buff);
int write_cmd(byte *buff,int len);
int read_exact(byte *buff,int len);
int write_exact(byte *buff,int len);


#endif/* BIU_COMM_H_ */
