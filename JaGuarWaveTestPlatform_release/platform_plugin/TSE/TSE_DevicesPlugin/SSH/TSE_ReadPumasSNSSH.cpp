#include "StdAfx.h"
#include "TSE_ReadPumasSNSSH.h"
#include "../TSE_DevicesGlobalResource.h"
namespace JGW
{
    CTSE_ReadPumasSNSSH::CTSE_ReadPumasSNSSH(void)
    {
    }


    CTSE_ReadPumasSNSSH::~CTSE_ReadPumasSNSSH(void)
    {
    }

    const wchar_t* CTSE_ReadPumasSNSSH::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadPumasSNSSH\",\
                \"TestName\": \"读PUMAS SN(AP)\",\
                \"ReadResultSSHEnvironment\": \"包含PUMAS SN结果信息字符串环境变量\",\
                \"PumasSNEnvironment\": \"PumasSN对应的环境变量\"\
                }";
    }

    bool CTSE_ReadPumasSNSSH::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadResultSSHEnvironment"))
        {
            mstrReadResultSSHEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PumasSNEnvironment"))
        {
            mstrPumasSNEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ReadPumasSNSSH::TSE_Run()
    {
        std::wstring strAPNumberResult = GetGlobalEnvironment()->GetString(mstrReadResultSSHEnvironment);
        std::wstring strSN;

        unsigned char szBuf[MAX_SERIAL_NUMBER_FALSH_BIT + 1] = {0};
        if (!CTSE_DevicesGlobalResource::GetDonglesComPortInstance()->PareseDonglesReadBlockBuf(szBuf,JGW_W2A(strAPNumberResult).c_str(),MAX_SERIAL_NUMBER_FALSH_BIT / 4)) 
        {
            LogE(L"解析PUAMS SN失败");
            return false;
        }
        strSN = (wchar_t*)szBuf;
        GetGlobalEnvironment()->PutString(mstrPumasSNEnvironment,strSN);
        LogU(E_TSE_SHOW_CONTENT_INFO,strSN.c_str());
        return true;
    }
}