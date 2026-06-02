#pragma once
#include <string>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include "CJGW_MES_CPK_LOG_DEFINE.h"
#include <vector>

namespace JGW
{
    class CCJGW_MESQSEQCpkLog
    {
    public:
        CCJGW_MESQSEQCpkLog(void);
        ~CCJGW_MESQSEQCpkLog(void);

        std::wstring GetCPKLogFilePath();
    public:
        //! 
        bool BuildTestStation(const std::wstring& strTaskName,const std::wstring& strOperation,const std::wstring& strUserName,const std::wstring& strSerailNumber,bool bTestStatus = true);
        //! 
        void BuildLotinfo(const std::wstring& strSerailNumber,int nTotalTime,bool bTestStatus = true);
        //! 
        void BuildTDSCDMATestInfo(std::vector<S_TDSCDMA_TRX_CPK_INFO>& vsTDSCDMATRxCPKInfo);
        //! 
        void BuildWCDMATestInfo(std::vector<S_WCDMA_TRX_CPK_INFO>& vsWCDMATRxCPKInfo);
        //!
        void BuildLTETestInfo(std::vector<S_LTE_TRX_CPK_INFO>& vsLTETRxCPKInfo);
    private:
        std::wstring GetTestMetricKeyValue(const S_TEST_METRIC& sTestMetric);
        void WriteTestMetricKeyToCPK(const std::wstring& strAppName,const std::wstring& strKeyName,const S_TEST_METRIC& sTestMetric);
    private:
        std::wstring mstrCpkLogTemplate;
        std::wstring mstrCPKLogFilePath;
        CCJGW_ConfigIni mConfigIni;
    };
}