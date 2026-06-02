#include "StdAfx.h"
#include "TSE_RKResetRockusb.h"
#include "CJGW_RKDeviceHelp.h"
namespace JGW
{
    CTSE_RKResetRockusb::CTSE_RKResetRockusb(void)
    {
    }


    CTSE_RKResetRockusb::~CTSE_RKResetRockusb(void)
    {
    }

    const wchar_t* CTSE_RKResetRockusb::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RKResetRockusb\",\
                \"TestName\": \"Reset rockusb device\"\
                }";
    }

    bool CTSE_RKResetRockusb::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        return true;
    }

    bool CTSE_RKResetRockusb::TSE_Run()
    {
        RK_ResetRockusb(0, CCJGW_RKDeviceHelp::m_currentDeviceDesc.dwLayer);
        return true;
    }
}

