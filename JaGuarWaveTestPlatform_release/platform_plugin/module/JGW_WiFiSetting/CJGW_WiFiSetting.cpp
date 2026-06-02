#include "StdAfx.h"
#include "CJGW_WiFiSetting.h"
#include "JSON/CJGW_WiFiServerJsonCommand.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_WiFiSetting::CCJGW_WiFiSetting(void) : mnSocketPort(3001),mnTimeOut(10),mstrSocketIP("127.0.0.1"),mbIsLoginMES(false)
    {
        mRecvBuf = (char*)calloc(MES_MAX_RECV_BUFFER_SIZE,1);
    }


    CCJGW_WiFiSetting::~CCJGW_WiFiSetting(void)
    {
        free(mRecvBuf);
    }

    void CCJGW_WiFiSetting::DisConnectSocket()
    {
        mWiFiServerSocket.CleanupSocket();
    }

    void CCJGW_WiFiSetting::SetWiFiServerInfo(const std::string& strSocketIP,const size_t socketPort,const size_t timeOutSec,const std::string& strSourceIP /* = "" */)
    {
        mnTimeOut = timeOutSec;
        if (0 == mstrSocketIP.compare(strSocketIP) && socketPort == mnSocketPort) return ;
        mstrSocketIP = strSocketIP;
        mnSocketPort = socketPort;
        mstrSourceSocketIP = strSourceIP;
        mWiFiServerSocket.CleanupSocket();
    }

    void CCJGW_WiFiSetting::GetWiFiServerInfo(std::string& strSocketIP,size_t& socketPort)
    {
        strSocketIP = mstrSocketIP;
        socketPort = mnSocketPort;
    }

    std::wstring& CCJGW_WiFiSetting::GetErrorMsg()
    {
        return mstrErrorMsg;
    }

    bool CCJGW_WiFiSetting::CheckConnectWiFiServiceSocket()
    {
        if (!mWiFiServerSocket.CheckSocketIsConnected())
        {
            PDLOG4WW(GetString(E_WIFI_CONNECTING_DATA_SERVICE));
            if (!mWiFiServerSocket.ConnectSocketTimeOut(
                mstrSocketIP,
                mnSocketPort,
                mnTimeOut * 1000,mnTimeOut * 1000,mstrSourceSocketIP.empty() ? NULL : mstrSourceSocketIP.c_str()))
            {
                PELOG4WW(GetString(E_WIFI_CONNECT_FAIL_CHECK_SERVICE_IS_RUNING));
                mbIsLoginMES = false;
                return false;
            }
            PDLOG4WW(GetString(E_WIFI_CONNECT_SUCCESSFULLY));
        }
        return true;
    }

    bool CCJGW_WiFiSetting::ScanWiFi(std::vector<std::string>& vstrSSID)
    {
        return ExecuteWiFiSetting_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
            if (!ReadJsonSocketToWriteSocket(CCJGW_WiFiServerJsonCommand::GetScanWiFiJson(),E_SCAN_WIFI,ftmJson,mnTimeOut)) return false;
            ftmJson.GetJsonValueToKey("ssidArray",vstrSSID);
            return true;
        });
    }

    bool CCJGW_WiFiSetting::ConnectWiFi(const std::string& strSSID,const std::string& strPassword)
    {
        return ExecuteWiFiSetting_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
            if (!ReadJsonSocketToWriteSocket(CCJGW_WiFiServerJsonCommand::GetConnectWiFiJson(strSSID,strPassword),E_CONNECT_WIFI,ftmJson,mnTimeOut)) return false;
            return true;
        });
    }

    void CCJGW_WiFiSetting::DisConnectWiFi()
    {
        ExecuteWiFiSetting_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
            if (!ReadJsonSocketToWriteSocket(CCJGW_WiFiServerJsonCommand::GetDisConnectWiFiJson(),E_DISCONNECT_WIFI,ftmJson,mnTimeOut)) return false;
            return true;
        });
    }

    bool CCJGW_WiFiSetting::ConnectPerasoDevice()
    {
        return ExecuteWiFiSetting_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
            if (!ReadJsonSocketToWriteSocket(CCJGW_WiFiServerJsonCommand::GetConnectPerasoDeviceJson(),E_CONNECT_PUMAS,ftmJson,mnTimeOut)) return false;
            return true;
        });
    }

    bool CCJGW_WiFiSetting::CheckConnectWiFi(const std::string& strSSID,const std::string& strPassword,const std::string& strWlanGateWay,const std::string& strLocalAddress,const std::string& strInterfaceDescription,std::string& strWlanAddress)
    {
        return ExecuteWiFiSetting_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
            if (!ReadJsonSocketToWriteSocket(CCJGW_WiFiServerJsonCommand::GetCheckConnectWiFiJson(strSSID,strPassword,strWlanGateWay,strLocalAddress,strInterfaceDescription),E_CHECKCONNECT_WIFI,ftmJson,mnTimeOut)) return false;
            ftmJson.GetJsonValueToKey("wlanAddress",strWlanAddress);
            return true;
        });
    }

    bool CCJGW_WiFiSetting::WriteReadPerasoCommad(const std::string& strWrite,std::string& strRead)
    {
        return ExecuteWiFiSetting_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
            if (!ReadJsonSocketToWriteSocket(CCJGW_WiFiServerJsonCommand::GetWriteReadPerasoDeviceJson(strWrite),E_COMMAND_PUMAS,ftmJson,mnTimeOut)) return false;
            ftmJson.GetJsonValueToKey<std::string>("returnMessage",strRead);
            return true;
        });
    }

    bool CCJGW_WiFiSetting::ShellExecuteCommad(const std::string& command)
    {
        return ExecuteWiFiSetting_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
            if (!ReadJsonSocketToWriteSocket(CCJGW_WiFiServerJsonCommand::GetShellExecuteJson(command),E_SHELL_EXECUTE,ftmJson,mnTimeOut)) return false;
            //! ftmJson.GetJsonValueToKey<std::string>("returnMessage",strRead);
            return true;
        });
    }

    void CCJGW_WiFiSetting::DisConnectPerasoDevice()
    {
        ExecuteWiFiSetting_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
            if (!ReadJsonSocketToWriteSocket(CCJGW_WiFiServerJsonCommand::GetDisConnectPerasoDeviceJson(),E_DISCONNECT_PUMAS,ftmJson,mnTimeOut)) return false;
            return true;
        });
    }

    bool CCJGW_WiFiSetting::WriteJsonSocket(const std::string& strWriteJson,size_t timeOutSec /* = 10 */,int retryCount /* = 3 */)
    {
        for (int i = 0;i < retryCount;i ++)
        {
            if(mWiFiServerSocket.AsyncWriteSocket(strWriteJson)) return true;
        }
        mstrErrorMsg = L"Send MES Service Command Fail";
        return false;
    }

    bool CCJGW_WiFiSetting::ReadJsonSocketToWriteSocket(const std::string& strWriteJson,size_t nReqCode,CCJGW_FTM_Json& ftmJson,size_t timeOutSec /* = 10 */,int retryCount /* = 3 */)
    {
        std::stringstream msg_ss;
        int nRsponseCode = -1;
        //! 写入的JSON为空则直接读
        if (!WriteJsonSocket(strWriteJson,timeOutSec,retryCount)) return false;

        for (int i = 0;i < retryCount;i ++)
        {
            memset(mRecvBuf,0x00,MES_MAX_RECV_BUFFER_SIZE);
            if (!mWiFiServerSocket.AsyncReadSocket(mRecvBuf,MES_MAX_RECV_BUFFER_SIZE))
            {
                mstrErrorMsg = L"Recv Json Fail";
                continue;
            }
            if (!ftmJson.FromJsonToString(mRecvBuf)) {PELOG4WW_F(L"WIFI Read:%s",JGW_A2W_A(mRecvBuf).c_str());continue;}
            //! log print
            for(boost::property_tree::ptree::iterator msg_it = ftmJson.GetJsonTree().begin(); msg_it != ftmJson.GetJsonTree().end(); ++msg_it)
            {
                PDLOG4WW_F(L"%s : %s",JGW_A2W(msg_it->first).c_str(),JGW_A2W(ftmJson.GetJsonTree().get<std::string>(msg_it->first)).c_str());
            }
            //! 判断请求code和响应code是否一致
            if (!ftmJson.GetJsonValueToKey<int>("responseType",nRsponseCode)) continue;
            if (nRsponseCode == nReqCode) return true;
            else return false;
        }
        return false;
    }
}