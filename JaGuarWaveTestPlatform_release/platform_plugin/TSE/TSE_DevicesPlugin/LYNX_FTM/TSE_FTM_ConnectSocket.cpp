#include "StdAfx.h"
#include "TSE_FTM_ConnectSocket.h"
#include "../TSE_DevicesGlobalResource.h"
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
namespace JGW
{
    CTSE_FTM_ConnectSocket::CTSE_FTM_ConnectSocket(void):mSocketIP("127.0.0.1"),mSocketPort(5432),mbIsConfigSocketIP(false)
    {
    }

    const wchar_t* CTSE_FTM_ConnectSocket::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_ConnectSocket\",\
                \"TestName\": \"连接设备(LYNX FTM)\",\
                \"ClientIP\": \"adb tcpip IP地址\",\
                \"ClientPort\": \"adb tcpip 端口远程调试ADB\",\
                \"TimeOutSec\": \"通信超时时间设置 以S为单位，默认10S\"\
                }";
    }

    bool CTSE_FTM_ConnectSocket::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_FTM_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ClientIP"))
        {
            mSocketIP = JGW_W2A_W(strParamValue);
            mbIsConfigSocketIP = true;
            GetGlobalEnvironment()->PutString(TSE_FTM_CLIENT_SOCKET_IP_ADDRESS,strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ClientPort"))
        {
            mSocketPort = _wtoi(strParamValue);    
            GetGlobalEnvironment()->PutString(TSE_FTM_CLIENT_PORT,strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTM_ConnectSocket::TSE_Run()
    {
        if (!mbIsConfigSocketIP) mSocketIP = JGW_W2A(GetGlobalEnvironment()->GetString(TSE_FTM_CLIENT_SOCKET_IP_ADDRESS));
        if (!CTSE_DevicesGlobalResource::GetFTMClientSocket()->AsyncAdbTcpIPConnnectSocket(mSocketIP,mSocketPort,mTimeOutSec)) return false;

        //         char szReadBuf[TSE_FTM_MAX_READ_BUF] = {0};
        //         memset(szReadBuf,0x00,TSE_FTM_MAX_READ_BUF);
        //         if (!CTSE_DevicesGlobalResource::GetFTMClientSocket()->AsyncReadSocket(szReadBuf,TSE_FTM_MAX_READ_BUF,mTimeOutSec))
        //         {
        //             LogE_F("Recv Version Json Fail\n");
        //             return false;
        //         }

        return true;//(NULL != strstr(szReadBuf,MSG_CLINET_CONNECT_SUCCESS));
    }
}

