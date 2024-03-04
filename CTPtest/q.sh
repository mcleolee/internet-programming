# q.sh
clear
echo '\n======================编译开始======================\n'

g++ main.cpp -o m -I./include #-L./lib -lLinuxDataCollect -lthostmduserapi_se -lthosttraderapi_se 
# -v 
    #  -ICTP_API/clientdatacollect

    # -Lv6.3.19_P1_20200106_api/v6.3.19_P1_20200106_api_clientdatacollectdll_linux64 \
    # -Lv6.3.19_P1_20200106_api/v6.3.19_P1_20200106_api_tradeapi_se_linux64 \



# rm a.out

# sudo cp /v6.3.19_P1_20200106_api/v6.3.19_P1_20200106_api_clientdatacollectdll_linux64/LinuxDataCollect.so /usr/lib/
# sudo cp /v6.3.19_P1_20200106_api/v6.3.19_P1_20200106_api_tradeapi_se_linux64/thostmduserapi_se.so /usr/lib/
# sudo cp /v6.3.19_P1_20200106_api/v6.3.19_P1_20200106_api_tradeapi_se_linux64/thosttraderapi_se.so /usr/lib/


echo '\n======================编译完成======================\n'

# ./ser

