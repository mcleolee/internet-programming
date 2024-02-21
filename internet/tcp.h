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

// ================================================================
// ===================  声明  ======================================
// ================================================================


socklen_t serverAddrLength;
socklen_t clientAddrLength;
extern struct sockaddr_in serverAddr;
extern struct sockaddr_in clientAddr;

// ==================================================================
// ===================  结构体  ======================================
// ==================================================================

// in server.cpp
struct sockaddr_in serverAddr;

void initializeServerAddr(struct sockaddr_in& addr, socklen_t &serverAddrLength){
    // sin -> socket internet
    addr.sin_family = AF_INET; // IPV4
    addr.sin_port = htons(SERV_PORT); // 定义端口号8888，已转换主机字节序为网络字节序
    // addr.sin_addr.s_addr = inet_addr(INET_IP); //设置本机IP，已把地址转换为整数
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // 服务器将绑定到所有可用的网络接口上，而不是一个特定的IP地址
    serverAddrLength = sizeof(addr);
}

// int server.cpp

struct sockaddr_in clientAddr;

void initializeClientAddr(struct sockaddr_in& addr, socklen_t &clientAddrLength){
        memset(&addr, 0, sizeof(addr));
        clientAddrLength = sizeof(addr);
}


// ================================================================
// ===================  函数  ======================================
// ================================================================


class tool{
public:
    static void delay(int sec){this_thread::sleep_for(chrono::seconds(sec));} // delay 1s}
    static void timeExist(){static int temp;cout << "\ntime ticks "<< temp << "\n";temp++;} // 时间流动的证据 
    static int timeExistLoop(){static int temp;cout << "\ntime ticks "<< temp << "\n";temp++;return temp;} // 循环跳出
    static void clear(){system("clear");}
};

void printClientIP(sockaddr_in addr){
    // 客户端PORT是不是不太对!!
    cout<<"Client IP is "<<inet_ntoa(clientAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl;; // 不能输出自动识别的ip
}


void printServerIP(sockaddr_in addr){
    cout <<"Server IP is "<<inet_ntoa(serverAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl;;
}

// 自带一个\n
void printMessage(const char *msg){
    cout << msg << "\n";
}


int errorIfValueLessThan_0(const int value ,const char *msg){
    if(value < 0){
        perror(msg);
        cerr << "Error code: " << errno << endl;
        return -1;
    }else return value;
}

// 这函数有他妈的问题
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


// ================================================================
// =================  主要函数  ======================================
// ================================================================



// 1. 创建套接字
int createSocket(){
    // socket 函数返回值应该是3，因为0、1、2都被占用了
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //保存 socket 返回值 作为 监听套接字
    if(errorIfValueLessThan_0(sockfd, "Failed to create socket") != -1) 
    cout<<"- socket created! -\nSocket File Descriptor: "<<sockfd<<"\n\n";
    return sockfd;
}

//2. 绑定IP地址和端口号
void bindSocketToServer(int sockfd){
    int bindReturn = bind(sockfd, (struct sockaddr *)&serverAddr, static_cast<socklen_t>(serverAddrLength));
    if(errorIfValueLessThan_0(bindReturn, "Failed to bind") != -1){
        cout<<"- Bind successful -\n";
        printServerIP(serverAddr);
    }
}

//3. 设置监听套接字
void startListeningOnSocket(int sockfd){
    int listenReturn = listen(sockfd, 5);
    if(errorIfValueLessThan_0(listenReturn, "Failed to listen") != -1){
    cout<<"- Listen successful -\n";
    printClientIP(clientAddr);
    }
}

//3.5 客户端请求链接 -- CLIENT  
//connect
void clientConnect(int sockfd){
    int connectReturn = connect(sockfd, (struct sockaddr*)&serverAddr, serverAddrLength);
    if(errorIfValueLessThan_0(connectReturn, "Failed to connect") != -1){
    printMessage("- connect successful -\n");
    printServerIP(serverAddr);
    }
}

//4. 接受客户端链接
void acceptClientConnect(int &connectfd, int sockfd){
    connectfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLength);
    if(errorIfValueLessThan_0(connectfd, "Failed to connect and create a new connectfd") != -1)
    {cout<<"- Client connected -\n";
    printClientIP(clientAddr);}
}

//4.5 客户端数据发送 -- CLIENT
void clientWrite(int sockfd){
    char buffer[64] = {0};
    while(1){
        fgets(buffer, 64, stdin);
        buffer[strlen(buffer) - 1] = '\0'; // 把倒数第一个换成换行符？

        if(isQuit(buffer)) break;

        int writeReturn = write(sockfd, buffer, strlen(buffer));
        if(writeReturn < 0){
            perror("Failed to write");
            cerr << "Error code: " << errno << endl;
            // return -1;
        }else if(writeReturn == 0){
            cout<< "- write NULL, quiting now -\n";
            break;
        }else cout<< "- write successful! -\n";
    }
}

//5. 数据接受和发送 read
void readClientMessage(int connectfd){
        char buffer[64];
        while(1){
            memset(buffer, 0, sizeof(buffer));
            int readReturn = read(connectfd, buffer, 64);

            if(readReturn < 0){
                perror("Failed to read");
                cerr << "Error code: " << errno << endl;
                // return -1;
            }else if(readReturn == 0){
                printMessage("\n-- The client disconnected! --");
                printClientIP(clientAddr);
                break;
            }

            cout << "- read successful -\n"<<"recieved message ("<< readReturn <<" bytes):\n"<<buffer<<"\n\n";
            

            // if(errorIfValueLessThan_0(readReturn, "Failed to read buffer") != -1)
            // cout << "- read successful -\n"<<"recieved message ("<< readReturn <<" bytes):\n"<<buffer<<"\n\n";
            // else if(readReturn == 0){
            //     cout << "\n-- The client disconnected! --\n";
            //     cout<<"Client IP is "<<inet_ntoa(clientAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl; // 不能输出自动识别的ip
            //     break;
            // }
        }
}



void closeSocket(int sockfd){
    int i = close(sockfd);
    if(errorIfValueLessThan_0(i, "Failed to close socket") != -1)
    cout << "- socket ("<< sockfd <<") closed! -\n";
    
}




// void printMessageAndServerIP(const char *msg){
//     cout << "- connect successful -\n" <<"server IP is "<<inet_ntoa(serverAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl;; // 不能输出自动识别的ip;
// }