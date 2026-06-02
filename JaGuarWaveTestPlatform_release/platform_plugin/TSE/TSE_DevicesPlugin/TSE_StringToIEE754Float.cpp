#include "StdAfx.h"
#include "TSE_StringToIEE754Float.h"
#include <regex>
namespace JGW
{
    CTSE_StringToIEE754Float::CTSE_StringToIEE754Float(void) : mbIsHexString (true)
    {
    }


    CTSE_StringToIEE754Float::~CTSE_StringToIEE754Float(void)
    {
    }

    const wchar_t* CTSE_StringToIEE754Float::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_StringToIEE754Float\",\
                \"TestName\": \"String To IEE754 Float\",\
                \"IsHexString\": \"Is Hex String Def:true\",\
                \"IEE754StringEnvrionment\": \"IEE754 String Environment\",\
                \"SaveFloatEnvironment\": \"Save Float Environment\"\
                }";
    }

    bool CTSE_StringToIEE754Float::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"IsHexString",strParamName))
        {
            mbIsHexString = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"IEE754StringEnvrionment",strParamName))
        {
            mstrIEE754StringEnvrionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveFloatEnvironment",strParamName))
        {
            mstrSaveFloatEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    typedef union 
    {
        float fData; 
        unsigned long ulData; 
        char cData[4]; 
    } Resolve_Typedef;

    bool CTSE_StringToIEE754Float::TSE_Run()
    {
        Resolve_Typedef resolve = {0};
        std::string strValue = GetGlobalEnvironment()->GetAnsiString(mstrIEE754StringEnvrionment);
        if (mbIsHexString)
        {
            JGW_ReplaceStringA(strValue,"0x","");
            JGW_ReplaceStringA(strValue,"0X","");
            std::regex partten("[0-9A-Za-z]{0,8}");
            if (!regex_match(strValue,partten))
            {
                LogE_F(L"rule:[0-9A-Za-z]{0,8},value:%s,regex_match fail",JGW_A2W(strValue).c_str());
                return false;
            }
            sscanf_s(strValue.c_str(),"%X",&resolve.ulData);
            LogU_F(E_TSE_SHOW_CONTENT_INFO,L"%.3f",resolve.fData);
            GetGlobalEnvironment()->PutFloat(mstrSaveFloatEnvironment,resolve.fData);
        }
        else
        {
            std::regex partten("[0-9]{0,10}");
            if (!regex_match(strValue,partten))
            {
                LogE_F(L"rule:[0-9A-Za-z]{0,8},value:%s,regex_match fail",JGW_A2W(strValue).c_str());
                return false;
            }
            sscanf_s(strValue.c_str(),"%u",&resolve.ulData);
            LogU_F(E_TSE_SHOW_CONTENT_INFO,L"%.3f",resolve.fData);
            GetGlobalEnvironment()->PutFloat(mstrSaveFloatEnvironment,resolve.fData);
        }    
        return true;
    }
}

