#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int main(int argc,const char *argv[])
{
    int listenfd; // 保存 socket 返回值 作为 监听套接字
    int connfd; // 用于保存 accept 处理后的返回值 作为通信套接字

    // create socket

    // PF_INET:IPV4 协议
    // SOCK_STREAM: TCP协议 流式套接字
    listenfd = socket(PF_INET,SOCK_STREAM,0); 
    if(-1 == listenfd)
    {
        perror("socket");
        return -1;
    }
    printf("socket is OK\n");
}