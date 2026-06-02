#include "StdAfx.h"
#include "TSE_GetComputerName.h"

namespace JGW
{
    CTSE_GetComputerName::CTSE_GetComputerName(void)
    {
        TCHAR szComputerName[260] = {0};
        DWORD dwSize = 260;
        GetComputerNameW(szComputerName,&dwSize);
        mstrComputerName = szComputerName;
    }


    CTSE_GetComputerName::~CTSE_GetComputerName(void)
    {
    }

    const wchar_t* CTSE_GetComputerName::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_GetComputerName\",\
                \"TestName\": \"获取电脑名称\",\
                \"ComputerNameEnvironment\": \"Computer Name Environment\"\
                }";
    }

    bool CTSE_GetComputerName::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"ComputerNameEnvironment",strParamName))
        {
            mstrComputerNameEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_GetComputerName::TSE_Run()
    {
        GetGlobalEnvironment()->PutString(mstrComputerNameEnvironment,mstrComputerName);
        return true;
    }
}
