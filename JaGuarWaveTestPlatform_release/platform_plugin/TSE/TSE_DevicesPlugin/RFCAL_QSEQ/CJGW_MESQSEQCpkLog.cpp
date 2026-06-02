#include "StdAfx.h"
#include "CJGW_MESQSEQCpkLog.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#define CPK_LOG_TEMPLATE_FILE_NAME L"cpk_template.txt"
namespace JGW
{
    std::wstring mstrTestFolder;
    CCJGW_MESQSEQCpkLog::CCJGW_MESQSEQCpkLog(void)
    {
        if (mstrTestFolder.empty())
        {
            mstrTestFolder = JGW_GetApplicationFolder();
            mstrTestFolder += L"TestLog\\CPK\\";
        }   
        mstrCpkLogTemplate = JGW_GetApplicationFolder();
        mstrCpkLogTemplate += CPK_LOG_TEMPLATE_FILE_NAME;
    }


    CCJGW_MESQSEQCpkLog::~CCJGW_MESQSEQCpkLog(void)
    {
    }

    std::wstring CCJGW_MESQSEQCpkLog::GetCPKLogFilePath()
    {
        return mstrCPKLogFilePath;
    }

    bool CCJGW_MESQSEQCpkLog::BuildTestStation(const std::wstring& strTaskName,const std::wstring& strOperation,const std::wstring& strUserName,const std::wstring& strSerailNumber,bool bTestStatus /* = true */)
    {
        JGW_FormatWString(mstrCPKLogFilePath,L"%s%s\\%s\\%s_%s_%s_%s.txt",mstrTestFolder.c_str(),strTaskName.c_str(),JGW_GetTimeFolder().c_str(),strOperation.c_str(),strSerailNumber.c_str(),JGW_GetCurrentTimeStampYMDHMS().c_str(),bTestStatus?L"pass":L"fail");
        JGW_CreateDirectory(mstrCPKLogFilePath.c_str(),false);
        CopyFile(mstrCpkLogTemplate.c_str(),mstrCPKLogFilePath.c_str(),FALSE);
        if (!mConfigIni.InitIniFilePath(mstrCPKLogFilePath.c_str())) return false;

        mConfigIni.SetIniKeyValue(L"Test_Station",L"OrderID",strTaskName.c_str());
        mConfigIni.SetIniKeyValue(L"Test_Station",L"OperationID",strOperation.c_str());
        mConfigIni.SetIniKeyValue(L"Test_Station",L"UserName",strUserName.c_str());
        return true;
    }

    void CCJGW_MESQSEQCpkLog::BuildLotinfo(const std::wstring& strSerailNumber,int nTotalTime,bool bTestStatus /* = true */)
    {
        mConfigIni.SetIniKeyValue(L"LOTINFO",L"TestStartTime",JGW_GetCPKTestStartTime().c_str());
        mConfigIni.SetIniKeyValue(L"LOTINFO",L"TestSN",strSerailNumber.c_str());
        mConfigIni.SetIniKeyValue(L"LOTINFO",L"TestTotTime",nTotalTime);
        mConfigIni.SetIniKeyValue(L"LOTINFO",L"TestResult",bTestStatus?L"PASS":L"FAIL");
    }

    std::wstring CCJGW_MESQSEQCpkLog::GetTestMetricKeyValue(const S_TEST_METRIC& sTestMetric)
    {
        std::wstring strKeyValue;
        JGW_FormatWString(strKeyValue,L"%s,%s,%s,%s",(E_PASS_CPK_TEST_STATUS == sTestMetric.meCPKTestStatus)?L"PASS":L"FAIL",sTestMetric.mstrTestValue.c_str(),sTestMetric.mstrMinValue.c_str(),sTestMetric.mstrMaxValue.c_str());
        return strKeyValue;
    }

    void CCJGW_MESQSEQCpkLog::WriteTestMetricKeyToCPK(const std::wstring& strAppName,const std::wstring& strKeyName,const S_TEST_METRIC& sTestMetric)
    {
        if (E_NOT_CPK_TEST_STATUS == sTestMetric.meCPKTestStatus) return ;
        mConfigIni.SetIniKeyValue(strAppName.c_str(),strKeyName.c_str(),GetTestMetricKeyValue(sTestMetric).c_str());
    }

