#pragma once
#include <JGW_WiFiSetting/CJGW_WiFiSettingImpl.h>
#include "CJGW_WiFiServerSocket.h"
#include "JSON/CJGW_WiFiResponseBaseJson.h"
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
#define MES_DATABSE_COMMAND_RECOUNT 3
#define MES_MAX_RECV_BUFFER_SIZE 1024
namespace JGW
{
    class CCJGW_WiFiSetting : public CCJGW_WiFiSettingImpl
    {
    public:
        CCJGW_WiFiSetting(void);
        ~CCJGW_WiFiSetting(void);
    private:
        std::wstring& GetErrorMsg();
        //! 
        virtual void SetWiFiServerInfo(const std::string& strSocketIP,const size_t socketPort,const size_t timeOutSec,const std::string& strSourceIP = "");
        //! 
        virtual void GetWiFiServerInfo(std::string& strSocketIP,size_t& socketPort);
        //! 
        virtual bool ScanWiFi(std::vector<std::string>& vstrSSID);
        //! 
        virtual bool ConnectWiFi(const std::string& strSSID,const std::string& strPassword);
        //! 
        virtual void DisConnectWiFi();
        //! 连接MES 服务 如果已经连接则直接返回true，否则则连接MES SOCKET
        bool CheckConnectWiFiServiceSocket();
        //! 
        bool WriteJsonSocket(const std::string& strWriteJson,size_t timeOutSec = 10,int retryCount  = 3 );
        //! 
        bool ReadJsonSocketToWriteSocket(const std::string& strWriteJson,size_t nReqCode,CCJGW_FTM_Json& ftmJson,size_t timeOutSec = 10 ,int retryCount = 3);
        //! 
        void DisConnectSocket(); 
        //!
        bool CheckConnectWiFi(const std::string& strSSID,const std::string& strPassword,const std::string& strWlanGateWay,const std::string& strLocalAddress,const std::string& strInterfaceDescription,std::string& strWlanAddress);

        /*********** puma ***************/
        bool ConnectPerasoDevice();

        bool WriteReadPerasoCommad(const std::string& strWrite,std::string& strRead);

        void DisConnectPerasoDevice();
        /**************shell execute*********************/
        bool ShellExecuteCommad(const std::string& command);

        //! 通用执行
        template <typename Func> 
        inline bool ExecuteWiFiSetting_G(Func func)
        {
            if (!CheckConnectWiFiServiceSocket()) { GetErrorMsg() = GetString(E_WIFI_CONNECT_FAIL_CHECK_SERVICE_IS_RUNING); return false;}
            try
            {
                bool bResult = false;
                CCJGW_WiFiResponseBaseJson resResponseJson;
                CCJGW_FTM_Json ftmJson;
                for (int i = 0;i < MES_DATABSE_COMMAND_RECOUNT && !bResult;i ++)
                {
                    bResult = func(ftmJson); 
                }
                resResponseJson.FromJosn(ftmJson);
                GetErrorMsg() = JGW_A2W(resResponseJson.returnMessage);
                //! mWiFiServerSocket.CloseSocket();
                return (resResponseJson.bStatus && bResult);
            }
            catch (...) 
            {
                return false;
            }
        }
    private:
        bool mbIsLoginMES;
        char* mRecvBuf;
        size_t mnSocketPort;
        size_t mnTimeOut;
        std::string mstrSocketIP;
        std::string mstrSourceSocketIP;
        std::wstring mstrErrorMsg;  
        CCJGW_AsyncSocket mWiFiServerSocket;
        //! CCJGW_WiFiServerSocket mWiFiServerSocket;
    };
}