#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "TSE_PerasoDMGTRXTest.h"
namespace JGW
{
    //! DMG金机校准
    class CTSE_PerasoDMGGuCalTest : public CTSE_TestBase
    {
    public:
        CTSE_PerasoDMGGuCalTest(void);
        ~CTSE_PerasoDMGGuCalTest(void);
    private:    
        //! 
        const wchar_t* TSE_GetParamDescription();
        //! 
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        //! 
        virtual bool TSE_Run(void);
        //! 保存测试log
        virtual bool TSE_Exit(void);
    private:
        //! S_DMG_PERASO_INFO
        void InitDMGPerasoInfo(S_DMG_PERASO_INFO& sDMGPerasoInfo);
        //! S_DMG_TRX_RESULT
        void InitDMGTRxResults(LPS_DMG_TRX_RESULT psDMGTRxLogResults);
        //! 
        bool InitTestParamEnvironment();
        //!
        bool DisableTestMode(CCJGW_DonglesComPort& donglesComPort);
        //! 
        bool EnableTestMode(CCJGW_DonglesComPort& donglesComPort);
        //! 
        bool ConfigureChannel(CCJGW_DonglesComPort& donglesComPort,int channel = 1);
        //!
        bool TransmitDMGPackets(CCJGW_DonglesComPort& donglesComPort,const std::wstring& strSector);
        //!
        bool ReceiveDMGPackets(CCJGW_DonglesComPort& donglesComPort,const std::wstring& strSector);
        //! Stop RX and query results after some delay 
        std::string StopRxAndQueryResultsAfterSomeDelay(CCJGW_DonglesComPort& donglesComPort);
        //! 获取金机测试结果指标
        bool GetRxResultsToTestStandards(const std::string& strRxResult,int nRssiStandard,int nFerStandard,int& nRssi,int& FerPerimile);
        //!
        void SendViewTestLog(const std::wstring& strLog);
        //! 
        void SendViewTestLogFormat(const wchar_t* strCustomLog,...);
        //! 金机校准配置
        bool StartDMGTRxAutomaticCalibrationConfigurationTest(CCJGW_DonglesComPort& txDonglesComPort,S_DMG_PERASO_INFO& sTxPerasoInfo,CCJGW_DonglesComPort& rxDonglesComPort,S_DMG_PERASO_INFO& sRxPerasoInfo,std::vector<int>& vRssiRange,LPS_DMG_TRX_RESULT psDMGTRxLogResults);
#if 0
        bool StopTxRxTest(CCJGW_DonglesComPort& donglesComPort);
#endif
        
        //! 
        //bool CheckRxResultsToTestStandards(const std::string& strRxResult,int nRssiStandard,int nFerStandard,S_DMG_TRX_RESULT& sDMGTRxResult);
        
        //!
        //bool StartDMGTRxTest(CCJGW_DonglesComPort& txDonglesComPort,S_DMG_PERASO_INFO& sTxPerasoInfo,CCJGW_DonglesComPort& rxDonglesComPort,S_DMG_PERASO_INFO& sRxPerasoInfo,std::vector<int>& vRssiRange,std::vector<int>& vFerRange,LPS_DMG_TRX_RESULT psDMGTRxLogResults);
        
        
    private:
        bool mbInitializeOnce;
        bool mbTestResult;
        int mnRecvTimeMs;
        int mnPPDUDetecs;
        int mnItemTestErrorRetryCount;
        //std::wstring mstrGuSerialNumber;
        //std::wstring mstrDutSerialNumber;
        //std::wstring mstrGuMacAddress;
        //std::wstring mstrDutMacAddress;
        //std::wstring mstrGuSector;
        //std::wstring mstrDutSector;
        std::wstring mstrPhyTestTxCommand;
        std::wstring mstrPPDUDetectEnv;
        std::wstring mstrGuSerialComPortEnv;
        std::wstring mstrDutSerialComPortEnv;
        std::wstring mstrGuSectorEnv;
        std::wstring mstrDutSectorEnv;
        std::wstring mstrChannelsEnv;

        std::wstring mstrRecvTimeMsEnv;
        std::wstring mstrPhyTestTxCommandEnv;
        std::wstring mstrRssiRxGuTxDutEnv;
        std::wstring mstrRssiTxGuRxDutEnv;
        std::wstring mstrSaveGuCalDataFileEnv;

        std::wstring mstrDMGCsvLogFilePath;

        std::vector<int> mvChannels;
        std::vector<int> mvRssiRxGuTxDut;
        std::vector<int> mvRssiTxGuRxDut;

        S_DMGTRX_LOG msDmgTRxLog;
    };
}


