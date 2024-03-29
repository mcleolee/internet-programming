// // main.cpp

#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"

#include "MdSpi.h"
#include <iostream>

CThostFtdcMdApi *global_pMdUserApi = nullptr;  // 行情指针，但是，指针很危险，可以用栈对象
char MdFRONT_ADDR[] = "tcp://180.168.146.187:10131"; //行情前置地址

 
int main(){
    MdSpi::printMsg("\n== start ==\n");

    global_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi("",0,0); // 创建行情实例
    CThostFtdcMdSpi *pMdUserSpi = new MdSpi;    // 创建行情回调spi实例  // todo 虚函数
	
    global_pMdUserApi->RegisterSpi(pMdUserSpi); // 注册事件类
    global_pMdUserApi->RegisterFront((char*)MdFRONT_ADDR); // 行情前置，用tcp
    global_pMdUserApi->Init();

    // 线程推出
    global_pMdUserApi->Join();
    return 0;

}