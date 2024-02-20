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
