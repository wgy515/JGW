#include "StdAfx.h"
#include "TSE_RKDInit.h"
#include "CJGW_RKDeviceHelp.h"
namespace JGW
{
    CTSE_RKDInit::CTSE_RKDInit(void) : mbRKDInit(false),mContextHandle(NULL)
    {
    }


    CTSE_RKDInit::~CTSE_RKDInit(void)
    {
        RKDDeInit(mContextHandle);
    }

    const wchar_t* CTSE_RKDInit::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RKDInit\",\
                \"TestName\": \"RKDInit\",\
                \"ContextHandleEnvironment\": \"RKD上下文句柄环境变量\"\
                }";
    }

    bool CTSE_RKDInit::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ContextHandleEnvironment"))
        {
            mstrContextHandleEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_RKDInit::TSE_Run()
    {
        if (mbRKDInit) return true;

        mContextHandle = RKDInit();
        if (NULL == mContextHandle) 
        {
            LogE(L"RKDInit failed\r\n");
			return false;
        } 
        else 
        {
            RKDSetLogEn(mContextHandle, NULL, 0);
            CCJGW_RKDeviceHelp::mhRKDContextHandle = mContextHandle;
        }
        mbRKDInit = true;

        if (!mstrContextHandleEnvironment.empty()) GetGlobalEnvironment()->PutInt(mstrContextHandleEnvironment,(int)mContextHandle);

        return true;
    }
}
