#include "StdAfx.h"
#include "TSE_BuildDateTimeStamp.h"

namespace JGW
{
    //! 2021/12/15 13:50
    CTSE_BuildDateTimeStamp::CTSE_BuildDateTimeStamp(void) : mstrTimeStampYMDHMSFormmat(L"%04d/%02d/%02d %02d:%02d:%02d")
    {
    }


    CTSE_BuildDateTimeStamp::~CTSE_BuildDateTimeStamp(void)
    {
    }

    const wchar_t* CTSE_BuildDateTimeStamp::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_BuildDateTimeStamp::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"DateTimeEnvironment",strParamName))
        {
            mstrDateTimeEnvironment =  strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TimeStampEnvironment",strParamName))
        {
            mstrTimeStampEnvironment =  strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TimeStampYMDHMSFormmat",strParamName))
        {
            mstrTimeStampYMDHMSFormmat =  strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TimeStampYMDHMSEnvironment",strParamName))
        {
            mstrTimeStampYMDHMSEnvironment =  strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_BuildDateTimeStamp::TSE_Run()
    {
        if (!mstrDateTimeEnvironment.empty())
        {
            std::wstring strTmep = JGW_GetTimeFolder();
            LogI_F(L"Date Time : %s",strTmep.c_str());
            GetGlobalEnvironment()->PutString(mstrDateTimeEnvironment,strTmep);
        }
        if (!mstrTimeStampEnvironment.empty())
        {
            std::wstring timeStamp = JGW_GetFormatWString(L"%u",JGW_GetTimeOfDay());
            LogI_F(L"Time Stamp : %s",timeStamp.c_str());
            GetGlobalEnvironment()->PutString(mstrTimeStampEnvironment,timeStamp);
        }

        if (!mstrTimeStampYMDHMSEnvironment.empty())
        {
            std::wstring strTimeStampYMDHMS = JGW_GetCurrentTimeStampYMDHMS(mstrTimeStampYMDHMSFormmat);
            LogI_F(L"Time Stamp YMDHMS : %s",strTimeStampYMDHMS.c_str());
            GetGlobalEnvironment()->PutString(mstrTimeStampYMDHMSEnvironment,strTimeStampYMDHMS);
        }

        return true;
    }

}
