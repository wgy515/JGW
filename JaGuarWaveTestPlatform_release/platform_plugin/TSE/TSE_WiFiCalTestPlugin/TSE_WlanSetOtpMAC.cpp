#include "StdAfx.h"
#include "TSE_WlanSetOtpMAC.h"

namespace JGW
{
    CTSE_WlanSetOtpMAC::CTSE_WlanSetOtpMAC(void)
    {
    }


    CTSE_WlanSetOtpMAC::~CTSE_WlanSetOtpMAC(void)
    {
    }

    const wchar_t* CTSE_WlanSetOtpMAC::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WlanSetOtpMAC\",\
                \"TestName\": \"Wlan Set Otp Mac\",\
                \"MacAddressEnv\": \"Mac address environment\"\
                }";
    }

    bool CTSE_WlanSetOtpMAC::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"MacAddressEnv",strParamName))
        {
            mstrMacAddressEnv = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WlanSetOtpMAC::TSE_Run()
    {
        if (mstrMacAddressEnv.empty())
        {
            LogE(L"Mac address environment is empty");
            return false;
        }
        std::string strMac = GetGlobalEnvironment()->GetAnsiString(mstrMacAddressEnv),strTemp;
        if (12 != strMac.length())
        {
            LogE_F(L"Invalid MAC address : %s",GetGlobalEnvironment()->GetString(mstrMacAddressEnv));
            return false;
        }
        unsigned char szMac[7] = {0};
        for (size_t i = 0;i < 6;i++)
        {
            strTemp = strMac.substr(i * 2,2);
            szMac[i] = static_cast<unsigned char>(strtol(strTemp.c_str(),NULL,16));
        }
        return PHONE->SetMacAddress(szMac);
    }
}

