#pragma once
#include "JGW_Phone.h"

namespace JGW
{
    class CCJGW_WlanConfigFile
    {
    public:
        CCJGW_WlanConfigFile(void);
        ~CCJGW_WlanConfigFile(void);
    public:
        void SetWlanConfigFilePath(const std::wstring& strWlanConfigFilePath);
    public:
        bool GetTxParamToPatnName(const std::wstring& strPathName,S_TxTestParam& sTxTestParam);
        bool GetTestingStandardToPatnName(const std::wstring& strPathName,S_TX_TestingStandard& sTxTestingStandard);
    private:
        std::wstring mstrWlanConfigFilePath;
    };

}

