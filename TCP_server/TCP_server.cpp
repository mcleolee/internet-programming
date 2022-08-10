#include <stdio.h>
#include <string.h>
#include <sys/socket.h> // man 3 socket

#include <sys/socket.h> // man 3 socket 
    #include <netinet/in.h>
        #include <netinet/ip.h> //

#include <sys/socket.h> // 
    #include <netinet/in.h> 

#include <sys/types.h>
#include <arpa/inet.h>

#include <unistd.h>


// 可以写宏来替换下面的端口PORT和地址IP


int main(int argc,const char *argv[])
{
    int listenfd; // 保存 socket 返回值 作为 监听套接字
    int connfd; // 用于保存 accept 处理后的返回值 作为通信套接字
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //         创建套接字
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    // PF_INET:IPV4 协议
    // SOCK_STREAM: TCP协议 流式套接字
    listenfd = socket(PF_INET,SOCK_STREAM,0); 
    if(-1 == listenfd)
    {
        perror("socket");
        return -1;
    }
    printf("socket is OK\n");
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //    填充ip,端口,协议等信息
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
#if 0 // 写法 1
    struct sockaddr_in ser_addr; // 保存服务器的 info
    memset(&ser_addr, 0 , sizeof(ser_addr));
    ser_addr.sin_family     = PF_INET; // IPV4
    ser_addr.sin_port       = htons(6666); //端口号 1024-49151
    ser_addr.sin_addr.s_addr= inet_addr("127.0.0.1"); // 转化 ip
#else // 写法 2
    struct sockaddr_in ser_addr = 
    {
        .sin_family     = PF_INET,
        .sin_port       = htons(6666), // SERV_PORT
        .sin_addr.s_addr= inet_addr("127.0.0.1") // SERV_IP
    };
#endif
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //     绑定 IP 和端口等信息
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    int ret = bind(listenfd, (struct sockaddr *) &ser_addr, sizeof(ser_addr));
    if(-1 == ret)
    {
        perror("bind");
        return -1;
    }
    printf("bind is OK\n");
    
    
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //           建立监听
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    ret = listen(listenfd, 8);
    if(-1 == ret)
    {
        perror("listen");
        return -1;
    }
    printf("等待客户端链接...\n"); 

    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //      处理客户端连接请求
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // 相当于前台
    // 当 accept 处理之后,监听套接字转接通信套接字
    connfd = accept(listenfd, NULL,NULL); // 不关心客户端的 IP 和端口
    if(-1 == connfd)
    {
        perror("accept");
        return -1;
    }
    printf("客户端链接成功！\n"); // 模拟发起客户端：nc 127.0.0.1 6666

    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //           正常通信
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    while(1)
    {
        int count = -1;
        memset(recvbuf, 0 , sizeof(recvbuf));
        count = read(connfd, recvbuf, sizeof(recvbuf));
        if(-1 == count)
        {
            perror("read");
            return -1;
        }
        else if (0 == count)
        {
            printf("client had quit!\n");
            break;
        }
        printf("recv:%s\n",recvbuf);
        
    }


    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    //          关闭套接字
    // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

}