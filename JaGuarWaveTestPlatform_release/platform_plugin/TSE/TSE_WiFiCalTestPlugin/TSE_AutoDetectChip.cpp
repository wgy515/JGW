#include "StdAfx.h"
#include "TSE_AutoDetectChip.h"

namespace JGW
{
    CTSE_AutoDetectChip::CTSE_AutoDetectChip(void)
    {
    }


    CTSE_AutoDetectChip::~CTSE_AutoDetectChip(void)
    {
    }

    bool CTSE_AutoDetectChip::TSE_Run()
    {
        std::string strChipInfo;
        if (!PHONE->AutoDetectChip(strChipInfo))
        {
            LogE(L"Load Wlan Chip Fail");
            return false;
        }
        LogI_F("Auto detect chip : %s",strChipInfo);
        return true;
    }
}
