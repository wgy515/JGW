#include "StdAfx.h"
#include "TSE_SetPhoneResource.h"

namespace JGW
{
    CTSE_SetPhoneResource::CTSE_SetPhoneResource(void)/* : meMode(PhyA_only),mePhyID(PhyID_A)*/
    {
    }


    CTSE_SetPhoneResource::~CTSE_SetPhoneResource(void)
    {
    }

    const wchar_t* CTSE_SetPhoneResource::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SetPhoneResource\",\
                \"TestName\": \"set phone resouce context\",\
                \"ResourceContextEnvironment\": \"Save Connect Resource Environment\"\
                }";
    }

    bool CTSE_SetPhoneResource::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("ResourceContextEnvironment"),strParamName))
        {
            mstrResourceContextEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SetPhoneResource::TSE_Run()
    {
        HANDLE hResourceContext = (HANDLE)GetGlobalEnvironment()->GetInt(mstrResourceContextEnvironment);
        PHONE->SetResourceContent(hResourceContext);
        return true;
    }
}
