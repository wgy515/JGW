#include "StdAfx.h"
#include "TSE_ReadAPBinNumber.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "TSE_AP_NUMBER_DEFINE.h"
namespace JGW
{
    CTSE_ReadAPBinNumber::CTSE_ReadAPBinNumber(void)
    {
    }


    CTSE_ReadAPBinNumber::~CTSE_ReadAPBinNumber(void)
    {
    }

    const wchar_t* CTSE_ReadAPBinNumber::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadAPBinNumber\",\
                \"TestName\": \"不使用,解析BIN文件号段信息(AP)\",\
                \"SNEnvironment\": \"SN环境变量\",\
                \"MACEnvironment\": \"MAC环境变量\",\
                \"SSIDEnvironment\": \"SSID环境变量\",\
                \"PasswordEnvironment\": \"Password环境变量\",\
                \"APBinFilePathEnvironment\": \"bin文件路径环境变量\"\
                }";
    }

    bool CTSE_ReadAPBinNumber::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
            mstrPasswordEnvironment = strParamValue;
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

    bool CTSE_ReadAPBinNumber::TSE_Run()
    {
        std::string strAPBinFilePath = GetGlobalEnvironment()->GetAnsiString(mstrAPBinFilePathEnvironment);
        size_t file_size = 0;
        char* szFileBuf = JGW_GetFileBufToPath_C(strAPBinFilePath.c_str(),file_size);

        if (file_size<= 0 || !szFileBuf)
        {
            LogE_F("Read AP Bin File(%s) Fail",strAPBinFilePath.c_str());
            return false;
        }

        s_ap_info* psApInfo = (s_ap_info*)szFileBuf;

        GetGlobalEnvironment()->PutAnsiString(mstrSNEnvironment,psApInfo->sn);
        GetGlobalEnvironment()->PutAnsiString(mstrMACEnvironment,psApInfo->mac);
        GetGlobalEnvironment()->PutAnsiString(mstrSSIDEnvironment,psApInfo->ssid_60G);
        GetGlobalEnvironment()->PutAnsiString(mstrPasswordEnvironment,psApInfo->password_60G);

        LogU(E_TSE_SHOW_CONTENT_INFO,JGW_GetFormatWString(L"%s,%s,%s,%s",GetGlobalEnvironment()->GetString(mstrSNEnvironment).c_str(),GetGlobalEnvironment()->GetString(mstrMACEnvironment).c_str(),GetGlobalEnvironment()->GetString(mstrSSIDEnvironment).c_str(),GetGlobalEnvironment()->GetString(mstrPasswordEnvironment).c_str()).c_str());
        LogI_F("sn:%s,mac:%s,ssid:%s,password:%s\n",psApInfo->sn,psApInfo->mac,psApInfo->ssid_60G,psApInfo->password_60G);


        return true;
    }
}
