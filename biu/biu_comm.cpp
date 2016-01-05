//
//  biu_comm.cpp
//  Biu
//
//  Created by Leviathan on 15/12/22.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include <iostream>
#include "biu_comm.h"
using namespace std;

int read_cmd(byte *buff)
{
  int len;

  if (read_exact(buff, 2) != 2)
    return(-1);
  len = (buff[0] << 8) | buff[1];
  return read_exact(buff, len);
}

int write_cmd(byte *buff, int len)
{
  byte li;

  li = (len >> 8) & 0xff;
  write_exact(&li, 1);
  
  li = len & 0xff;
  write_exact(&li, 1);

  return write_exact(buff, len);
}

int read_exact(byte *buff, int len)
{
  int i, got=0;

  do {
    if ((i = read(0, buff+got, len-got)) <= 0)
      return(i);
    got += i;
  } while (got<len);

  return(len);
}

int write_exact(byte *buff, int len)
{
  int i, wrote = 0;

  do {
    if ((i = write(1, buff+wrote, len-wrote)) <= 0)
      return (i);
    wrote += i;
  } while (wrote<len);

  return (len);
}