    void CCJGW_MESQSEQCpkLog::BuildTDSCDMATestInfo(std::vector<S_TDSCDMA_TRX_CPK_INFO>& vsTDSCDMATRxCPKInfo)
    {
        //! TDSCDMA^Band=2^Channel=9400
        std::wstring strAppName,strKeyName,strKeyValue;
        for (std::vector<S_TDSCDMA_TRX_CPK_INFO>::iterator it = vsTDSCDMATRxCPKInfo.begin();
            it != vsTDSCDMATRxCPKInfo.end();
            ++ it)
        {
            //! TX
            if (E_NOT_CPK_TEST_STATUS == it->mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_BLER].meCPKTestStatus)
            {
                JGW_FormatWString(strAppName,L"TDSCDMA^Band=%d^Channel=%d",it->mnBand,it->mnChannel);
                //! TX POWER
                WriteTestMetricKeyToCPK(strAppName,L"TxPower",it->mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_TX_POWER]);
                WriteTestMetricKeyToCPK(strAppName,L"EVM_RMS_TargetPower:30",it->mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_EVM_RMS]);
                WriteTestMetricKeyToCPK(strAppName,L"PCDE_TargetPower:30",it->mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_PCDE_RMS]);
                WriteTestMetricKeyToCPK(strAppName,L"Phase_Error_RMS_TargetPower:30",it->mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_PHASE_ERROR_RMS]);
                WriteTestMetricKeyToCPK(strAppName,L"Freq_Error_RMS_TargetPower:30",it->mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_FREQ_ERROR_RMS]);
                WriteTestMetricKeyToCPK(strAppName,L"ACLR_-3.2M",it->mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_ACLR_MINUS32]);
                WriteTestMetricKeyToCPK(strAppName,L"ACLR_-1.6M",it->mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_ACLR_MINUS16]);
                WriteTestMetricKeyToCPK(strAppName,L"ACLR_1.6M",it->mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_ACLR_PLUS16]);
                WriteTestMetricKeyToCPK(strAppName,L"ACLR_3.2M",it->mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_ACLR_PLUS32]);
            }//! RX
            else
            {
                JGW_FormatWString(strAppName,L"TDSCDMA_RX^Band=%d^",it->mnBand);
                JGW_FormatWString(strKeyName,L"BLER_Channel:%d",it->mnChannel);
                mConfigIni.SetIniKeyValue(strAppName.c_str(),strKeyName.c_str(),GetTestMetricKeyValue(it->mvTDSCDMATRxCPKTest[E_TDSCDMA_CPK_BLER]).c_str());
            }
        }
    }

    void CCJGW_MESQSEQCpkLog::BuildWCDMATestInfo(std::vector<S_WCDMA_TRX_CPK_INFO>& vsWCDMATRxCPKInfo)
    {
        //! WCDMA^Band=2^Channel=9400
        std::wstring strAppName,strKeyName,strKeyValue;
        for (std::vector<S_WCDMA_TRX_CPK_INFO>::iterator it = vsWCDMATRxCPKInfo.begin();
            it != vsWCDMATRxCPKInfo.end();
            ++ it)
        {
            //! TX
            if (E_NOT_CPK_TEST_STATUS == it->mvWCMDATRxCPKTest[E_WCDMA_CPK_SEBER].meCPKTestStatus)
            {
                JGW_FormatWString(strAppName,L"WCDMA^Band=%d^Channel=%d",it->mnBand,it->mnChannel);
                //! TX POWER
                WriteTestMetricKeyToCPK(strAppName,L"TxPower",it->mvWCMDATRxCPKTest[E_WCDMA_CPK_TX_POWER]);
                WriteTestMetricKeyToCPK(strAppName,L"EVM_RMS_TargetPower:30",it->mvWCMDATRxCPKTest[E_WCDMA_CPK_EVM_RMS]);
                WriteTestMetricKeyToCPK(strAppName,L"Mag_Error_RMS_TargetPower:30",it->mvWCMDATRxCPKTest[E_WCDMA_CPK_MAG_ERROR_RMS]);
                WriteTestMetricKeyToCPK(strAppName,L"Phase_Error_RMS_TargetPower:30",it->mvWCMDATRxCPKTest[E_WCDMA_CPK_PHASE_ERROR_RMS]);
                WriteTestMetricKeyToCPK(strAppName,L"Freq_Error_RMS_TargetPower:30",it->mvWCMDATRxCPKTest[E_WCDMA_CPK_FREQ_ERROR_RMS]);
                WriteTestMetricKeyToCPK(strAppName,L"ACLR_-10M",it->mvWCMDATRxCPKTest[E_WCDMA_CPK_ACLR_MINUS10]);
                WriteTestMetricKeyToCPK(strAppName,L"ACLR_-5M",it->mvWCMDATRxCPKTest[E_WCDMA_CPK_ACLR_MINUS5]);
                WriteTestMetricKeyToCPK(strAppName,L"ACLR_5M",it->mvWCMDATRxCPKTest[E_WCDMA_CPK_ACLR_PLUS5]);
                WriteTestMetricKeyToCPK(strAppName,L"ACLR_10M",it->mvWCMDATRxCPKTest[E_WCDMA_CPK_ACLR_PLUS10]);
            }//! RX
            else
            {
                JGW_FormatWString(strAppName,L"WCDMA_RX^Band=%d^",it->mnBand);
                JGW_FormatWString(strKeyName,L"SE_BER_Channel:%d",it->mnChannel);
                mConfigIni.SetIniKeyValue(strAppName.c_str(),strKeyName.c_str(),GetTestMetricKeyValue(it->mvWCMDATRxCPKTest[E_WCDMA_CPK_SEBER]).c_str());
            }
        }
    }

    void CCJGW_MESQSEQCpkLog::BuildLTETestInfo(std::vector<S_LTE_TRX_CPK_INFO>& vsLTETRxCPKInfo)
    {
        std::wstring strAppName,strKeyName,strKeyValue;
        for (std::vector<S_LTE_TRX_CPK_INFO>::iterator it = vsLTETRxCPKInfo.begin();
            it != vsLTETRxCPKInfo.end();
            ++ it)
        {
            //! TX
            if (E_NOT_CPK_TEST_STATUS == it->mvLTETestMetric[E_LTE_CPK_BLER].meCPKTestStatus)
            {
                JGW_FormatWString(strAppName,L"LTE^Band=%d^Channel=%d_ULStartRB=%d_ULNumRB=%d_MCS=%s",it->mnBand,it->mnChannel,it->mnStartRB,it->mnNumRB,it->mstrMCS.c_str());
                WriteTestMetricKeyToCPK(strAppName,L"AvgTxPwr",it->mvLTETestMetric[E_LTE_CPK_TX_POWER]);
                WriteTestMetricKeyToCPK(strAppName,L"AvgFreqError_Tx_Pwr:23.0",it->mvLTETestMetric[E_LTE_CPK_FREQ_ERROR]);
                WriteTestMetricKeyToCPK(strAppName,L"EVM_Tx_Pwr:23.0",it->mvLTETestMetric[E_LTE_CPK_EVM_POWER]);
                WriteTestMetricKeyToCPK(strAppName,L"SEM_Tx_Pwr:23.0",it->mvLTETestMetric[E_LTE_CPK_SEM]);
                WriteTestMetricKeyToCPK(strAppName,L"UTRA_ACLR_Offset:-2",it->mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS2]);
                WriteTestMetricKeyToCPK(strAppName,L"UTRA_ACLR_Offset:-1",it->mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_MINUS1]);
                WriteTestMetricKeyToCPK(strAppName,L"UTRA_ACLR_Offset:1",it->mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS1]);
                WriteTestMetricKeyToCPK(strAppName,L"UTRA_ACLR_Offset:2",it->mvLTETestMetric[E_LTE_CPK_ACLR_OFFSET_PLUS2]);

            } //! RX
            else
            {
                JGW_FormatWString(strAppName,L"LTE_RX^Band=%d^Channel=%d^",it->mnBand,it->mnChannel);
                JGW_FormatWString(strKeyName,L"BLER_MCS:%s_RXChain:%d_ULStartRB:%d_ULNumRB:%d",it->mstrMCS.c_str(),it->mnSelectedRxChain,it->mnStartRB,it->mnNumRB);
                mConfigIni.SetIniKeyValue(strAppName.c_str(),strKeyName.c_str(),GetTestMetricKeyValue(it->mvLTETestMetric[E_LTE_CPK_BLER]).c_str());
            }
        }
    }
}