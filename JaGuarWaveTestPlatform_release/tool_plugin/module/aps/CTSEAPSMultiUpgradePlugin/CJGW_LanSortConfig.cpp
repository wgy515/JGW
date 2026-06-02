#include "StdAfx.h"
#include "CJGW_LanSortConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_LanSortConfig::CCJGW_LanSortConfig(LPS_NET_CARD_PARAM psNetCardParams) : mpsNetCardParams(psNetCardParams)
    {
    }


    CCJGW_LanSortConfig::~CCJGW_LanSortConfig(void)
    {
    }

    bool CCJGW_LanSortConfig::LoadLanSortConfig(const std::wstring& strSortFilePath)
    {
        if (!mConfigIni.InitIniFilePath(strSortFilePath.c_str())) return false;

        std::wstring strTemp;
        for (int i = 1;i <= DOWNLOAD_THREAD_MAX;i ++)
        {
            JGW_FormatWString(strTemp,L"DEV_%d",i);
            mpsNetCardParams[i - 1].mstrDescription = mConfigIni.GetIniKeyValue(strTemp.c_str(),L"Description");
        }

        return true;
    }

    void CCJGW_LanSortConfig::SaveLanSortConfig(LPS_NET_CARD_PARAM psNetCardParam)
    {
        int nIndex = 1;
        for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
        {
            if (&mpsNetCardParams[i] == psNetCardParam)
            {
                nIndex = i + 1;
                break;
            }
        }
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"DEV_%d",nIndex);
        mConfigIni.SetIniKeyValue(strTemp.c_str(),L"Description",psNetCardParam->mstrDescription.c_str());
    }

    void CCJGW_LanSortConfig::ClearLanSortConfig()
    {
        std::wstring strTemp;
        for (int i = 1;i <= DOWNLOAD_THREAD_MAX;i ++)
        {
            JGW_FormatWString(strTemp,L"DEV_%d",i);
            mConfigIni.SetIniKeyValue(strTemp.c_str(),L"Description",L"");
        }
    }
}