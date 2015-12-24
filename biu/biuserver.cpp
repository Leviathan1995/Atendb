//
//  biuserver.cpp
//  Biubitcask
//
//  Created by Leviathan on 15/12/22.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include <iostream>
#include <memory.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "biu_api.h"
using namespace std;

#define PORT 8888    					/*侦听端口地址*/
#define BACKLOG 2						/*侦听队列长度*/


/*服务器对客户端的处理*/
void process_conn_server(int s)
{
    ssize_t size = 0;
    char buffer[1024];				    //数据的缓冲区
    string response="oh!";
    for(;;){
        size = read(s, buffer, 1024);		    //从套接字中读取数据放到缓冲区buffer中
        if(size == 0){				    //没有数据
            return;
        }
        /*
         响应客户端
         */
        string req=string(buffer);
        response=biu_api(req);
        write(s,response.c_str(), response.length());/*发给客户端*/
    }
}

int main()
{
    int server_socket,client_socket;	//ss为服务器的socket描述符，cs为客户端的socket描述符
    sockaddr_in server_addr;	        //服务器地址结构
    sockaddr_in client_addr;	        //客户端地址结构
    int error;
    pid_t pid;				//分叉的thread ID
    
    //建立一个流式套接字
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket< 0){
        cout<<"socket error"<<endl;
        return -1;
    }
    
    /*设置服务器地址*/
    memset(&server_addr, sizeof(server_addr),0);			/*清零*/
    server_addr.sin_family = AF_INET;					/*协议族*/
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	/*本地地址*/
    server_addr.sin_port = htons(PORT);				/*服务器端口*/
    
    //绑定地址结构到套接字描述符
    //error =
    error=bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(error < 0){
        cout<<"bind error"<<endl;
        return -1;
    }
    /*设置侦听*/
    error = listen(server_socket, BACKLOG);
    if(error < 0){										/*出错*/
        cout<<"listen error"<<endl;
        return -1;
    }
    
    /*主循环过程*/
    for(;;)	{
        socklen_t addrlen = sizeof(struct sockaddr);
        
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addrlen);
        /*接收客户端连接*/
        if(client_socket < 0){							/*出错*/
            continue;						/*结束本次循环*/
        }
        
        /*建立一个新的进程处理到来的连接*/
        pid = fork();						/*分叉进程*/
        if( pid == 0 ){						/*子进程中*/
            process_conn_server(client_socket);		/*处理连接*/
            close(server_socket);						/*在子进程中关闭服务器的侦听*/
        }else{
            close(client_socket);						/*在父进程中关闭客户端的连接*/
        }
    }
    return 0;
}

