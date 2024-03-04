// MdSpi.h

#include "./CTP_API/TradeApi/ThostFtdcMdApi.h"
// #include "ThostFtdcMdApi.h"
#include <iostream>


class MdSpi : public CThostFtdcMdSpi{
public:
    
    // print
    static void printMsg(const char* msg);
    // 客户端和交易后台connect时调用（未登录）
    void OnFrontConnected();

    // void Release();
    // void Init();
    // int Join();
    // const char* GetTradingDay();
    // void RegisterFront(char* pszFrontAddress);
    // void RegisterNameServer(char* pszNsAddress);
    // void RegisterFensUserInfo(CThostFtdcFensUserInfoField* pFensUserInfo);
    // void RegisterSpi(CThostFtdcMdSpi* pSpi);
    // int SubscribeMarketData(char* ppInstrumentID[], int nCount);
    // int UnSubscribeMarketData(char* ppInstrumentID[], int nCount);
    // int SubscribeForQuoteRsp(char* ppInstrumentID[], int nCount);
    // int UnSubscribeForQuoteRsp(char* ppInstrumentID[], int nCount);
    // int ReqUserLogin(CThostFtdcReqUserLoginField* pReqUserLoginField, int nRequestID);
    // int ReqUserLogout(CThostFtdcUserLogoutField* pUserLogout, int nRequestID);
    // int ReqQryMulticastInstrument(CThostFtdcQryMulticastInstrumentField* pQryMulticastInstrument, int nRequestID);
private:
};

////////////// 实现  //////////////

void MdSpi::printMsg(const char *msg){std::cout << msg << "\n";}

void MdSpi::OnFrontConnected(){printMsg("已连接交易后台");}
