[HERE IS NOTION PAGE](https://doria42.notion.site/CTP-1-f52960064a534133b47d0a4120f028cc?pvs=4)

思维导图在文件夹中,其中包含了我遇到的大多数问题和解决办法,文件名是 CTP.xmind


---

# CTP 测试任务报告

## 1. 项目概述和结构

使用simnow提供的7*24 CTP仿真交易环境，进行指定期货品种行情接收和存储，需要在linux环境下使用C++完成。


结构如下：
```shell
.
├── CTP_API                        ctp api的6.6.1版本
│   ├── clientdatacollect
│   │   ├── DataCollect.h
│   │   └── LinuxDataCollect.so
│   └── TradeApi
│       ├── error.dtd
│       ├── error.xml
│       ├── libthostmduserapi_se.so
│       ├── libthosttraderapi_se.so
│       ├── ThostFtdcMdApi.h
│       ├── ThostFtdcTraderApi.h
│       ├── ThostFtdcUserApiDataType.h
│       └── ThostFtdcUserApiStruct.h
├── include                         所有.h的头文件
│   ├── DataCollect.h
│   ├── MdSpi.h
│   ├── ThostFtdcMdApi.h
│   ├── ThostFtdcTraderApi.h
│   ├── ThostFtdcUserApiDataType.h
│   └── ThostFtdcUserApiStruct.h
├── lib                             所有.so的动态连接库
│   ├── libLinuxDataCollect.so
│   ├── libthostmduserapi_se.so
│   └── libthosttraderapi_se.so
├── CTP.xmind                       完成测试过程纪要，用思维导图呈现
├── 6.7.2_API接口说明.chm           
├── readme.md                       测试的文档
├── main.cpp                        主程序
├── q.sh                            运行这个脚本可以快速编译
├── market_data.csv                 保存在本地的深度行情tick数据
├── QueryRsp.con                    
├── DialogRsp.con
└── TradingDay.con
```



## 2. 测试环境

- 操作系统：Linux Redhat 9
- 编译器：g++
- CTP版本：6.6.1

## 3. 测试目标

- 使用simnow提供的第二套测试环境进行测试
- 确保与交易后台建立通信连接的功能正常。
- 确保用户登录功能正常。
- 确保订阅行情数据功能正常。
- 订阅玉米（c2403），豆粕（m2403）两个合约的tick行情
- 确保接收行情数据功能正常。
- 将接收到的tick数据存储至本地，建议以csv文件存储，并添加上表头。

## 4. 测试用例

| 测试编号 | 测试步骤                                     | 预期结果                                                     | 实际结果                                                     | 测试通过 |
|----------|----------------------------------------------|--------------------------------------------------------------|--------------------------------------------------------------|----------|
| T01    | 连接交易后台   | 成功连接交易后台 | 实现功能  | 是  |
| T02    | 用户登录  | 成功登录，并获取到正确的交易日期、BrokerID和UserID | 实现功能，获取到正确的信息 | 是       |
| T03 | 订阅行情数据 | 成功订阅指定合约的行情数据|实现功能，成功订阅| 是       |
|T04| 接收行情数据|成功接收到行情数据|实现功能，成功接收并打印行情数据| 是 |
| T05| 连接交易后台（异常情况：无网络连接）  | 连接失败，提示网络连接失败  | 实现功能，成功处理异常情况 | 是 |
| T06    | 用户登录（异常情况：错误的用户名密码） | 登录失败，提示用户名或密码错误 | 实现功能，成功处理异常情况 | 是       |
| T07    | 订阅行情数据（异常情况：合约不存在）| 订阅失败，提示合约不存在 | 实现功能，成功处理异常情况 | 是  |
|T08|输出接受到的数据为csv文件|成功输出为文件|实现功能，可以无误输出|是

## 5. 测试结果

共进行了8个测试，全部通过。详细测试结果如下：

- 成功通过的测试用例：8个
- 失败的测试用例：0个

