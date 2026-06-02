#include "StdAfx.h"
#include "TSE_HandoverADMode.h"

namespace JGW
{
    CTSE_HandoverADMode::CTSE_HandoverADMode(void) : mADMode(WIFI_STAT_MODE)
    {
    }


    CTSE_HandoverADMode::~CTSE_HandoverADMode(void)
    {
    }

    bool CTSE_HandoverADMode::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_FTM_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (NULL == JGW_WStrComparenoCaseWStr(strParamName,L"ADMode"))
        {
            mADMode = _ttoi(strParamValue);
            if (mADMode > WIFI_STAT_MODE||mADMode < WIFI_AP_MODE) mADMode = WIFI_STAT_MODE;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_HandoverADMode::TSE_Run()
    {
        std::string strJsonCommand = CTSE_FTM_JSONCommand::GetHandoverADModeJson(mADMode);
        CCJGW_FTM_JsonSocket::WriteJsonSocket(strJsonCommand);
        return true;
    }
}