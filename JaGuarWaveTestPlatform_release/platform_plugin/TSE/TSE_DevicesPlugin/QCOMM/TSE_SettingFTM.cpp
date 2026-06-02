#include "StdAfx.h"
#include "TSE_SettingFTM.h"
#include "../TSE_DevicesGlobalResource.h"
namespace JGW
{
    CTSE_SettingFTM::CTSE_SettingFTM(void)
    {
    }


    CTSE_SettingFTM::~CTSE_SettingFTM(void)
    {
    }

    const wchar_t* CTSE_SettingFTM::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SettingFTM\",\
                \"TestName\": \"Set FTM\"\
                }";
    }

    bool CTSE_SettingFTM::TSE_Run()
    {
        for (int i = 0;i < 4;i ++)
        {
            if (CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->EnterFTMMode())
            {
                return true;
            }
            Sleep(1000);
        }
        return false;
    }
}
