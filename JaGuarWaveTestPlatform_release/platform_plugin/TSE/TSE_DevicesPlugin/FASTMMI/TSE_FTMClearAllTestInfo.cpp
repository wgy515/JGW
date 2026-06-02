#include "StdAfx.h"
#include "TSE_FTMClearAllTestInfo.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_FTMClearAllTestInfo::CTSE_FTMClearAllTestInfo(void)
    {
    }


    CTSE_FTMClearAllTestInfo::~CTSE_FTMClearAllTestInfo(void)
    {
    }

    const wchar_t* CTSE_FTMClearAllTestInfo::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTMClearAllTestInfo\",\
                \"TestName\": \"清空FASTMMI所有测试信息\"\
                }";
    }

    bool CTSE_FTMClearAllTestInfo::TSE_Run()
    {
        CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->FTM_AP_ERASE_ALL_FILES();
        return true;
    }
}