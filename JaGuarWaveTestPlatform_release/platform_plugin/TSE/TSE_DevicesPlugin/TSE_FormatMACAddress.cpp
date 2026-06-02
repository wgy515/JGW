#include "StdAfx.h"
#include "TSE_FormatMACAddress.h"

namespace JGW
{
    CTSE_FormatMACAddress::CTSE_FormatMACAddress(void) : mcMacFormatSeparate(':')
    {
    }


    CTSE_FormatMACAddress::~CTSE_FormatMACAddress(void)
    {
    }

    const wchar_t* CTSE_FormatMACAddress::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FormatMACAddress\",\
                \"TestName\": \"Format MAC\",\
                \"MacEnvironment\": \"mac Environment\",\
                \"SaveMacEnvironment\": \"format mac save Environment\"\
                }";
    }

    bool CTSE_FormatMACAddress::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if( TSE_PARAM_NAME_EQUAL(_T("MacEnvironment"),strParamName) )
        {
            mstrMacEnvironment = strParamValue;
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("SaveMacEnvironment"),strParamName) )
        {
            mstrSaveMacEnvironment = strParamValue;
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("MacFormatSeparate"),strParamName) )
        {
            std::wstring strTemp = strParamValue;
            if (strTemp.length() == 0)
            {
                mcMacFormatSeparate = ':';
            }
            else
            {
                //JGW_EraseLastAndFristTwoCharsW(strTemp,' ');
                mcMacFormatSeparate = (char)strParamValue[0];
            }
        }
        else
        {
            return false;
        }      
        return true;
    }

    bool CTSE_FormatMACAddress::TSE_Run()
    {
        std::wstring strMac = GetGlobalEnvironment()->GetString(mstrMacEnvironment);
        if (strMac.length() != 12)
        {
            LogE_F("mac(%s) len != 12",strMac.c_str());
            return false;
        }
        std::wstring strFormatMac = L"";
        for (int i = 0;i < 12;i += 2)
        {
            strFormatMac += strMac.substr(i,2);
            strFormatMac += mcMacFormatSeparate;
        }
        JGW_EraseLastAndFristChars(strFormatMac,mcMacFormatSeparate);
        LogU(E_TSE_SHOW_CONTENT_INFO,strFormatMac.c_str());
        GetGlobalEnvironment()->PutString(mstrSaveMacEnvironment,strFormatMac);
        return true;
    }
}
