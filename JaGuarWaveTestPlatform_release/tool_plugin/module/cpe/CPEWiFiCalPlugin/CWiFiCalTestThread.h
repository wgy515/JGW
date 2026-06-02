#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <JGW_WindowsFuncPlugin/CJGW_CSerialComPort.h>
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>
#include "CPEWiFiCalDefine.h"
#include <JGW_QMSLFuncPlugin/CJGW_WCNTestIPQ8070Dut.h>
#include <JGW_Device_WCNTester_CommonSCPIWLAN/WCNTester_CommonSCPIWLAN.h>
namespace JGW
{
    class CCWiFiCalTestThread : public CCMessageThread
    {
    public:
        CCWiFiCalTestThread(void);
        ~CCWiFiCalTestThread(void);
    private:
        void OnInitThread(WPARAM wParam,LPARAM lParam);
        void OnRunThread(WPARAM wParam,LPARAM lParam);

        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        bool InitWiFiCalSerail();

        //!
        bool ConnectDut();
        //! 
        bool DutLog();
        //!
        bool WlanLoadDut();
        //!
        bool WlanSetInstance(WLAN_INSTANCE instance = WLAN0);
        //!
        bool WlanSetDutTxMacAddressBssId(const std::string& rxStation = "00.03.7F.44.55.68",const std::string& txStation = "00.03.7F.44.55.69",const std::string& bssId = "00.03.7F.44.55.70",unsigned int deviceIndex = 0);
        //!
        bool WlanSetLocalMac();
        //!
        bool Wlan5GCalibration();
        //!
        bool Wlan2GCalibration();
        //!
        bool WlanSaveBDF2();
    private: //! INIT WIFI CAL
        bool PingServiceIPAddr();
        //! 
        bool OpenSerialPort();
        //! 登录到设备后台中
        bool LoginDeviceBackground();
        //!
        bool InitWiFiCalModelToDevice();
        //! 
        void InitTestListMsg(const std::wstring& strProject = L"");
        //!
        void SendTestListMsg(bool bInsertItem = false);
    private: //! Calibration
        bool SetUpDutTxDetails2();
        //! 
        bool GetTxReport2();
        //!
        bool WlanXtalCalInit2();
        //!
        bool WlanXtalCalibration2();
        //！
        bool WlanTxCalibrationFWControlled2(WLAN_UniversalPhyID phyId = PHY_A0, WLAN_TpcCalScheme calScheme = ALL_POINTS);
        //! 
        bool WlanRxCalibrationFWControlled2(WLAN_UniversalPhyID phyId, uint chainToCalibrate, WLAN_SYS_BAND band = WLAN_SYS_5G);
        //! 
        bool WlanNoiseFloorCal2(WLAN_UniversalPhyID PhyId = PHY_A0, WLAN_CHAIN_MASK2 chainMask = WLAN_CHAIN_1234, WLAN_CHAIN_MASK2 chainMaskH = WLAN_CHAIN_NONE, unsigned short channel = 5745);
        //！
        bool WlanNoiseFloorCalGenBDF(const std::string& strChannel = "5180,5320,5500,5745",WLAN_UniversalPhyID PhyId= PHY_A0, WLAN_CHAIN_MASK2 chainMask = WLAN_CHAIN_1234, WLAN_CHAIN_MASK2 chainMaskH = WLAN_CHAIN_NONE);
    private:
        //!
        template <typename Func> 
        inline bool GeneralTestTemplate(const std::wstring& strProject,Func func)
        {
            bool result = false;
            InitTestListMsg(strProject);
            SendTestListMsg(true);

            result = func();

            msTestListMsg.mstrTestResult = result?L"P":L"F";
            SendTestListMsg();
            return result;
        }
    private:
        LPS_WIFI_CAL_CONFIG mpsWiFiCalConfig;
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;

        S_TEST_LIST_MSG msTestListMsg;

        CCJGW_CSerialComPort mSerialComPort;
        CCJGW_ICMPSocket mICMPSocket;
        CCJGW_WCNTestIPQ8070Dut mWCNTestIPQ8070Dut;

    };
}


