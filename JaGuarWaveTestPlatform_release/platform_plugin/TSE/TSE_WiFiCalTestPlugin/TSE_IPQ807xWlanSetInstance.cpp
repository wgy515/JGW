#include "StdAfx.h"
#include "TSE_IPQ807xWlanSetInstance.h"
#include "CJGW_IPQ807xPhone.h"
namespace JGW
{
    CTSE_IPQ807xWlanSetInstance::CTSE_IPQ807xWlanSetInstance(void) : meWlanInstance(WLAN0)
    {
    }


    CTSE_IPQ807xWlanSetInstance::~CTSE_IPQ807xWlanSetInstance(void) 
    {
    }

    const wchar_t* CTSE_IPQ807xWlanSetInstance::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xWlanSetInstance::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("WlanInstance"),strParamName))
        {
            meWlanInstance = (WLAN_INSTANCE)_ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ807xWlanSetInstance::TSE_Run()
    {
        return CCJGW_IPQ807xPhone::GetIPQ807xPhone()->WlanSetInstance(meWlanInstance);
    }
}

