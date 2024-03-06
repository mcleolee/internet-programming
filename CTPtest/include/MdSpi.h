// MdSpi.h

#include "ThostFtdcMdApi.h"
// #include "ThostFtdcMdApi.h"
#include <iostream>
#include <cstring>
#include <fstream>

// NOT SAFE
#define INVESTORID "223264" // do login function 
#define BROKERID   "9999"
#define USERID     "ravin"
#define PASSWORD   "TstwTdtct42!!"

extern CThostFtdcMdApi* global_pMdUserApi;


class MdSpi : public CThostFtdcMdSpi{
public:
    
    // print
    static void printMsg(const char* msg);
    static void printMsg(const char* msg, TThostFtdcDateType info);

    // 客户端和交易后台connect时调用（未登录）
    void OnFrontConnected();
    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void OnFrontDisconnected(int nReason);
	void OnHeartBeatWarning(int nTimeLapse);
private:
};

////////////// 实现  //////////////

void MdSpi::printMsg(const char *msg){std::cout << msg << "\n";} // use template to achieve multi-print
void MdSpi::printMsg(const char *msg, TThostFtdcDateType info){std::cout << msg << info << "\n";}


///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void MdSpi::OnFrontConnected(){
    printMsg("已连接交易后台");
    CThostFtdcReqUserLoginField loginReq;

    // do the type and login func
    strcpy(loginReq.BrokerID,BROKERID);
    strcpy(loginReq.UserID, USERID);
    strcpy(loginReq.Password, PASSWORD);
    static int reqID = 0;
    int result = global_pMdUserApi->ReqUserLogin(&loginReq, ++reqID);
    if(!result) printMsg("suceessfully sent login request");
    else std::cerr << "DID NOT send login request\n";
}

///登录请求响应
void MdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    bool bResult = pRspInfo && (pRspInfo->ErrorID != 0); // ???
    if(!bResult){
        std::cout<< "\nAccount login successful\nTrade Date: " << pRspUserLogin->TradingDay << std::endl;
        std::cout << "BrokerID: "<< pRspUserLogin->BrokerID << "UserID: "<<pRspUserLogin->UserID<<std::endl;

        char** ppInstrumentID_1 = new char*[50];
        char** ppInstrumentID_2 = new char*[50];
        ppInstrumentID_1[0] = "c2403";
        ppInstrumentID_2[0] = "m2403";
        global_pMdUserApi->SubscribeMarketData(ppInstrumentID_1, 1);
        global_pMdUserApi->SubscribeMarketData(ppInstrumentID_2, 1);
    }
    else std::cerr << "\nLogin in failed\tErrorID = " << pRspInfo->ErrorID << "\nError message: " << pRspInfo->ErrorMsg << std::endl;
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
void MdSpi::OnFrontDisconnected(int nReason){
    std::cerr << "Connection Disconnected\n";
    std::cerr << "Error code: " << nReason << std::endl;
}

///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void MdSpi::OnHeartBeatWarning(int nTimeLapse){
    std::cerr << "Network Heartbeat Warning\n";
    std::cerr << "Time to Last Connection: " <<  nTimeLapse << std::endl;
}

///订阅行情应答
void MdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast){
    bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
    if(!bResult) std::cout<< "Market data sub successful\nInstrument ID: " << pSpecificInstrument->InstrumentID << std::endl;
    else std::cerr << "\nMarket data sub failed\tErrorID = " << pRspInfo->ErrorID << "\nError message: " << pRspInfo->ErrorMsg << std::endl;
}

///深度行情通知
void MdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData){
    printMsg("\n====================Successful Recieve Depth Market Data=====================");
 
    //save to a csv file
    std::ofstream file("market_data.csv", std::ios::app);
    // Write column headers if file is empty
    if (file.tellp() == 0) { 
    file << "Instrument ID,Last price,Average price,Close price,Current Delta,Trade Volume\n";}
    file << "Instrument ID: " << pDepthMarketData->InstrumentID << ",";
    file << "Last price: " << pDepthMarketData->LastPrice << ",";
    file << "Average price: " << pDepthMarketData->AveragePrice << ",";
    file << "Close price: " << pDepthMarketData->ClosePrice << ",";
    file << "Current Delta: " << pDepthMarketData->CurrDelta << ",";
    file << "Trade Volume: " << pDepthMarketData->Volume << "\n";
    file.close();
    
    std::cout << "Instrument ID: " << pDepthMarketData->InstrumentID << "\t交易所代码： " << pDepthMarketData->ExchangeID << std::endl;
    // std::cout << "交易日： " << pDepthMarketData->TradingDay << std::endl;  // 交易要跨天吗？
    std::cout << "Last price: " << pDepthMarketData->LastPrice  << std::endl;
    std::cout << "Trade Volume: " << pDepthMarketData->Volume << std::endl;
}