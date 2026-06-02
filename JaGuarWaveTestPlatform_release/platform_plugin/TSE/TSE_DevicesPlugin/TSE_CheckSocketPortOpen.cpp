#include "StdAfx.h"
#include "TSE_CheckSocketPortOpen.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
namespace JGW
{
    CTSE_CheckSocketPortOpen::CTSE_CheckSocketPortOpen(void) : mnTimeOutSec(10),mnSocketPort(80)
    {
    }


    CTSE_CheckSocketPortOpen::~CTSE_CheckSocketPortOpen(void)
    {
    }

    const wchar_t* CTSE_CheckSocketPortOpen::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CheckSocketPortOpen\",\
                \"TestName\": \"检查SOCKET PORT是否开放\",\
                \"SourceAddrEnvironment\": \"源IP地址对应的环境变量\",\
                \"HostAddrEnvironment\": \"本地主机地址对应的环境变量\",\
                \"SocketPort\": \"socket port\",\
                \"TimeOutSec\": \"通信超时时间设置 以S为单位，默认10S\"\
                }";
    }

    bool CTSE_CheckSocketPortOpen::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SourceAddrEnvironment"))
        {
            mstrSourceAddrEnvironment = strParamValue;
        }
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"HostAddrEnvironment"))
        {
            mstrHostAddrEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SocketPort"))
        {
            mnSocketPort = _wtoi(strParamValue);    
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOutSec"))
        {
            mnTimeOutSec = _wtoi(strParamValue);    
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_CheckSocketPortOpen::TSE_Run()
    {
        jgw_timer tm;
        std::string strSourceAddr = GetGlobalEnvironment()->GetAnsiString(mstrSourceAddrEnvironment);
        std::string strHostAddr = GetGlobalEnvironment()->GetAnsiString(mstrHostAddrEnvironment);
        std::string strSocketPort;
        LogI_F("Check Socket Port Open connect addr %s:%d (src addr : %s)",strHostAddr.c_str(),mnSocketPort,strSourceAddr.c_str());
        JGW_FormatString(strSocketPort,"%d",mnSocketPort);
        mcSyncSocket.CleanupSocket();

        while (tm.elapsed() <= mnTimeOutSec)
        {
            LogI_F(L"Start Check Socket Port Is Open ...");
            if (mcSyncSocket.ConnectSocket(strHostAddr.c_str(),strSocketPort.c_str(),strSourceAddr.empty()?NULL:strSourceAddr.c_str())) 
            {
                mcSyncSocket.CleanupSocket();
                return true;
            }
            Sleep(200);
        }
        return false;
    }

}
