#include "StdAfx.h"
#include "TSE_BuildAPInfoTxt.h"
#include <JGW_PropertyConfigPlugin/CJGW_PropertyExport.h>
namespace JGW
{
    CTSE_BuildAPInfoTxt::CTSE_BuildAPInfoTxt(void)
    {
    }


    CTSE_BuildAPInfoTxt::~CTSE_BuildAPInfoTxt(void)
    {
        std::wstring strAPTxtFilePath = JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrAPInfoFilePathEnvironment).c_str());
        DestroyFilePropertyToFileName(strAPTxtFilePath.c_str());
    }

    const wchar_t* CTSE_BuildAPInfoTxt::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_BuildAPInfoTxt\",\
                \"TestName\": \"构建小AP 号段配置文件(windows格式)\",\
                \"SNEnvironment\": \"SN对应的环境变量值\",\
                \"MACEnvironment\": \"MAC对应的环境变量值\",\
                \"SSIDEnvironment\": \"SSID对应的环境变量值\",\
                \"PasswordEnvironment\": \"password对应的环境变量值\",\
                \"APTxtFilePathEnvironment\": \"小AP 号段配置文件保存路径环境变量\"\
                }";
    }

    bool CTSE_BuildAPInfoTxt::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SNEnvironment",strParamName))
        {
            mstrSNEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"MACEnvironment",strParamName))
        {
            mstrMACEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SSIDEnvironment",strParamName))
        {
            mstrSSIDEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"PasswordEnvironment",strParamName))
        {
            mstrPasswordEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"APTxtFilePathEnvironment",strParamName))
        {
            mstrAPInfoFilePathEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_BuildAPInfoTxt::TSE_Run()
    {
        std::wstring strAPTxtFilePath = JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrAPInfoFilePathEnvironment).c_str()),temp,numberInfo;
        JGW::CCJGW_PropertyImpl* pPropertyImpl = GetFilePropertyToFileName(strAPTxtFilePath.c_str());
        pPropertyImpl->SetPropertyAppName(L"ap_info");

        if (!mstrSNEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetString(mstrSNEnvironment).c_str();
            numberInfo += temp;
            pPropertyImpl->PutString(L"sn",temp);
        }
        if (!mstrMACEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetString(mstrMACEnvironment).c_str();
            numberInfo += L",";
            numberInfo += temp;
            pPropertyImpl->PutString(L"mac",temp);
        }
        else
        {
            pPropertyImpl->PutString(L"mac",L"");
        }
        if (!mstrSSIDEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetString(mstrSSIDEnvironment).c_str();
            numberInfo += L",";
            numberInfo += temp;
            pPropertyImpl->PutString(L"ssid",temp);
        }
        else
        {
            pPropertyImpl->PutString(L"ssid",L"");
        }
        if (!mstrPasswordEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetString(mstrPasswordEnvironment).c_str();
            numberInfo += L",";
            numberInfo += temp;
            pPropertyImpl->PutString(L"password",temp);   
        }
        else
        {
            pPropertyImpl->PutString(L"password",L"");   
        }
        LogU(E_TSE_SHOW_CONTENT_INFO,numberInfo.c_str());
        return true;
    }
}