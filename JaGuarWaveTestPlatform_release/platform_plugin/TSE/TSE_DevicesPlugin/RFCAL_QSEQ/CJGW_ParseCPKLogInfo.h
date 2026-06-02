#pragma once
#include <vector>
#include "CJGW_MES_CPK_LOG_DEFINE.h"

namespace JGW
{
    class CCJGW_ParseCPKLogInfo
    {
    public:
        CCJGW_ParseCPKLogInfo(void);
        ~CCJGW_ParseCPKLogInfo(void);
        //! 解析QSEQ测试信息
        void StartHandleQSEQTestInfo(const wchar_t* szQSEQTestInfo);
        //! 结束QSEQ测试信息解析
        void EndHandleQSEQTestInfo();
        //! 
        std::vector<S_LTE_TRX_CPK_INFO>& GetLTECPKLogInfos();
        //! 
        std::vector<S_WCDMA_TRX_CPK_INFO>& GetWCDMACPKLogInfos();
        //!
        std::vector<S_TDSCDMA_TRX_CPK_INFO>& GetTDSCDMACPKLogInfos();
    private: //! LTE
        //! 解析QSEQ LTE测试信息
        void HandleMesLTETestInfo(const wchar_t* szLTETestInfo);
        //! 解析LTE频段信息
        void PareseLTEBandInfo(const wchar_t* szLTETestInfo);
        //! 解析LTE通用测试指标
        void HandleTestLTEMetric(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric);
        //! 解析LTE SEM测试指标信息
        void HandleLTETestMetric_SEM(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric);
        //! 解析LTE ACLR测试指标信息
        void HandleTestLTEMetric_ACLR(const wchar_t* szTestMetricInfo);
        //! 解析LTE BLER测试指标信息
        void HandleTestLTEMetric_BLER(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric);
        //! 解析LET EVM测试指标信息
        void HandleLTETestMetric_EVM(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric);
    private: //! WCDMA
        //! 解析QSEQ WCDMA测试信息
        void HandleMesWCDMATestInfo(const wchar_t* szWCDMATestInfo);
        //! 解析WCDMA频段信息
        void PareseWCDMABandInfo(const wchar_t* szWCDMATestInfo);
        //! 解析WCDMA通用测试指标
        void HandleTestWCDMAMetric(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric);
        //! 解析WCDMA BER测试指标信息
        void HandleWCMDATestMetric_SEBER(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric);
    private: //! TDSCDMA
        //! 解析QSEQ TDSCDMA测试信息
        void HandleMesTDSCDMATestInfo(const wchar_t* szTDSCDMATestInfo);
        //! 解析TDSCDMA频段信息
        void PareseTDSCDMABandInfo(const wchar_t* szTDSCDMATestInfo);
        //! 解析TDSCDMA频段信息
        void HandleTestTDSCDMAMetric(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric);
        //! 解析TDSCDMA BLER测试指标信息
        void HandleTDSCDMATestMetric_BLER(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric);
    private: //! COMMON FUNC
        //! 初始化测试指标信息
        void HandleTestMetricInit(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric);
        //! 格式化测试指标信息
        void HandleTestMetricFormat(const wchar_t* szTestMetricInfo,std::vector<std::wstring>& vMetric);
        //! 格式化测试指标信息
        void HandleTestMetricFormat_Float(const wchar_t* szTestMetricInfo,std::vector<float>& vMetric);
    private:
        S_TDSCDMA_TRX_CPK_INFO msTDSTRxCPKInfo;
        S_WCDMA_TRX_CPK_INFO msWCDMATRxCPKInfo;
        S_LTE_TRX_CPK_INFO msLTETRxCPKInfo;
        std::vector<S_TDSCDMA_TRX_CPK_INFO> mvTDSCDMATRxCPKInfo;
        std::vector<S_WCDMA_TRX_CPK_INFO> mvWCDMATRxCPKInfo;
        std::vector<S_LTE_TRX_CPK_INFO> mvLTETRxCPKInfo;
    };
}