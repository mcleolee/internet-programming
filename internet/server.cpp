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

int main(){
    Tool::clear();
//1. 创建套接字
    int sockfd = createSocket();
//2. 绑定IP地址和端口号
    
struct sockaddr_in serverAddr;
    {
        // 定义成员
        // sin -> socket internet
        serverAddr.sin_family = AF_INET; // IPV4
        serverAddr.sin_port = htons(SERV_PORT); // 定义端口号8888，已转换主机字节序为网络字节序
        // serverAddr.sin_addr.s_addr = inet_addr(INET_IP); //设置本机IP，已把地址转换为整数
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 服务器将绑定到所有可用的网络接口上，而不是一个特定的IP地址
    };
    
    int saddr_length = sizeof(serverAddr);
    int bindReturn = bind(sockfd, (struct sockaddr *)&serverAddr, saddr_length);
    if(errorIfValueLessThan_0(bindReturn, "Failed to bind") != -1)
    cout<<"- Bind successful -\n"<<"INET is "<<inet_ntoa(serverAddr.sin_addr)<<" / "<<SERV_PORT<<endl<<endl;; // 不能输出自动识别的ip
//3. 设置监听套接字
    int listenReturn = listen(sockfd, 5);
    if(errorIfValueLessThan_0(listenReturn, "Failed to listen") != -1)
    cout<<"- Listen successful -\n";

//4. 接受客户端链接
    struct sockaddr_in clientAddr;
    {
        memset(&clientAddr, 0, sizeof(clientAddr));
    }
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