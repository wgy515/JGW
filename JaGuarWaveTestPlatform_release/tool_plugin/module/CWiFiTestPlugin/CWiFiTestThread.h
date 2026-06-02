#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <JGW_Device_RohdeSchwarz_CMW500/JGW_CMW500EquipmentControll.h>
#include <JGW_QMSLFuncPlugin/CJGWWiFiQMSL.h>

#include "CWiFiTest_Define.h"

namespace JGW
{
    class CCWiFiTestThread:public CCMessageThread
    {
    public:
        CCWiFiTestThread(void);
        ~CCWiFiTestThread(void);

        std::wstring GetSerialNumber();
    private:
        void OnInitThread(WPARAM wParam,LPARAM lParam);
        void OnRunThread(WPARAM wParam,LPARAM lParam);
       
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        void SendListMsgToWnd(const wchar_t* strMsg,bool bInsert = true);
        bool InitWlanModuleView();
        bool InitEquipment();
        bool RunWiFiTest();
        bool GetSerialNumberTest();
        //! MES ¼ì²éSNµ±Ç°×´Ì¬
        bool CheckMESSNStatus();
        //!
        bool SerialNumberNextStationMES();
        //!
        bool MESSaveTestRecords(bool bTestStatus);
    private:
        bool ConnectDevice();
        bool DisConnectDevice();
        void CloseWlanBatProcess();
        bool LoadWiFiDut();

        bool WiFiChannelTest();

        bool WiFiTxTestChain();
        bool WiFiTxTestBandWidth(int nBandWidth );
        bool WiFiTxTestChannel( );
        bool WiFiRxTestChannel( );


        bool InitWiFiTxTestEquipment( );
        bool InitWiFiTxTestDevice( );
        bool CheckWiFiTxTestData( );
        bool DisableWiFiTxTestDevice();

        void SendMsgToDisplay(const wchar_t* strMsg,bool bInsertItem = true);

    private:
        bool CheckTxPower(float fMinPower,float fMaxPower,float fPower);
        bool CheckEVM_B(const wchar_t* testProject,float fEVMRMS,float fMinEVMRMS = 0,float fMaxEVMRMS = 35);
        bool CheckEVM(const wchar_t* testProject,float fValue,float fMinValue,float fMaxValue);

        bool CheckFreq_Error_B(float fFreqError,float fMinFreqError = -60000,float fMaxFreqError = 60000);
        bool CheckTransmitSpectrumMask(const wchar_t* testProject,float fValue,float fMinValue = -40,float fMaxValue = 0);
        bool CheckTransmitSpectrumMaskResult(float fValue);

        bool CheckWiFiRxRssi(float value,float minValue,float maxValue);

        bool CheckWiFiPER(int txbGoodPackets,int txbTotalPackets,float fPercentValue);
    private:
		bool			    mbCheckTSM;
        bool                m_bInitEquipment;
        E_WLAN_MODE         meWlanMode;
        E_WLAN_HT_MODE      meWlanHtMode;
        E_WLAN_TX_DATA_RATE meWlanTxDataRate;
        E_WLAN_RX_DATA_RATE meWlanRxDataRate;
        int                 mnTxChain;
        int                 mnRfPort;
        S_WLAN_TEST_ITEM_KPI* mpsWlanTestItemKPI;
        int                 mnFreq;
        HANDLE              mhWlanBatProcess;
    private:
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
        PS_WLAN_CONFIG      mpWiFiTestConfig;
        JGW_CMW500EquipmentControll* m_pCMW500EquipmentControll;
        CCJGWWiFiQMSL       mWiFiQmsl;
        std::wstring        mstrSN;
        std::string         mstrStartTestTime;
        std::string         mstrEndTestTime;
    };
}


