#include "StdAfx.h"
#include "TSE_ReadEth0Mac.h"

namespace JGW
{
    CTSE_ReadEth0Mac::CTSE_ReadEth0Mac(void) : mNumberLen(0)
    {
    }


    CTSE_ReadEth0Mac::~CTSE_ReadEth0Mac(void)
    {
    }

    const wchar_t* CTSE_ReadEth0Mac::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadEth0Mac\",\
                \"TestName\": \"解析Lynx设备以太网MAC地址\",\
                \"ETH0EnvironmentInfo\": \"环境变量名称,读取的EHT0信息\",\
                \"Environment\": \"解析到的以太网MAC地址保存环境变量名\",\
                \"EnvironmentLen\": \"MAC长度限制默认按照12位,小于等于0不判断长度\"\
                }";
    }

    bool CTSE_ReadEth0Mac::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ETH0EnvironmentInfo"))
        {
            mstrETH0EnvironmentInfo = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Environment"))
        {
            mstrEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentLen"))
        {
            mNumberLen = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ReadEth0Mac::TSE_Run()
    {        
        std::wstring strReadMac = GetGlobalEnvironment()->GetString(mstrETH0EnvironmentInfo);

        if (std::wstring::npos == strReadMac.find(L"NODEID")) return false;

        JGW_ReplaceStringW(strReadMac,L"NODEID",L"");
        JGW_ReplaceStringW(strReadMac,L"=",L"");
        JGW_ReplaceStringW(strReadMac,L" ",L"");
        LogI_F(L"Read Eth0 Mac:%s",strReadMac.c_str());
        LogU(E_TSE_SHOW_CONTENT_INFO,strReadMac.c_str());
        if (mNumberLen > 0 && strReadMac.length() != mNumberLen)
        {
            LogE_F(L"读取的eth0 mac长度:%d,与配置中指定长度(%d)不一致",strReadMac.length(),mNumberLen);
            return false;
        }

        GetGlobalEnvironment()->PutString(mstrEnvironment,strReadMac);
        return true;
    }
}
