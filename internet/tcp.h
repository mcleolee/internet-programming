#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// from atc project
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>  // 用于使用系统命令的头文件

// time
#include <chrono>
#include <thread>

// 实现控制台输入检测
#include <ncurses.h>

#include <sqlite3.h>

// #include <graphics.h>
// #include "sqlite_modern_cpp.h"



//宏：INADDR_ANY 是自动帮你找一个当前可用的任意IP
#define SERV_IP  "0.0.0.0"    
#define SERV_PORT 6969
#define INET_IP "192.168.1.20"

using namespace std;

// ==================================================================



// in client.cpp
extern struct sockaddr_in serverAddr;

// struct sockaddr_in serverAddr;
//     {
//         serverAddr.sin_family = AF_INET;
//         serverAddr.sin_port   = htons(SERV_PORT);
//         serverAddr.sin_addr.s_addr = inet_addr(INET_IP);
//     };


// in server.cpp
struct sockaddr_in serverAddr;

void initializeServerAddr(struct sockaddr_in& addr){
    // sin -> socket internet
    addr.sin_family = AF_INET; // IPV4
    addr.sin_port = htons(SERV_PORT); // 定义端口号8888，已转换主机字节序为网络字节序
    serverAddr.sin_addr.s_addr = inet_addr(INET_IP); //设置本机IP，已把地址转换为整数
    // addr.sin_addr.s_addr = htonl(INADDR_ANY); // 服务器将绑定到所有可用的网络接口上，而不是一个特定的IP地址
}

// int server.cpp
extern struct sockaddr_in clientAddr;

struct sockaddr_in clientAddr;

void initializeClientAddr(struct sockaddr_in& addr){
        memset(&addr, 0, sizeof(addr));
}
// ==================================================================


class Tool{
public:
    static void delay(int sec){this_thread::sleep_for(chrono::seconds(sec));} // delay 1s}
    static void timeExist(){static int temp;cout << "\ntime ticks "<< temp << "\n";temp++;} // 时间流动的证据 
    static int timeExistLoop(){static int temp;cout << "\ntime ticks "<< temp << "\n";temp++;return temp;} // 循环跳出
    static void clear(){system("clear");}
};

int errorIfValueLessThan_0(const int value ,const char *msg){
    if(value < 0){
        perror(msg);
        cerr << "Error code: " << errno << endl;
        return -1;
    }else return value;
}


int createSocket(){
    // 创建套接字
    // socket 函数返回值应该是3，因为0、1、2都被占用了
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //保存 socket 返回值 作为 监听套接字
    if(errorIfValueLessThan_0(sockfd, "Failed to create socket") != -1) 
    cout<<"- socket created! -\nSocket File Descriptor: "<<sockfd<<"\n\n";
    return sockfd;
}

void closeSocket(int sockfd){
    int i = close(sockfd);
    if(errorIfValueLessThan_0(i, "Failed to close socket") != -1)
    cout << "- socket ("<< sockfd <<") closed! -\n";
    
}

int quitIfTypeQuit(){
    char buffer[64] = {0};
    fgets(buffer, 64, stdin);
    if(strncmp(buffer, "quit", 4) == 0) return 1;
    else return 0;
}

int isQuit(char buffer[]){
    if(strncmp(buffer, "quit", 4) == 0) return 1;
    else return 0;
}


// void printClientIP(){
//     // 客户端IP和PORT是不是不太对
//     "Client IP is "<<inet_ntoa(serverAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl;; 
// }


// void printServerIP(){
//     "Server IP is "<<inet_ntoa(clientAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl;;
// }

// void printMessageAndServerIP(const char *msg){
//     cout << "- connect successful -\n" <<"server IP is "<<inet_ntoa(serverAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl;; // 不能输出自动识别的ip;
// }