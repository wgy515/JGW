#include "StdAfx.h"
#include "TSE_CloseTruntable.h"
#include "SCLLibHelper.h"
namespace JGW
{
    CTSE_CloseTruntable::CTSE_CloseTruntable(void)
    {
    }


    CTSE_CloseTruntable::~CTSE_CloseTruntable(void)
    {
    }

    const wchar_t* CTSE_CloseTruntable::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CloseTruntable\",\
                \"TestName\": \"关闭转台连接\",\
                \"TruntableEnvironment\": \"Truntable environment\"\
                }";
    }

    bool CTSE_CloseTruntable::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("TruntableEnvironment"),strParamName))
        {
            mstrTruntableEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_CloseTruntable::TSE_Run()
    {
        SCLLibHelper* pSCLLibHelper = (SCLLibHelper*)(GetGlobalEnvironment()->GetInt(mstrTruntableEnvironment));
        if (NULL != pSCLLibHelper)
        {
            pSCLLibHelper->Close();
        }
        return true;
    }

}

