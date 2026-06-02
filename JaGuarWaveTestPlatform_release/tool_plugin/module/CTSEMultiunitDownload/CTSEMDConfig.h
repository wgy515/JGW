#pragma once
#include "CTSEMultiunitDonwloadDefine.h"

namespace JGW
{
    class CCTSEMDConfig
    {
    public:
        CCTSEMDConfig(void);
        ~CCTSEMDConfig(void);
    public:
        bool LoadMultiunitDownloadConfig(const std::wstring& strConfigPath);

        S_TSE_MULTIUNIT_DOWNLOAD_CONFIG& GetSTSEMultiunitDownalodConfig() { return msTseMultiunitDownloadConfig;}

        void SaveMultiunitDownloadConfig();
    private:
        std::wstring mstrConfigPath;
        S_TSE_MULTIUNIT_DOWNLOAD_CONFIG msTseMultiunitDownloadConfig;
    };
}