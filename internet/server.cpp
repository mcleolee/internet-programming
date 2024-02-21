#include "tcp.h"

using namespace std;

extern socklen_t serverAddrLength;
extern socklen_t clientAddrLength;
int connectfd = -2;

int main(){
    tool::clear();
//1. 创建套接字
    int sockfd = createSocket();

//2. 绑定IP地址和端口号
    // 定义成员
    initializeServerAddr(serverAddr, serverAddrLength);
    bindSocketToServer(sockfd);
    
//3. 设置监听套接字
    startListeningOnSocket(sockfd);

//4. 接受客户端链接
    initializeClientAddr(clientAddr, clientAddrLength);
    
//4.5 accept


    while(1){ // 这个循环使得程序可以连接第二个客户端
        printMessage("waiting for a new client...\n");
        acceptConnect(connectfd, sockfd);

//5. 数据接受和发送 read
        readClientMessage(connectfd);
        
        closeSocket(connectfd);
    } // 循环在4.5处开始
    closeSocket(sockfd);
    
    cout << endl;
    return 0;
}