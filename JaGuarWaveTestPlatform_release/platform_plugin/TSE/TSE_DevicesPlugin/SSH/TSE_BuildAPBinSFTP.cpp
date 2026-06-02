#include "StdAfx.h"
#include "TSE_BuildAPBinSFTP.h"
#include "TSE_AP_NUMBER_DEFINE.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CTSE_BuildAPBinSFTP::CTSE_BuildAPBinSFTP(void)
    {
    }


    CTSE_BuildAPBinSFTP::~CTSE_BuildAPBinSFTP(void)
    {
    }

    const wchar_t* CTSE_BuildAPBinSFTP::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_BuildAPBinSFTP\",\
                \"TestName\": \"生成SN/MAC/SSID/PASSWORD BIN文件(AP)\",\
                \"SNEnvironment\": \"SN对应的环境变量值\",\
                \"MACEnvironment\": \"MAC对应的环境变量值\",\
                \"SSIDEnvironment\": \"SSID对应的环境变量值\",\
                \"PasswordEnvironment\": \"password对应的环境变量值\",\
                \"APBinFilePathEnvironment\": \"小AP 号段BIN配置文件保存路径环境变量\"\
                }";
    }

    bool CTSE_BuildAPBinSFTP::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(L"PasswortEnvironment",strParamName))
        {
            mstrPasswortEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"APBinFilePathEnvironment",strParamName))
        {
            mstrAPBinFilePathEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_BuildAPBinSFTP::TSE_Run()
    {
        s_ap_info sAPInfo = {0};
        std::string temp,numberInfo;
        std::string strAPBinFilePath = GetGlobalEnvironment()->GetAnsiString(mstrAPBinFilePathEnvironment);

        if (!mstrSNEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetAnsiString(mstrSNEnvironment).c_str();
            numberInfo += temp;
            memcpy_s(sAPInfo.sn,32,temp.c_str(),temp.length());
        }
        if (!mstrMACEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetAnsiString(mstrMACEnvironment).c_str();
            numberInfo += ",";
            numberInfo += temp;
            memcpy_s(sAPInfo.mac,32,temp.c_str(),temp.length());
        }
        if (!mstrSSIDEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetAnsiString(mstrSSIDEnvironment).c_str();
            numberInfo += ",";
            numberInfo += temp;
            memcpy_s(sAPInfo.ssid_60G,32,temp.c_str(),temp.length());
        }
        if (!mstrPasswortEnvironment.empty())
        {
            temp = GetGlobalEnvironment()->GetAnsiString(mstrPasswortEnvironment).c_str();
            numberInfo += ",";
            numberInfo += temp;
            memcpy_s(sAPInfo.password_60G,32,temp.c_str(),temp.length());
        }

        LogU(E_TSE_SHOW_CONTENT_INFO,JGW_A2W(numberInfo).c_str());
        if (!JGW_WriteFileToBuf(strAPBinFilePath.c_str(),(char*)&sAPInfo,sizeof(s_ap_info)))
        {
            LogE_F("Write File(%s) Fail\n",strAPBinFilePath.c_str());
            return false;
        }
        return true;
    }
}