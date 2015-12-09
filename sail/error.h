//
//  error.h
//  Sail
//
//  Created by Leviathan on 15/11/19.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#ifndef error_h
#define error_h

#include <iostream>
using namespace std;

class error
{
public:
    string module; //error module
    string function;//error function
    string message; //error message
    error()=default;
    error(string m,string f,string info):
        module(m),function(f),message(info){ cout<<"In module "<<module<<" "<<function<<" funciton "<<"\""<<message<<"\""<<endl;}
    
    
};
#endif /* error_h */
