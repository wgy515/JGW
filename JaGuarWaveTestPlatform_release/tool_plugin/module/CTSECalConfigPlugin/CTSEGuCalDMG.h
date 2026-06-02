#pragma once
#include "CTSEGuCalBase.h"
#include "CTSECalConfigDefine.h"
#include <map>
#include <vector>
namespace JGW
{
    class CCTSEGuCalDMG : public CCTSEGuCalBase
    {
    public:
        CCTSEGuCalDMG(void);
        ~CCTSEGuCalDMG(void);
    private:
        bool LoadGuCalConfig(const std::wstring& strGuCalConfigPath,PS_TSE_CALCONFIG_TEST_CONFIG_PARAM psCalConfigTestConfigParam);

        std::wstring SaveDutCalibrationLossFile(const std::wstring& strDutTestDataFilePath);

        std::wstring GetDutTestDataFilePath();
    private:
        void LoadTSESuiteConfigEnvironment(const std::wstring& strCalSuiteXmlFilePath);
        bool GetTSERssiStandard(const std::wstring& strDutTestDataFilePath,std::wstring& strDutTxGuRxRssi,std::wstring& DutRxGuTxRssi);
    private:
        bool ChangeTSEConfigXml(const std::wstring& strTSEConfigXmlPath,std::wstring& strDutTxGuRxRssi,std::wstring& strDutRxGuTxRssi);
        int GetTSERssiStandardValue(std::vector<int>& vValue);
    private:
        std::wstring mstrGlobalEnvironmentConfigXmlPath;
        S_DMG_CAL_CONFIG msDmgCalConfig;
        std::map<std::wstring,std::wstring> mmapCommonEnvironment;
    };
}


