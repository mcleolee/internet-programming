#include "tcp.h"

using namespace std;

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
    }else return 0;
}

int createSocket(){
    // 创建套接字
    // socket 函数返回值应该是3，因为0、1、2都被占用了
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //保存 socket 返回值 作为 监听套接字
    if(errorIfValueLessThan_0(sockfd, "Failed to create socket") != -1) 
    cout<<"- socket created! -\nSocket File Descriptor: "<<sockfd<<"\n\n";
    return sockfd;
}

int isQuit(char buffer[]){
    if(strncmp(buffer, "quit", 4) == 0) return 1;
    else return 0;
}

int main(){
    Tool::clear();
    int sockfd = createSocket();
//connect
    struct sockaddr_in serverAddr;
    {
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port   = htons(SERV_PORT);
        serverAddr.sin_addr.s_addr = inet_addr(INET_IP);
    };
    int serverAddrLength = sizeof(serverAddr);
    int connectReturn = connect(sockfd, (struct sockaddr*)&serverAddr, serverAddrLength);
    if(errorIfValueLessThan_0(connectReturn, "Failed to connect") != -1)
    cout << "- connect successful -\n"<<"server IP is "<<inet_ntoa(serverAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl;; // 不能输出自动识别的ip;
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