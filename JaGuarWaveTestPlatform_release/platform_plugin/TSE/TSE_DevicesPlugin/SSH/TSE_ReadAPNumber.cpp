#include "StdAfx.h"
#include "TSE_ReadAPNumber.h"

namespace JGW
{
    CTSE_ReadAPNumber::CTSE_ReadAPNumber(void)
    {
    }


    CTSE_ReadAPNumber::~CTSE_ReadAPNumber(void)
    {
    }

    const wchar_t* CTSE_ReadAPNumber::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadAPNumber\",\
                \"TestName\": \"读取组装包装SN(AP)\",\
                \"APNumberResultEnvironment\": \"包含SN相关信息字符串环境变量\",\
                \"PCBASNEnvironment\": \"PCBASN对应环境变量\",\
                \"SerialNumberEnvironment\": \"包装SN环境变量\"\
                }";
    }

    bool CTSE_ReadAPNumber::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"APNumberResultEnvironment"))
        {
            mstrAPNumberResultEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PCBASNEnvironment"))
        {
            mstrPCBASNEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SerialNumberEnvironment"))
        {
            mstrSerialNumberEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ReadAPNumber::TSE_Run()
    {
        std::wstring strAPNumberResult = GetGlobalEnvironment()->GetString(mstrAPNumberResultEnvironment);
        std::vector<std::wstring> vstrNumber;
        JGW_ParserStrW(strAPNumberResult.c_str(),L",",vstrNumber);

        GetGlobalEnvironment()->PutString(mstrPCBASNEnvironment,!vstrNumber.empty()?vstrNumber[0]:L"");
        GetGlobalEnvironment()->PutString(mstrSerialNumberEnvironment,vstrNumber.size()>1?vstrNumber[1]:L"");

        LogU(E_TSE_SHOW_CONTENT_INFO,JGW_GetFormatWString(L"%s,%s",GetGlobalEnvironment()->GetString(mstrPCBASNEnvironment).c_str(),GetGlobalEnvironment()->GetString(mstrSerialNumberEnvironment).c_str()).c_str());

        return true;
    }
}
