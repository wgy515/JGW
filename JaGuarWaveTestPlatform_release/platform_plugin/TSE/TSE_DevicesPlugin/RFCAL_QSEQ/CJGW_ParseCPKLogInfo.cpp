#include "StdAfx.h"
#include "CJGW_ParseCPKLogInfo.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_ParseCPKLogInfo::CCJGW_ParseCPKLogInfo(void)
    {
        msWCDMATRxCPKInfo.Init();
        msLTETRxCPKInfo.Init();
        msTDSTRxCPKInfo.Init();
    }


    CCJGW_ParseCPKLogInfo::~CCJGW_ParseCPKLogInfo(void)
    {
    }

    std::vector<S_LTE_TRX_CPK_INFO>& CCJGW_ParseCPKLogInfo::GetLTECPKLogInfos()
    {
        return mvLTETRxCPKInfo;
    }

    std::vector<S_WCDMA_TRX_CPK_INFO>& CCJGW_ParseCPKLogInfo::GetWCDMACPKLogInfos()
    {
        return mvWCDMATRxCPKInfo;
    }

    std::vector<S_TDSCDMA_TRX_CPK_INFO>& CCJGW_ParseCPKLogInfo::GetTDSCDMACPKLogInfos()
    {
        return mvTDSCDMATRxCPKInfo;
    }

    //! 确认相关参数指标都已经加入OK
    void CCJGW_ParseCPKLogInfo::EndHandleQSEQTestInfo()
    {
        if (msTDSTRxCPKInfo.mbIsTest) mvTDSCDMATRxCPKInfo.push_back(msTDSTRxCPKInfo);
        msTDSTRxCPKInfo.Init();

        if (msWCDMATRxCPKInfo.mbIsTest) mvWCDMATRxCPKInfo.push_back(msWCDMATRxCPKInfo);
        msWCDMATRxCPKInfo.Init();

        if (msLTETRxCPKInfo.mbIsTest) mvLTETRxCPKInfo.push_back(msLTETRxCPKInfo);
        msLTETRxCPKInfo.Init();

    }

    void CCJGW_ParseCPKLogInfo::StartHandleQSEQTestInfo(const wchar_t* szQSEQTestInfo)
    {
        //! TX 测试
        if (NULL != wcsstr(szQSEQTestInfo,L"Measure EQP")
            || NULL != wcsstr(szQSEQTestInfo,L"Measure MOB: Report")
            || NULL != wcsstr(szQSEQTestInfo,L"Measure MOB"))
        {
            //! 处理WCDMA CPK数据解析
            if (NULL != wcsstr(szQSEQTestInfo,L"WCDMA"))
            {
                HandleMesWCDMATestInfo(szQSEQTestInfo);
            }
            //! 处理LTE CPK数据解析
            else if (NULL != wcsstr(szQSEQTestInfo,L"LTE"))
            {
                HandleMesLTETestInfo(szQSEQTestInfo);
            }
            else if (NULL != wcsstr(szQSEQTestInfo,L"TDSCDMA"))
            {
                HandleMesTDSCDMATestInfo(szQSEQTestInfo);
            }
        }
        else
        {
            return ;
        }
    }

    void CCJGW_ParseCPKLogInfo::PareseLTEBandInfo(const wchar_t* szLTETestInfo)
    {
        //! 确认是否测试TX POWER -> 指标只取MAX POWER状态下的测试指标
        if (E_NOT_CPK_TEST_STATUS != msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_TX_POWER].meCPKTestStatus)
        {
            if (_ttoi(msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_TX_POWER].mstrMinValue.c_str()) < 18) msLTETRxCPKInfo.mbIsTest = false;
        }
        //! 是否测试 屏蔽掉多余的频段信息上报
        if (msLTETRxCPKInfo.mbIsTest) mvLTETRxCPKInfo.push_back(msLTETRxCPKInfo);
        //! 
        msLTETRxCPKInfo.Init();
        //! szLTETestInfo : NSVFS LTE Measure EQP: BAND_CHANNEL      = 34,36300,0,1,QPSK,1
        //! band,channel startrb numrb mcs rxchain
        const wchar_t* pTemp = wcsstr(szLTETestInfo,L"=");
        if (!pTemp) return ;
        pTemp += 1;
        if (!pTemp || '\0' == pTemp[0]) return ;

        std::vector<std::wstring> vstrTemp;
        JGW_ParserStrW(pTemp,L",",vstrTemp);
        if (vstrTemp.size() < 5) return ;

        msLTETRxCPKInfo.mnBand = _ttoi(vstrTemp[0].c_str());
        msLTETRxCPKInfo.mnChannel = _ttoi(vstrTemp[1].c_str());
        msLTETRxCPKInfo.mnStartRB = _ttoi(vstrTemp[2].c_str());
        msLTETRxCPKInfo.mnNumRB = _ttoi(vstrTemp[3].c_str());
        msLTETRxCPKInfo.mstrMCS = vstrTemp[4].c_str();

        if (vstrTemp.size() > 5) msLTETRxCPKInfo.mnSelectedRxChain = _ttoi(vstrTemp[5].c_str()) - 1;
    }

    void CCJGW_ParseCPKLogInfo::HandleTestLTEMetric(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
    {
        msLTETRxCPKInfo.mbIsTest = true;
        HandleTestMetricInit(szTestMetricInfo,sTestMetric);
        std::vector<std::wstring> vstrTemp;
        HandleTestMetricFormat(szTestMetricInfo,vstrTemp);

        if (vstrTemp.size() < 3) return ;
        sTestMetric.mstrTestValue = vstrTemp[0];
        sTestMetric.mstrMinValue = vstrTemp[1];
        sTestMetric.mstrMaxValue = vstrTemp[2];
        //n/a
        JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"n/a",L"");
        JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"---",L"");
    }

    void CCJGW_ParseCPKLogInfo::HandleLTETestMetric_SEM(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
    {
        msLTETRxCPKInfo.mbIsTest = true;

        //! SEM (dB)     = OK,-26.34,-19.15,-16.25,-27.88,-26.57,-15.71,-19.62,-25.71
        std::vector<float> vfTemp;
        HandleTestMetricFormat_Float(szTestMetricInfo,vfTemp);
        sTestMetric.mstrMinValue = L"1";
        sTestMetric.mstrMaxValue = L"1";

        if (vfTemp[0] < -23.50 
            && vfTemp[1] < -11.50
            && vfTemp[2] < -8.50
            && vfTemp[3] < -16.50
            && vfTemp[4] < -16.50
            && vfTemp[5] < -8.50
            && vfTemp[6] < -11.50
            && vfTemp[7] < -23.50)
        {
            sTestMetric.meCPKTestStatus = E_PASS_CPK_TEST_STATUS;
            sTestMetric.mstrTestValue = L"1";
        }
        else
        {
            sTestMetric.meCPKTestStatus = E_FAIL_CPK_TEST_STATUS;
            sTestMetric.mstrTestValue = L"0";
        }
    }
    //! -2 -1 1 2  //! OK,39.96,37.36,32.68,32.81,37.24,39.98
    void CCJGW_ParseCPKLogInfo::HandleTestLTEMetric_ACLR(const wchar_t* szTestMetricInfo)
    {
        msLTETRxCPKInfo.mbIsTest = true;
        std::vector<float> vfTemp;
        std::vector<std::wstring> vstrTemp;
        HandleTestMetricFormat(szTestMetricInfo,vstrTemp);
        HandleTestMetricFormat_Float(szTestMetricInfo,vfTemp);
        if (vfTemp.size() < 6) return ;
        //! Offset  -2
        msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS2].mstrMinValue = L"35.20";
        msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS2].mstrMaxValue = L"";
        msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS2].mstrTestValue = vstrTemp[0];
        if (vfTemp[0] > 35.20)
        {
            msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS2].meCPKTestStatus = E_PASS_CPK_TEST_STATUS;
        }
        else
        {
            msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS2].meCPKTestStatus = E_FAIL_CPK_TEST_STATUS;
        }
        //! Offset -1
        msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS1].mstrMinValue = L"32.20";
        msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS1].mstrMaxValue = L"";
        msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS1].mstrTestValue = vstrTemp[1];
        if (vfTemp[1] > 32.20)
        {
            msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS1].meCPKTestStatus = E_PASS_CPK_TEST_STATUS;
        }
        else
        {
            msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS1].meCPKTestStatus = E_FAIL_CPK_TEST_STATUS;
        }
        //! Offset  1
        msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS1].mstrMinValue = L"32.20";
        msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS1].mstrMaxValue = L"";
        msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS1].mstrTestValue = vstrTemp[4];
        if (vfTemp[4] > 32.20)
        {
            msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS1].meCPKTestStatus = E_PASS_CPK_TEST_STATUS;
        }
        else
        {
            msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS1].meCPKTestStatus = E_FAIL_CPK_TEST_STATUS;
        }
        //! Offset 2
        msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS2].mstrMinValue = L"35.20";
        msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS2].mstrMaxValue = L"";
        msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS2].mstrTestValue = vstrTemp[5];
        if (vfTemp[5] > 35.20)
        {
            msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS2].meCPKTestStatus = E_PASS_CPK_TEST_STATUS;
        }
        else
        {
            msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS2].meCPKTestStatus = E_FAIL_CPK_TEST_STATUS;
        }
    }

    void CCJGW_ParseCPKLogInfo::HandleTestLTEMetric_BLER(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
    {
        msLTETRxCPKInfo.mbIsTest = true;
        HandleTestMetricInit(szTestMetricInfo,sTestMetric);
        std::vector<std::wstring> vstrTemp;
        const wchar_t* szTestMetricInfoTemp = wcsstr(szTestMetricInfo,L"=");
        if (NULL == szTestMetricInfoTemp) return;
        szTestMetricInfoTemp += 1;
        if (!szTestMetricInfoTemp || '\0' == szTestMetricInfoTemp[0]) return ;

        std::wstring strTemp = szTestMetricInfoTemp;
        JGW_ReplaceStringW(strTemp,L",",L" ");
        JGW_ReplaceStringW(strTemp,L"[",L"");
        JGW_ReplaceStringW(strTemp,L"]",L"");
        JGW_ParserStrW(strTemp.c_str(),L" ",vstrTemp);

        if (vstrTemp.size() < 3) return ;

        sTestMetric.mstrTestValue = vstrTemp[0];
        sTestMetric.mstrMinValue = vstrTemp[1];
        sTestMetric.mstrMaxValue = vstrTemp[2];
    }

    void CCJGW_ParseCPKLogInfo::HandleLTETestMetric_EVM(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
    {
        msLTETRxCPKInfo.mbIsTest = true;
        HandleTestMetricInit(szTestMetricInfo,sTestMetric);
        std::vector<std::wstring> vstrTemp;
        HandleTestMetricFormat(szTestMetricInfo,vstrTemp);

        if (vstrTemp.size() < 4) return ;
        sTestMetric.mstrTestValue = vstrTemp[0];
        sTestMetric.mstrMinValue = vstrTemp[2];
        sTestMetric.mstrMaxValue = vstrTemp[3];
        //n/a
        JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"n/a",L"");
        JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"---",L"");
    }

    void CCJGW_ParseCPKLogInfo::HandleMesLTETestInfo(const wchar_t* szMeasLTE)
    {
        const wchar_t* pPos = NULL;
        //! NSVFS LTE Measure EQP: BAND_CHANNEL      = 34,36300,0,50,QPSK,1
        if (NULL != (pPos = wcsstr(szMeasLTE,L"BAND_CHANNEL")))
        {
            PareseLTEBandInfo(pPos);
        }//! NSVFS LTE Measure EQP: PCC UL_Power(avg)= OK,22.46 [19.00, 25.00, PASS]
        else if (NULL != (pPos = wcsstr(szMeasLTE,L"UL_Power(avg)")))
        {
            HandleTestLTEMetric(pPos,msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_TX_POWER]);
        }//! NSVFS LTE Measure EQP: FreqErr (ppm) average = OK,0.0024 [-0.1000, 0.1000, PASS]
        else if (NULL != (pPos = wcsstr(szMeasLTE,L"FreqErr (ppm) average")))
        {
            HandleTestLTEMetric(pPos,msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_FREQ_ERROR]);
        }//! NSVFS LTE Measure EQP: EVM          = OK,4.23,5.15 [---, 17.50, PASS]
        else if (NULL != (pPos = wcsstr(szMeasLTE,L"EVM ")))
        {
            HandleLTETestMetric_EVM(pPos,msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_EVM_POWER]);
        } //! NSVFS LTE Measure EQP: SEM (dB)     = OK,-26.34,-19.15,-16.25,-27.88,-26.57,-15.71,-19.62,-25.71
        else if (NULL != (pPos = wcsstr(szMeasLTE,L"SEM")))
        {
            HandleLTETestMetric_SEM(pPos,msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_SEM]);
        }//! NSVFS LTE Measure EQP: ACLR (dB)    = OK,39.34,37.43,32.30,32.45,36.89,39.92
        else if (NULL != (pPos = wcsstr(szMeasLTE,L"ACLR")))
        {
            HandleTestLTEMetric_ACLR(pPos);
        }//! NSVFS LTE Measure MOB: Carrier[0]_BLER (percent)   = 0.00, [0.00, 3.72, PASS]
        else if (NULL != (pPos = wcsstr(szMeasLTE,L"Carrier[0]_BLER")))
        {
            HandleTestLTEMetric_BLER(pPos,msLTETRxCPKInfo.mvLTETestMetric[E_LTE_CPK_BLER]);
        }
    }

    /***********          WCDMA        **************************/
    void CCJGW_ParseCPKLogInfo::PareseWCDMABandInfo(const wchar_t* szMeasWCDMA)
    {
        //! MAX POWER 测试指标
        if (E_NOT_CPK_TEST_STATUS != msWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_TX_POWER].meCPKTestStatus)
        {
            if (_ttoi(msWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_TX_POWER].mstrMinValue.c_str()) < 20) msWCDMATRxCPKInfo.mbIsTest = false;
        }
        if (msWCDMATRxCPKInfo.mbIsTest) mvWCDMATRxCPKInfo.push_back(msWCDMATRxCPKInfo);
        //!
        msWCDMATRxCPKInfo.Init();
        const wchar_t* pTemp = wcsstr(szMeasWCDMA,L"=");
        if (!pTemp) return ;
        pTemp += 1;
        if (!pTemp || '\0' == pTemp[0]) return ;
        msWCDMATRxCPKInfo.mnBand = _ttoi(pTemp);
        pTemp = wcsstr(pTemp,L",");
        if (!pTemp) return ;
        pTemp += 1;
        if (!pTemp || '\0' == pTemp[0]) return ;
        msWCDMATRxCPKInfo.mnChannel = _ttoi(pTemp);
    }

    void CCJGW_ParseCPKLogInfo::HandleTestWCDMAMetric(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
    {
        msWCDMATRxCPKInfo.mbIsTest = true;
        HandleTestMetricInit(szTestMetricInfo,sTestMetric);
        std::vector<std::wstring> vstrTemp;
        HandleTestMetricFormat(szTestMetricInfo,vstrTemp);

        if (vstrTemp.size() < 3) return ;
        sTestMetric.mstrTestValue = vstrTemp[0];
        sTestMetric.mstrMinValue = vstrTemp[1];
        sTestMetric.mstrMaxValue = vstrTemp[2];
        //n/a
        JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"n/a",L"");
        JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"---",L"");
    }

    void CCJGW_ParseCPKLogInfo::HandleWCMDATestMetric_SEBER(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
    {
        msWCDMATRxCPKInfo.mbIsTest = true;
        HandleTestMetricInit(szTestMetricInfo,sTestMetric);
        std::vector<std::wstring> vstrTemp;
        const wchar_t* szTestMetricInfoTemp = wcsstr(szTestMetricInfo,L"=");
        if (NULL == szTestMetricInfoTemp) return;
        szTestMetricInfoTemp += 1;
        if (!szTestMetricInfoTemp || '\0' == szTestMetricInfoTemp[0]) return ;
        HandleTestMetricFormat(szTestMetricInfoTemp,vstrTemp);
        if (vstrTemp.size() < 4) return ;

        sTestMetric.mstrTestValue = vstrTemp[1];
        sTestMetric.mstrMinValue = vstrTemp[2];
        sTestMetric.mstrMaxValue = vstrTemp[3];
    }

    void CCJGW_ParseCPKLogInfo::HandleMesWCDMATestInfo(const wchar_t* szMeasWCDMA)
    {
        const wchar_t* pPos = NULL;
        //! NSVFS WCDMA Measure EQP: BAND_CHANNEL      = 2,9262
        if (NULL != (pPos = wcsstr(szMeasWCDMA,L"BAND_CHANNEL")))
        {
            PareseWCDMABandInfo(pPos);
        } //! NSVFS WCDMA Measure EQP: UE_Power     = OK,23.26 [22.00, 26.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"UE_Power")))
        {
            HandleTestWCDMAMetric(pPos,msWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_TX_POWER]);
        } //! NSVFS WCDMA Measure EQP: EVM          = OK,3.15 [0.00, 17.50 PASS]
        else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"EVM")))
        {
            HandleTestWCDMAMetric(pPos,msWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_EVM_RMS]);
        } //! NSVFS WCDMA Measure EQP: Mag_Error    = OK,1.73 [0.00, 100.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"Mag_Error")))
        {
            HandleTestWCDMAMetric(pPos,msWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_MAG_ERROR_RMS]);
        } //! NSVFS WCDMA Measure EQP: Phase_Error  = OK,1.51 [0.00, 100.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"Phase_Error")))
        {
            HandleTestWCDMAMetric(pPos,msWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_PHASE_ERROR_RMS]);
        } //! NSVFS WCDMA Measure EQP: Freq_Error   = OK,-0.0043 [-0.1000, 0.1000 PASS]
        else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"Freq_Error")))
        {
            HandleTestWCDMAMetric(pPos,msWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_FREQ_ERROR_RMS]);
        } //! NSVFS WCDMA Measure EQP: ACLR[-10]    = OK,-54.39 [[n/a], -43.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"ACLR[-10]")))
        {
            HandleTestWCDMAMetric(pPos,msWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_ACLR_MINUS10]);
        } //! NSVFS WCDMA Measure EQP: ACLR[-05]    = OK,-44.91 [[n/a], -33.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"ACLR[-05]")))
        {
            HandleTestWCDMAMetric(pPos,msWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_ACLR_MINUS5]);
        } //! NSVFS WCDMA Measure EQP: ACLR[+05]    = OK,-43.18 [[n/a], -33.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"ACLR[+05]")))
        {
            HandleTestWCDMAMetric(pPos,msWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_ACLR_PLUS5]);
        } //! NSVFS WCDMA Measure EQP: ACLR[+10]    = OK,-52.95 [[n/a], -43.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"ACLR[+10]")))
        {
            HandleTestWCDMAMetric(pPos,msWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_ACLR_PLUS10]);
        } //! NSVFS WCDMA Measure MOB: Report: SE_BER(nError,n,percent)= 0,2248,0.00 [0.00, 6.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasWCDMA,L"SE_BER")))
        {
            HandleWCMDATestMetric_SEBER(pPos,msWCDMATRxCPKInfo.mvWCMDATRxCPKTest[E_WCDMA_CPK_SEBER]);
        }
    }

    void CCJGW_ParseCPKLogInfo::PareseTDSCDMABandInfo(const wchar_t* szTDSCDMATestInfo)
    {
        //! MAX POWER 测试指标
        if (E_NOT_CPK_TEST_STATUS != msTDSTRxCPKInfo.mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_TX_POWER].meCPKTestStatus)
        {
            if (_ttoi(msTDSTRxCPKInfo.mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_TX_POWER].mstrMinValue.c_str()) < 20) msTDSTRxCPKInfo.mbIsTest = false;
        }
        if (msTDSTRxCPKInfo.mbIsTest) mvTDSCDMATRxCPKInfo.push_back(msTDSTRxCPKInfo);
        //!
        msTDSTRxCPKInfo.Init();
        const wchar_t* pTemp = wcsstr(szTDSCDMATestInfo,L"=");
        if (!pTemp) return ;
        pTemp += 1;
        if (!pTemp || '\0' == pTemp[0]) return ;
        msTDSTRxCPKInfo.mnBand = _ttoi(pTemp);
        pTemp = wcsstr(pTemp,L",");
        if (!pTemp) return ;
        pTemp += 1;
        if (!pTemp || '\0' == pTemp[0]) return ;
        msTDSTRxCPKInfo.mnChannel = _ttoi(pTemp);
    }

    void CCJGW_ParseCPKLogInfo::HandleTestTDSCDMAMetric(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
    {
        msTDSTRxCPKInfo.mbIsTest = true;
        HandleTestMetricInit(szTestMetricInfo,sTestMetric);
        std::vector<std::wstring> vstrTemp;
        HandleTestMetricFormat(szTestMetricInfo,vstrTemp);

        if (vstrTemp.size() < 3) return ;
        sTestMetric.mstrTestValue = vstrTemp[0];
        sTestMetric.mstrMinValue = vstrTemp[1];
        sTestMetric.mstrMaxValue = vstrTemp[2];
        //n/a
        JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"n/a",L"");
        JGW_ReplaceStringW(sTestMetric.mstrMinValue,L"---",L"");
        JGW_ReplaceStringW(sTestMetric.mstrMaxValue,L"n/a",L"");
        JGW_ReplaceStringW(sTestMetric.mstrMaxValue,L"---",L"");
    }

    void CCJGW_ParseCPKLogInfo::HandleTDSCDMATestMetric_BLER(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
    {
        msTDSTRxCPKInfo.mbIsTest = true;
        HandleTestMetricInit(szTestMetricInfo,sTestMetric);
        std::vector<std::wstring> vstrTemp;
        const wchar_t* szTestMetricInfoTemp = wcsstr(szTestMetricInfo,L"=");
        if (NULL == szTestMetricInfoTemp) return;
        szTestMetricInfoTemp += 1;
        if (!szTestMetricInfoTemp || '\0' == szTestMetricInfoTemp[0]) return ;

        std::wstring strTemp = szTestMetricInfoTemp;
        JGW_ReplaceStringW(strTemp,L",",L" ");
        JGW_ReplaceStringW(strTemp,L"[",L"");
        JGW_ReplaceStringW(strTemp,L"]",L"");
        JGW_ParserStrW(strTemp.c_str(),L" ",vstrTemp);

        if (vstrTemp.size() < 3) return ;

        sTestMetric.mstrTestValue = vstrTemp[0];
        sTestMetric.mstrMinValue = vstrTemp[1];
        sTestMetric.mstrMaxValue = vstrTemp[2];
    }

    void CCJGW_ParseCPKLogInfo::HandleMesTDSCDMATestInfo(const wchar_t* szMeasTDSCDMA)
    {
        const wchar_t* pPos = NULL;
        //! NSVFS TDSCDMA Measure EQP: BAND_CHANNEL      = 34,10054
        if (NULL != (pPos = wcsstr(szMeasTDSCDMA,L"BAND_CHANNEL")))
        {
            PareseTDSCDMABandInfo(pPos);
        } //! NSVFS TDSCDMA Measure EQP: UE_Power     = OK,23.37 [22.00, 26.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasTDSCDMA,L"UE_Power")))
        {
            HandleTestTDSCDMAMetric(pPos,msTDSTRxCPKInfo.mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_TX_POWER]);
        } //! NSVFS TDSCDMA Measure EQP: EVM          = OK,3.30 [0.00, 17.50 PASS]
        else if (NULL != (pPos = wcsstr(szMeasTDSCDMA,L"EVM")))
        {
            HandleTestTDSCDMAMetric(pPos,msTDSTRxCPKInfo.mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_EVM_RMS]);
        } //! RFVFS TDSCDMA Measure EQP: Peak CDE (dB) Avg    = OK,-35.66 [-20.00, ---, PASS]
        else if (NULL != (pPos = wcsstr(szMeasTDSCDMA,L"Peak CDE")))
        {
            HandleTestTDSCDMAMetric(pPos,msTDSTRxCPKInfo.mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_PCDE_RMS]);
        } //! NNSVFS TDSCDMA Measure EQP: Phase_Error  = OK,1.53 [0.00, 224.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasTDSCDMA,L"Phase_Error")))
        {
            HandleTestTDSCDMAMetric(pPos,msTDSTRxCPKInfo.mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_PHASE_ERROR_RMS]);
        } //! NSVFS TDSCDMA Measure EQP: Freq_Error   = OK,0.0008 [-0.1000, 0.1000 PASS]
        else if (NULL != (pPos = wcsstr(szMeasTDSCDMA,L"Freq_Error")))
        {
            HandleTestTDSCDMAMetric(pPos,msTDSTRxCPKInfo.mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_FREQ_ERROR_RMS]);
        } //! NSVFS TDSCDMA Measure EQP: ACLR[-10]    = OK,-54.39 [[n/a], -43.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasTDSCDMA,L"ACLR[-3.2]")))
        {
            HandleTestTDSCDMAMetric(pPos,msTDSTRxCPKInfo.mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_ACLR_MINUS32]);
        } //! NSVFS TDSCDMA Measure EQP: ACLR[-05]    = OK,-44.91 [[n/a], -33.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasTDSCDMA,L"ACLR[-1.6]")))
        {
            HandleTestTDSCDMAMetric(pPos,msTDSTRxCPKInfo.mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_ACLR_MINUS16]);
        } //! NSVFS TDSCDMA Measure EQP: ACLR[+05]    = OK,-43.18 [[n/a], -33.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasTDSCDMA,L"ACLR[+1.6]")))
        {
            HandleTestTDSCDMAMetric(pPos,msTDSTRxCPKInfo.mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_ACLR_PLUS16]);
        } //! NSVFS TDSCDMA Measure EQP: ACLR[+10]    = OK,-52.95 [[n/a], -43.00 PASS]
        else if (NULL != (pPos = wcsstr(szMeasTDSCDMA,L"ACLR[+3.2]")))
        {
            HandleTestTDSCDMAMetric(pPos,msTDSTRxCPKInfo.mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_ACLR_PLUS32]);
        } //! NSVFS TDSCDMA Measure MOB: BLER (percent)   = 0.16, [0.00, 10.00, PASS]
        else if (NULL != (pPos = wcsstr(szMeasTDSCDMA,L"BLER")))
        {
            HandleTDSCDMATestMetric_BLER(pPos,msTDSTRxCPKInfo.mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_BLER]);
        }
    }

    void CCJGW_ParseCPKLogInfo::HandleTestMetricInit(const wchar_t* szTestMetricInfo,S_TEST_METRIC& sTestMetric)
    {
        //! 判断测试状态
        if (NULL != JGW_WStrCaseWStr(szTestMetricInfo,L"PASS"))
        {
            sTestMetric.meCPKTestStatus = E_PASS_CPK_TEST_STATUS;
        }
        else
        {
            sTestMetric.meCPKTestStatus = E_FAIL_CPK_TEST_STATUS;
        }

        sTestMetric.mstrMaxValue = L"";
        sTestMetric.mstrMinValue = L"";
        sTestMetric.mstrTestValue = L"";
    }

    void CCJGW_ParseCPKLogInfo::HandleTestMetricFormat(const wchar_t* szTestMetricInfo,std::vector<std::wstring>& vMetric)
    {
        const wchar_t* pTemp = JGW_WStrCaseWStr(szTestMetricInfo,L",");
        if (NULL == pTemp) return;
        pTemp += 1;
        if (!pTemp || '\0' == pTemp[0]) return ;
        std::wstring strTemp = pTemp;
        JGW_ReplaceStringW(strTemp,L",",L" ");
        JGW_ReplaceStringW(strTemp,L"[",L"");
        JGW_ReplaceStringW(strTemp,L"]",L"");
        JGW_ParserStrW(strTemp.c_str(),L" ",vMetric);
    }

    void CCJGW_ParseCPKLogInfo::HandleTestMetricFormat_Float(const wchar_t* szTestMetricInfo,std::vector<float>& vMetric)
    {
        const wchar_t* pTemp = JGW_WStrCaseWStr(szTestMetricInfo,L",");
        if (NULL == pTemp) return;
        pTemp += 1;
        if (!pTemp || '\0' == pTemp[0]) return ;
        std::wstring strTemp = pTemp;
        JGW_ReplaceStringW(strTemp,L",",L" ");
        JGW_ReplaceStringW(strTemp,L"[",L"");
        JGW_ReplaceStringW(strTemp,L"]",L"");
        JGW_ParserFloatW(strTemp.c_str(),L" ",vMetric);
    }
}