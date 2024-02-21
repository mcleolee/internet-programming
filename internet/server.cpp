#include "tcp.h"

using namespace std;

// extern socklen_t serverAddrLength;
// extern socklen_t clientAddrLength;
int connectfd = -2;

int main(){
    tool::clear();
    int sockfd = createSocket();

// 定义成员
    initializeServerAddr(serverAddr, serverAddrLength);
    initializeClientAddr(clientAddr, clientAddrLength); // 为什么可以在一开始就定义client的一切

    bindSocketToServer(sockfd);
    startListeningOnSocket(sockfd);
    // 这个循环使得程序可以连接第二个客户端
    while(1){ 
        printMessage("waiting for a new client...\n");
        acceptClientConnect(connectfd, sockfd);
        readClientMessage(connectfd);
        closeSocket(connectfd);
    } // 循环在4.5处开始

    closeSocket(sockfd);
    
    cout << endl;
    return 0;
}