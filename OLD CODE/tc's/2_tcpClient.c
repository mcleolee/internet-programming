#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
 #include <unistd.h>
 #include <string.h>

#define SIZE 1024
int main(int argc, char **argv)
{
#if 0
    if(3 != argc)
    {
        printf("Usage: <%s> <IP> <PORT>\n", argv[0]);
        return -1;
    }
#endif 
   //创建通信套结字 ：注意：客户端 没有监听，创建的套结字直接就是通信套结字

    char buf[SIZE] ={0};
    int connfd = socket(PF_INET, SOCK_STREAM, 0);
    if(-1 == connfd)
    {
        perror("socket");
        return -1;
    }
    //以下填充的是 要连接的服务器ip 和端口等信息
#if 1
    struct sockaddr_in serveraddr = {
    .sin_family = PF_INET,
    .sin_addr.s_addr = inet_addr("127.0.0.1"),
    .sin_port = htons(6666)
};
#else 
    struct sockaddr_in serveraddr = {0};
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons( atoi(argv[2]) );
#endif 
    //连接服务器
    int len = sizeof(serveraddr);
    if(-1 == connect(connfd, (struct sockaddr*)&serveraddr, len) ) //主动连接服务器
    {
        perror("connect");
        return -1;
    }
    //通信
    while(1)
    {
        fgets(buf,sizeof(buf),stdin );
    
        write(connfd, buf, strlen(buf));
        //sleep(1);
        read(connfd, buf,sizeof(buf));
        
        printf("%s\n",buf);
        // write(STDOUT_FILENO, buf,strlen(buf)); //等同于 printf 输出到屏幕
    }
    //关闭套结字
    close(connfd);
}

