#include "StdAfx.h"
#include "TSE_PingTest.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
namespace JGW
{
    CTSE_PingTest::CTSE_PingTest(void) : mnPingTestTimeSec(1),mnPingTimeOutMs(1000)
    {
    }

    CTSE_PingTest::~CTSE_PingTest(void)
    {
    }

    const wchar_t* CTSE_PingTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_PingTest\",\
                \"TestName\": \"Ping测试\",\
                \"SourceAddrEnvironment\": \"Socke 路由IP地址\",\
                \"HostEnvironment\": \"主机IP地址\",\
                \"EthernetGUIDEnvironment\": \"Net Card GUID\",\
                \"PingTimeOutMs\": \"ping 单次测试接收超时时间毫秒\",\
                \"PingTestTimeSec\": \"ping测试，默认在1S中测试一次\",\
                \"TimeOutSec\": \"超时时间秒\"\
                }";
    }

    bool CTSE_PingTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;

        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SourceAddrEnvironment"))
        {
            mstrSourceAddrEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"HostEnvironment"))
        {
            mstrHostEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EthernetGUIDEnvironment"))
        {
            mstrEthernetGUIDEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PingTestTimeSec"))
        {
            mnPingTestTimeSec = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOutSec"))
        {
            mnTimeOutSec = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PingTimeOutMs"))
        {
            mnPingTimeOutMs = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }

        return true;
    }

    bool CTSE_PingTest::TSE_Run()
    {
        std::string strSourceAddrEnvironment,strHostEnvironment,strPingAddr;
        strHostEnvironment = GetGlobalEnvironment()->GetAnsiString(mstrHostEnvironment);
        if (!mstrSourceAddrEnvironment.empty()) strSourceAddrEnvironment = GetGlobalEnvironment()->GetAnsiString(mstrSourceAddrEnvironment);
        LogI_F("ping %s (source addr : %s)",strHostEnvironment.c_str(),strSourceAddrEnvironment.c_str());
        
        jgw_timer jt;
        bool bResult = false;

        while (jt.elapsed() <= mnTimeOutSec && !bResult)
        {
            bResult = true;
            strPingAddr = strSourceAddrEnvironment;
            if (!mstrEthernetGUIDEnvironment.empty())
            {
                std::wstring strEthernetGUID = GetGlobalEnvironment()->GetString(mstrEthernetGUIDEnvironment);
                std::string strIPAddr = JGW_W2A(JGW_GetIPV4AddrToAdpaterName(strEthernetGUID.c_str()));
                if (!strIPAddr.empty())
                {
                    strPingAddr = strIPAddr;
                    LogI_F("Get NetCard IPV4 Address:%s",strIPAddr.c_str());
                    GetGlobalEnvironment()->PutAnsiString(mstrSourceAddrEnvironment,strPingAddr);
                }
            }
            for (size_t i = 0;i < mnPingTestTimeSec && bResult;i ++)
            {
                bResult &= mIcmpSocket.Ping(strHostEnvironment,strPingAddr.empty()?NULL:strPingAddr.c_str(),mnPingTimeOutMs);
                if (1 != mnPingTestTimeSec) Sleep(1000);
            }       
        }

        return bResult;
    }
}