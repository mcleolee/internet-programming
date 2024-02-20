#include "tcp.h"

using namespace std;

int main(){
    Tool::clear();
//1. 创建套接字
    int sockfd = createSocket();
//2. 绑定IP地址和端口号
    // 定义成员
    initializeServerAddr(serverAddr);
    int saddr_length = sizeof(serverAddr);
    int bindReturn = bind(sockfd, (struct sockaddr *)&serverAddr, saddr_length);
    if(errorIfValueLessThan_0(bindReturn, "Failed to bind") != -1)
    cout<<"- Bind successful -\n"<<"INET is "<<inet_ntoa(serverAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl;; // 不能输出自动识别的ip
//3. 设置监听套接字
    int listenReturn = listen(sockfd, 5);
    if(errorIfValueLessThan_0(listenReturn, "Failed to listen") != -1)
    cout<<"- Listen successful -\n";

//4. 接受客户端链接
    initializeClientAddr(clientAddr);
    socklen_t clientAddrLength = sizeof(clientAddr);
    

//4.5 accept

    int connectfd = -2;

    while(1){ // 这个循环使得程序可以连接第二个客户端
        cout<<"waiting for a new client...\n\n";
        connectfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLength);
        if(errorIfValueLessThan_0(connectfd, "Failed to connect and create a new connectfd") != -1)
        cout<<"- Client connected -\n";
        cout<<"Client IP is "<<inet_ntoa(clientAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl;; // 不能输出自动识别的ip
        
//5. 数据接受和发送 read
        char buffer[64];
        while(1){
            memset(buffer, 0, sizeof(buffer));
            int readReturn = read(connectfd, buffer, 64);

            if(readReturn < 0){
                perror("Failed to read");
                cerr << "Error code: " << errno << endl;
                return -1;
            }else if(readReturn == 0){
                cout << "\n-- The client disconnected! --\n";
                cout<<"Client IP is "<<inet_ntoa(clientAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl; // 不能输出自动识别的ip
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


        closeSocket(connectfd);
    } // 循环在4.5处开始
    closeSocket(sockfd);
    
    cout << endl;
    return 0;
}