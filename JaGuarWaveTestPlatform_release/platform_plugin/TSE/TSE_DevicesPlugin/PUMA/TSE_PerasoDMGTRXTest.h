#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_WindowsFuncPlugin/CJGW_DonglesComPort.h>
#define PERASO_MAX_CHANNEL 6
namespace JGW
{
    typedef struct
    {
        std::wstring mstrSector;
        std::wstring mstrConfigPdbVersion;
        std::wstring mstrSerialNumber;
        std::wstring mstrMacAddress;
        std::wstring mstrFirewareVersion;
    }S_DMG_PERASO_INFO,*LPS_DMG_PERASO_INFO;

    typedef struct
    {
        int mnChannel;
        int mnRssi;
        int mnFerPerimile;
        int mnUgain;
        std::wstring mstrRssi;
        std::wstring mstrFerPermile;
        std::wstring mstrUgain;
    }S_DMG_TRX_RESULT,*LPS_DMG_TRX_RESULT;

    typedef struct
    {
        S_DMG_PERASO_INFO msGuDmgPerasoInfo;
        S_DMG_PERASO_INFO msDutDmgPerasoInfo;
        S_DMG_TRX_RESULT mvsDutTxGuRxResults[PERASO_MAX_CHANNEL];
        S_DMG_TRX_RESULT mvsGuTxDutRxResults[PERASO_MAX_CHANNEL];
        //! std::map<int,S_DMG_TRX_RESULT> mmapDutTxGutRxResults;
        //! std::map<int,S_DMG_TRX_RESULT> mmapGuTxDutRxResults;
    }S_DMGTRX_LOG,*LPS_DMGTRX_LOG;

    class CTSE_PerasoDMGTRXTest : public CTSE_TestBase
    {
    public:
        CTSE_PerasoDMGTRXTest(void);
        ~CTSE_PerasoDMGTRXTest(void);
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
#if 0
        bool StopTxRxTest(CCJGW_DonglesComPort& donglesComPort);
#endif
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
        //! 
        bool CheckRxResultsToTestStandards(const std::string& strRxResult,int nRssiStandard,int nFerStandard,S_DMG_TRX_RESULT& sDMGTRxResult);
        //! S_DMG_PERASO_INFO
        void InitDMGPerasoInfo(S_DMG_PERASO_INFO& sDMGPerasoInfo);
        //! S_DMG_TRX_RESULT
        void InitDMGTRxResults(LPS_DMG_TRX_RESULT psDMGTRxLogResults);
        //! 
        bool InitTestParamEnvironment();
        //!
        bool StartDMGTRxTest(CCJGW_DonglesComPort& txDonglesComPort,S_DMG_PERASO_INFO& sTxPerasoInfo,CCJGW_DonglesComPort& rxDonglesComPort,S_DMG_PERASO_INFO& sRxPerasoInfo,std::vector<int>& vRssiRange,std::vector<int>& vFerRange,LPS_DMG_TRX_RESULT psDMGTRxLogResults);
        //! 金机校准配置
        bool StartDMGTRxAutomaticCalibrationConfigurationTest(CCJGW_DonglesComPort& txDonglesComPort,S_DMG_PERASO_INFO& sTxPerasoInfo,CCJGW_DonglesComPort& rxDonglesComPort,S_DMG_PERASO_INFO& sRxPerasoInfo,std::vector<int>& vRssiRange,std::vector<int>& vFerRange,LPS_DMG_TRX_RESULT psDMGTRxLogResults);
        //! 获取金机测试结果指标
        bool GetRxResultsToTestStandards(const std::string& strRxResult,int nRssiStandard,int nFerStandard,int& nRssi,int& FerPerimile);
        //!
        void SendViewTestLog(const std::wstring& strLog);
        //! 
        void SendViewTestLogFormat(const wchar_t* strCustomLog,...);
    private:
        //! 金机测试自动配置方案
        bool mbAutomaticCalibrationConfigurationTest;
        bool mbInitializeOnce;
        bool mbTestResult;
        int mnRecvTimeMs;
        int mnPPDUDetecs;
        int mnItemTestErrorRetryCount;
        //! RSSI 测试标准在金机上浮 +-3
        int mnRssiTestRange;
        //std::wstring mstrGuSerialNumber;
        //std::wstring mstrDutSerialNumber;
        //std::wstring mstrGuMacAddress;
        //std::wstring mstrDutMacAddress;
        //std::wstring mstrGuSector;
        //std::wstring mstrDutSector;
        std::wstring mstrGoldenXmlConfigPath;
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
        std::wstring mstrFerRxGuTxDutEnv;
        std::wstring mstrRssiTxGuRxDutEnv;
        std::wstring mstrFerTxGuRxDutEnv;

        std::wstring mstrDMGCsvLogFilePath;

        std::vector<int> mvChannels;
        std::vector<int> mvRssiRxGuTxDut;
        std::vector<int> mvFerRxGuTxDut;
        std::vector<int> mvRssiTxGuRxDut;
        std::vector<int> mvFerTxGuRxDut;

        S_DMGTRX_LOG msDmgTRxLog;
    };
}


