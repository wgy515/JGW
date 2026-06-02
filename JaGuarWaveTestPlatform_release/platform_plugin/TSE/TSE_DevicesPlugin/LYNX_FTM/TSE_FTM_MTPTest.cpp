#include "StdAfx.h"
#include "TSE_FTM_MTPTest.h"
#include <JGW_WindowsFuncPlugin/CJGW_MediaTransferProtocol.h>

namespace JGW
{
    CTSE_FTM_MTPTest::CTSE_FTM_MTPTest(void) : mstrDescription(L"Lynx")
        ,mstrManufacturer(L"JAGUAR")
    {
    }

    CTSE_FTM_MTPTest::~CTSE_FTM_MTPTest(void)
    {
    }

    const wchar_t* CTSE_FTM_MTPTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_MTPTest\",\
                \"TestName\": \"MTP识别测试(LYNX FTM)\",\
                \"Manufacturer\": \"MTP制造商信息，JAGUAR\",\
                \"Description\": \"MTP设备描述信息,Lynx\"\
                }";
    }

    bool CTSE_FTM_MTPTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Manufacturer"))
        {
            mstrManufacturer = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Description"))
        {
            mstrDescription = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTM_MTPTest::TSE_Run()
    {
        CCJGW_MediaTransferProtocol mtp;
        if (mtp.GetMTPDevices(mstrManufacturer.c_str(),mstrDescription.c_str())) return true;
        Sleep(mnIntervalTimeMS);
        return false;
    }
}