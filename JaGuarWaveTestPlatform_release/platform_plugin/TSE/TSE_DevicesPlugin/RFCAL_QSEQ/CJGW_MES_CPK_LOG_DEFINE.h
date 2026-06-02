#pragma once
#include <string>

namespace JGW
{
    enum E_CPK_TEST_STATUS
    {
        E_NOT_CPK_TEST_STATUS = 0, //! 未测试
        E_PASS_CPK_TEST_STATUS, //! 测试PASS状态
        E_FAIL_CPK_TEST_STATUS //! 测试FAIL状态
    };
    //! 测试指标
    struct S_TEST_METRIC
    {
        E_CPK_TEST_STATUS meCPKTestStatus; //! 测试状态
        std::wstring mstrTestValue; //! 测试值
        std::wstring mstrMinValue; //! 测试最小值
        std::wstring mstrMaxValue; //! 测试最大值
    };

    enum E_WCDMA_CPK_TEST_PROJECT
    {
        E_WCDMA_CPK_TX_POWER = 0,
        E_WCDMA_CPK_EVM_RMS,
        E_WCDMA_CPK_MAG_ERROR_RMS,
        E_WCDMA_CPK_PHASE_ERROR_RMS,
        E_WCDMA_CPK_FREQ_ERROR_RMS,
        E_WCDMA_CPK_ACLR_MINUS10,
        E_WCDMA_CPK_ACLR_MINUS5,
        E_WCDMA_CPK_ACLR_PLUS5,//! + 5 
        E_WCDMA_CPK_ACLR_PLUS10,//! + 10 
        E_WCDMA_CPK_SEBER
    };

    struct S_WCDMA_TRX_CPK_INFO
    {
        int mnBand;
        int mnChannel;
        bool mbIsTest; //! 是否测试下面相关的哪一项测试指标

        S_TEST_METRIC mvWCMDATRxCPKTest[E_WCDMA_CPK_SEBER + 1];
        void Init()
        {
            mnChannel = 0;
            mnBand = 0;
            mbIsTest = false;
            for (int i = 0;i <= E_WCDMA_CPK_SEBER;i ++)
            {
                mvWCMDATRxCPKTest[i].meCPKTestStatus = E_NOT_CPK_TEST_STATUS;
            }
        }
    };

    enum E_LTE_CPK_TEST_PROJECT
    {
        E_LTE_CPK_TX_POWER = 0,
        E_LTE_CPK_FREQ_ERROR,
        E_LTE_CPK_EVM_POWER,
        E_LTE_CPK_SEM,
        E_LTE_CPK_ACLR_OFFSET_MINUS2,
        E_LTE_CPK_ACLR_OFFSET_MINUS1,
        E_LTE_CPK_ACLR_OFFSET_PLUS1,
        E_LTE_CPK_ACLR_OFFSET_PLUS2,
        E_LTE_CPK_BLER
    };

    struct S_LTE_TRX_CPK_INFO
    {
        bool mbIsTest; //! 是否测试下面相关的哪一项测试指标
        int mnBand;
        int mnChannel;
        int mnStartRB;
        int mnNumRB;
        int mnSelectedRxChain;
        std::wstring mstrMCS;
        S_TEST_METRIC mvLTETestMetric[E_LTE_CPK_BLER + 1];

        void Init()
        {
            mbIsTest = false;
            mnChannel = 0;
            mnBand = 0;
            mnStartRB = 0;
            mnNumRB = 0;
            mnSelectedRxChain = 0;
            mstrMCS = L"QPSK";
            
            for (int i = 0;i <= E_LTE_CPK_BLER;i ++)
            {
                mvLTETestMetric[i].meCPKTestStatus = E_NOT_CPK_TEST_STATUS;
            }
        }
    };

    enum E_TDSCDMA_CPK_TEST_PROJECT
    {
        E_TDSCDMA_CPK_TX_POWER = 0,
        E_TDSCDMA_CPK_EVM_RMS,
        E_TDSCDMA_CPK_PCDE_RMS,
        E_TDSCDMA_CPK_PHASE_ERROR_RMS,
        E_TDSCDMA_CPK_FREQ_ERROR_RMS,
        E_TDSCDMA_CPK_ACLR_MINUS32,
        E_TDSCDMA_CPK_ACLR_MINUS16,
        E_TDSCDMA_CPK_ACLR_PLUS16,//! + 5 
        E_TDSCDMA_CPK_ACLR_PLUS32,//! + 10 
        E_TDSCDMA_CPK_BLER
    };

    struct S_TDSCDMA_TRX_CPK_INFO
    {
        int mnBand;
        int mnChannel;
        bool mbIsTest; //! 是否测试下面相关的哪一项测试指标

        S_TEST_METRIC mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_BLER + 1];
        void Init()
        {
            mnChannel = 0;
            mnBand = 0;
            mbIsTest = false;
            for (int i = 0;i <= E_TDSCDMA_CPK_BLER;i ++)
            {
                mvTDSCDMATRxCPKTest[i].meCPKTestStatus = E_NOT_CPK_TEST_STATUS;
            }
        }
    };
}