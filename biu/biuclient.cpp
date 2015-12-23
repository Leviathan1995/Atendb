//
//  biuclient.cpp
//  Biubitcask
//
//  Created by Leviathan on 15/12/22.
//  Copyright © 2015年 Leviathan. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;
#define PORT 8888    							/*侦听端口地址*/

/*客户端的处理过程*/
void process_conn_client(int socket)
{
    ssize_t size = 0;
    char buffer[1024];							/*数据的缓冲区*/
    
    for(;;){									/*循环处理过程*/
        /*从标准输入中读取数据放到缓冲区buffer中*/
        size = read(0, buffer, 1024);
        if(size > 0){							/*读到数据*/
            write(socket, buffer, size);				/*发送给服务器*/
            size = read(socket, buffer, 1024);		/*从服务器读取数据*/
            write(1, buffer, size);				/*写到标准输出*/
        }
    }
}

int main()
{
    int client_socket;										/*s为socket描述符*/
    sockaddr_in server_addr;			/*服务器地址结构*/
    string ip_addr="127.0.0.1";
    client_socket= socket(AF_INET, SOCK_STREAM, 0); 		/*建立一个流式套接字 */
    if(socket < 0){									/*出错*/
        cout<<"socket error"<<endl;
        return -1;
    }
    
    /*设置服务器地址*/
    bzero(&server_addr, sizeof(server_addr));	/*清零*/
    server_addr.sin_family = AF_INET;					/*协议族*/
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	/*本地地址*/
    server_addr.sin_port = htons(PORT);				/*服务器端口*/
    
    /*将用户输入的字符串类型的IP地址转为整型*/
    inet_pton(AF_INET, ip_addr.c_str(), &server_addr.sin_addr);
    /*连接服务器*/
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    process_conn_client(client_socket);						/*客户端处理过程*/
    close(client_socket);									/*关闭连接*/
    return 0;
}
