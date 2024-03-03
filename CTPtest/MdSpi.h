#include "ThostFtdcMdApi.h"
#include <iostream>


class MdSpi : public CThostFtdcMdApi{
public:
    
    // print
    static void printMsg(const char* msg);
    // 客户端和交易后台connect时调用（未登录）
    void OnFrondConnected();
};

////////////// 实现  //////////////

void MdSpi::printMsg(const char *msg){std::cout << msg << "\n";}

void MdSpi::OnFrondConnected(){printMsg("已连接交易后台");}
