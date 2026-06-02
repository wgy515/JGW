#include "StdAfx.h"
#include "TSE_FTM_GetEth0IPAddress.h"
#include "..\TSE_DevicesGlobalResource.h"
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
namespace JGW
{
    CTSE_FTM_GetEth0IPAddress::CTSE_FTM_GetEth0IPAddress(void) : mstrCommand(L"adb shell \"ifconfig eth0 |grep 'inet addr'\"")
    {
    }

    CTSE_FTM_GetEth0IPAddress::~CTSE_FTM_GetEth0IPAddress(void)
    {
    }

    const wchar_t* CTSE_FTM_GetEth0IPAddress::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_GetEth0IPAddress\",\
                \"TestName\": \"获取网口IP地址(LYNX FTM)\",\
                \"Command\": \"获取网口IP地址命令\",\
                \"Pos\": \"通过指定字符串，定位IP地址的位置\"\
                }";
    }

    bool CTSE_FTM_GetEth0IPAddress::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_FTM_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Command"))
        {
            mstrCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Pos"))
        {
            mstrPos = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTM_GetEth0IPAddress::TSE_Run()
    {
        CCJGW_ProcessPipe* pCMDPipe = CTSE_DevicesGlobalResource::GetProcessCMDPipe();
        if (!pCMDPipe) return false;

        std::wstring strRead;
        if (!pCMDPipe->timed_send(mstrCommand,10)) return false;
        CTSE_DevicesGlobalResource::ReadCMDProcessPipeEOF(pCMDPipe,strRead);
        const wchar_t* pTemp = JGW_WStrCaseWStr(strRead.c_str(),mstrPos.c_str());
        if (!pTemp) { Sleep(mnIntervalTimeMS); return false;}
        std::wstring strTmp;
        wchar_t strIP[260] = {0};
        JGW_FormatWString(strTmp,L"%s%%s ",mstrPos.c_str());
        _stscanf(pTemp,strTmp.c_str(),strIP);
        LogI_F(L"Eth0 IP Address : %s\n",strIP);
        GetGlobalEnvironment()->PutString(TSE_FTM_CLIENT_SOCKET_IP_ADDRESS,strIP);
        return true;
    }
}