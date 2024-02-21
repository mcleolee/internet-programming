#include "tcp.h"

using namespace std;

// extern socklen_t serverAddrLength;
// extern socklen_t clientAddrLength;
// extern struct sockaddr_in serverAddr;
// extern struct sockaddr_in clientAddr;

int main(){
    tool::clear();
    int sockfd = createSocket();
    initializeServerAddr(serverAddr, serverAddrLength);
    clientConnect(sockfd);
    clientWrite(sockfd);


    closeSocket(sockfd);

    return 0;
}