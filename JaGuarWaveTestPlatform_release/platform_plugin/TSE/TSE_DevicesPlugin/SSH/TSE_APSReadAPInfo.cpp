#include "StdAfx.h"
#include "TSE_APSReadAPInfo.h"

namespace JGW
{
    CTSE_APSReadAPInfo::CTSE_APSReadAPInfo(void)
    {
    }


    CTSE_APSReadAPInfo::~CTSE_APSReadAPInfo(void)
    {
    }

    const wchar_t* CTSE_APSReadAPInfo::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_APSReadAPInfo\",\
                \"TestName\": \"读取小AP信息(SSID,PASSWORD)(AP)\",\
                \"SoftAPResultEnvironment\": \"读取小AP信息保存的环境变量\",\
                \"SSIDEnvironment\": \"读取SSID保存指定环境变量名称\",\
                \"PasswordEnvironment\": \"读取PASSWORD保存指定环境变量名称\"\
                }";
    }

    bool CTSE_APSReadAPInfo::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SoftAPResultEnvironment"))
        {
            mstrSoftAPResultEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SSIDEnvironment"))
        {
            mstrSSIDEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PasswordEnvironment"))
        {
            mstrPasswordEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_APSReadAPInfo::TSE_Run()
    {
        std::wstring stroftAPResult = GetGlobalEnvironment()->GetString(mstrSoftAPResultEnvironment);
        std::map<std::wstring,std::wstring> mapProperty;

        JGW_PareserConfigString(stroftAPResult,mapProperty);
        GetGlobalEnvironment()->PutString(mstrSSIDEnvironment,mapProperty[L"ssid"]);
        GetGlobalEnvironment()->PutString(mstrPasswordEnvironment,mapProperty[L"wpa_passphrase"]);

        LogU(E_TSE_SHOW_CONTENT_INFO,JGW_GetFormatWString(L"%s %s",mapProperty[L"ssid"].c_str(),mapProperty[L"wpa_passphrase"].c_str()).c_str());

        return true;
    }
}