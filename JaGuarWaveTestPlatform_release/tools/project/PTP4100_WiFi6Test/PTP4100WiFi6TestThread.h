#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include "PTP4100WiFi6TestDefine.h"
#include <JGW_SSHPlugin/CJGW_SSHShell.h>
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>
#include "CJGWQMSLWlanDeivce.h"
#include <fstream>
#include "PTP4100_QMSLConfigXml.h"
//#define TX_SINGLE_DEBUG
//#define RX_SINGLE_DEBUG
namespace JGW
{
    class CPTP4100WiFi6TestThread : public CCMessageThread
    {
    public:
        CPTP4100WiFi6TestThread(void);
        ~CPTP4100WiFi6TestThread(void);
    private:
        void OnStartWiFi6TestThread(WPARAM wParam,LPARAM lParam);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        void SendTestLogToWindow(const std::wstring& strTestLog);
        // ping ip
        //  strIPAddress -t  Ping 指定的主机，直到停止
        // strSourceAddress 要使用的源地址
        bool CheckSocketConnect(const std::wstring& strIPAddress,const std::wstring& strSourceAddress,int nTimeOutSec = 60);
        // ssh connect
        bool CheckSSHConnect(CCJGW_SSHShell& sshShell,const std::wstring& strIPAddress, const std::wstring& strSourceAddress);
        // wifi down
        // rmmod monitor
        // rmmod ecm_wifi_plugin
        // rmmod wifi_3_0
        // rmmod qca_ol
        // insmod qca_ol testmode = 1
        // insmod wifi_3_0
        // diag_socket_app - a 192.168.1.111 &
        // /etc/init.d/ftm start
        // ftm - n - c / tmp / ftm.conf &
        bool SwitchWiFiFtm(CCJGW_SSHShell& sshShell,const std::string& strAddres);
        // open serial port
        //bool CheckSerialPortConnect(const std::wstring& strSerialPort,CCJGW_CSerialComPort& serialComPort);
        bool ConnectQMSLPhone(CCJGWQMSLWlanDeivce& qmslWlandDevice,const std::wstring& strIPAddress);
        // load wifi tx rx dut
        // ConnectDutUsingQUTS WlanSetInstance WlanSetDutTxMacAddressBssId
        bool LoadDut6G(CCJGWQMSLWlanDeivce& qmslWlandDevice);
        // check ptp4100
        bool CheckPTP4100UsbConnect(CCJGW_SSHShell& sshShell);
        // Tx Atten VGA1 (0 ~ 0x1F) / Tx Atten VGA2(0~0x1F)
        bool SettingTxSerialVGA1AndVGA2Atten(unsigned char uTxAttenVga1, unsigned char uTxAttenVga2);
        // Rx Atten 0~ 0x1F
        bool SettingRxSerialAtten(unsigned char uRxAtten);
        // Start Rx
        bool StartQMSLRx();
        // start qmsl tx
        bool StartQMSLTx(int nTxPower0);
    private:
        void BuildWiFiTestLog();
        int GetTestChannel();
    private:
        int mnChannel;
        PS_TEST_PARAM mpsTestParam;
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
        CCJGW_ICMPSocket mIcmpSocket;
        CCJGW_SSHShell mcTxSshShell;
        CCJGW_SSHShell mcRxSshShell;
        CCJGWQMSLWlanDeivce mcTxQmslWlandDevice;
        CCJGWQMSLWlanDeivce mcRxQmslWlandDevice;
        std::wofstream mfIperfLog;
        CPTP4100_QMSLConfigXml mcQMSLConfigXml;
    };


}
