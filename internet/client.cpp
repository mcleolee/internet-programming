#include "tcp.h"

using namespace std;

int main(){
    Tool::clear();
    int sockfd = createSocket();
//connect
    initializeServerAddr(serverAddr);
    int serverAddrLength = sizeof(serverAddr);
    int connectReturn = connect(sockfd, (struct sockaddr*)&serverAddr, serverAddrLength);
    if(errorIfValueLessThan_0(connectReturn, "Failed to connect") != -1)
    cout << "- connect successful -\n" <<"server IP is "<<inet_ntoa(serverAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl;; // 不能输出自动识别的ip;
//write 数据发送和接受
    char buffer[64] = {0};
    while(1){
        fgets(buffer, 64, stdin);
        buffer[strlen(buffer) - 1] = '\0'; // 把倒数第一个换成换行符？

        if(isQuit(buffer)) break;

        int writeReturn = write(sockfd, buffer, strlen(buffer));
        if(writeReturn < 0){
            perror("Failed to write");
            cerr << "Error code: " << errno << endl;
            return -1;
        }else if(writeReturn == 0){
            cout<< "- write NULL, quiting now -\n";
            break;
        }else cout<< "- write successful! -\n";
    }


    

    // close(sockfd);

    return 0;
}