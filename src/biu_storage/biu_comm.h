//
//  biu_comm.h
//  Biu
//
//  Created by Leviathan on 16/1/5.
//  Copyright © 2016年 Leviathan. All rights reserved.
//

#ifndef _BIU_COMM_H
#define _BIU_COMM_H

#include <iostream>
using namespace std;
typedef unsigned char byte;


int read_cmd(byte *buff);
int write_cmd(byte *buff,int len);
int read_exact(byte *buff,int len);
int write_exact(byte *buff,int len);


#endif/* _BIU_COMM_H */
