#include <stdio.h>
#include <string.h>
#include <sys/socket.h> //man 3 socket 
#include <sys/socket.h>  //man 7 ip 
#include <netinet/in.h> 
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>   // man  3 inet_addr
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>  //man 3 toupper 

#define SERV_IP  "0.0.0.0"    //INADDR_ANY
#define SERV_PORT 6666

#define SIZE    1024

void myprint(char *s);

//调格式：  小 gg=GG 

int server_init(char *ip, short port, int backlog)
{
    //创建套结字
    //填充ip 等信息
    //bind 
    //listen
    
    //return listenfd; 
}


int main(int argc,const char *argv[])
{
    char recvbuf[SIZE] = {0};
    int listenfd;  //保存 socket 返回值 作为 监听套结字
    int connfd  ; //用于保存 accept 处理后的返回值 作为通信套结字

    //创建套结字
    // PF_INET:IPV4协议 SOCK_STREAM：TCP协议 流式套结字 
    listenfd = socket(PF_INET, SOCK_STREAM, 0);  
    if(-1 == listenfd)
    {
        perror("socket");
        return -1;
    }
    printf("socket %d ok\n", listenfd); 
    //填充ip、端口、协议等信息
    #if 0
    struct sockaddr_in  ser_addr;                       //保存 服务器的info
    memset(&ser_addr, 0, sizeof(ser_addr)); 
    ser_addr.sin_family      =  PF_INET;                //IPV4 协议
    ser_addr.sin_port        =  htons(6666);            //端口号范围：1024-49151
    ser_addr.sin_addr.s_addr =  inet_addr("127.0.0.1"); //转化ip  
    #else 
    struct sockaddr_in ser_addr = {
        .sin_family =  PF_INET,
        .sin_port   =  htons(SERV_PORT),        //SERV_PORT 前边有宏定义  6666
        .sin_addr.s_addr = inet_addr(SERV_IP)   //SERV_IP   前边宏定义    "0"
        //.sin_addr.s_addr = inet_addr(argv[1])   //用命令行参数  运行时 输入ip ： ./a.out  127.0.0.1 
    }; 
    #endif

    //优化2：设置套结字重用 
    int opt = 1; 
    int m  = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(-1 == m )
    {
        perror("setsockopt");
        return -1;
    }
    //绑定ip和端口等信息
    int ret = bind(listenfd,(struct sockaddr * ) &ser_addr, sizeof(ser_addr)); 
    if(-1 == ret )
    {
        perror("bind");
        return -1;
    }
    printf("bind ok\n"); 

    //建立监听

    ret = listen(listenfd, 8); 
    if(-1 == ret)
    {
        perror("listen");
        return -1;
    }
    printf("等待客户端 连接...\n");


    //处理客户端连接请求  相当于前台
    //当accept 处理之后， 监听套结字转接 通信套结字
    //优化1：加一个循环：客户端退出，服务器继续保持监听
    while(1){

#if 0
        connfd =  accept( listenfd , NULL, NULL);  //不关心客户端的ip和端口
#else
        //优化3：关心客户端ip和端口等
        struct sockaddr_in caddr ;  //定义一个结构体变量保存客户端的ip等信息 
        memset(&caddr, 0, sizeof(caddr));
        socklen_t  clen = sizeof(caddr);  //socklen_t -> int 
        connfd = accept(listenfd, (struct sockaddr*)&caddr, &clen); 
        if(-1 == connfd)
        {
            perror("accept");
            return -1;
        }
        printf("客户端（%s:%d) 连接成功!\n",inet_ntoa( caddr.sin_addr), ntohs(caddr.sin_port));
#endif
        printf("connfd=%d\n",connfd); 
        //正常通信
        while(1)
        {
            int  count = -1; 
            memset(recvbuf, 0, sizeof(recvbuf)); 
            count = read(connfd, recvbuf, sizeof(recvbuf));
            if(-1 == count)
            {
                perror("read");
                close(connfd); 
                return -1;
            }
            else if(0 == count)
            {
                printf("client had quit!\n");
                close(connfd);
                break; 
            }
            //正常通信
            printf("recv:%s\n",recvbuf); 
            if(strncmp(recvbuf, "sl", 2) == 0)
            {
                system("sl"); 
            }
            else if(strncmp(recvbuf, "goo", 3) == 0)
            {
                myprint("hello man\n"); 
            }
            //将接收到的数据转化为 大写
            int i ;
            for(i=0; i<count; i++)
            {
              recvbuf[i] = toupper(recvbuf[i]);  //将recvbuf里面的字符挨个转化为大写 然后放到recvbuf
            }

            write(connfd, recvbuf, count);
        }

        //关闭 通信套结字
      //  close(connfd);  //read 为 0 时 关闭
    }
close(listenfd); //关闭监听套结字
return 0;
}

void myprint(char *s)
{
    printf("%s", s); 
}
