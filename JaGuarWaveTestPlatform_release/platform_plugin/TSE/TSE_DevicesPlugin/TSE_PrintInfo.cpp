#include "StdAfx.h"
#include "TSE_PrintInfo.h"
#include <TSE_FoundationLib/TSE_FoundationLib_Define.h>
namespace JGW
{
    CTSE_PrintInfo::CTSE_PrintInfo(void)
    {
    }


    CTSE_PrintInfo::~CTSE_PrintInfo(void)
    {
    }

    const wchar_t* CTSE_PrintInfo::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_PrintInfo\",\
                \"TestName\": \"Print Log Info\",\
                \"EnvPrintCommand\": \"Environment Command String\"\
                }";
    }

    bool CTSE_PrintInfo::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvPrintCommand"))
        {
            mstrEnvPrintCommand = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_PrintInfo::TSE_Run()
    {
        std::wstring strEnvPrintCommand(mstrEnvPrintCommand);
        GetEnvironmentCommandString(strEnvPrintCommand);
        LogI(strEnvPrintCommand.c_str());
        LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,strEnvPrintCommand.c_str());
        LogU(E_TSE_SHOW_CONTENT_INFO,strEnvPrintCommand.c_str());
        return true;
    }
}

