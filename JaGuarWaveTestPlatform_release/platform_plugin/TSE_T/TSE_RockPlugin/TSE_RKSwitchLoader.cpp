#include "StdAfx.h"
#include "TSE_RKSwitchLoader.h"
#include "CJGW_RKDeviceHelp.h"
namespace JGW
{
    CTSE_RKSwitchLoader::CTSE_RKSwitchLoader(void)
    {
    }


    CTSE_RKSwitchLoader::~CTSE_RKSwitchLoader(void)
    {
    }

    const wchar_t* CTSE_RKSwitchLoader::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RKSwitchLoader\",\
                \"TestName\": \"Swicth RK Loader\"\
                }";
    }

    bool CTSE_RKSwitchLoader::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        return true;
    }

    bool CTSE_RKSwitchLoader::TSE_Run()
    {
        return CCJGW_RKDeviceHelp::SwitchLoader();
    }

}
