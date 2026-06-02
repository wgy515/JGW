#include "StdAfx.h"
#include "CJGWWiFiTestThread.h"
#include "CJGWWiFiTestDlg.h"
#include "JGWWiFiTestConfig.h"
#include <JGW_QMSLFuncPlugin/WiFiTestDefine.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
/*
adb root
adb shell
rmmod wlan
insmod /system/lib/modules/wlan.ko con_mode=5
ftmdaemon -n
*/

namespace JGW
{
    CCJGWWiFiTestThread::CCJGWWiFiTestThread(void):m_pWiFiTestDlg(NULL),m_pCMW500EquipmentControll(NULL),m_bInitEquipment(false),mhWlanBatInstance(NULL)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGWWiFiTestThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCJGWWiFiTestThread::OnInitThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCJGWWiFiTestThread::OnRunThread)
    END_MYTHREAD_MESSAGE_MAP()

    CCJGWWiFiTestThread::~CCJGWWiFiTestThread(void)
    {
        if (m_pCMW500EquipmentControll)
        {
            delete m_pCMW500EquipmentControll;
            m_pCMW500EquipmentControll = NULL;
        }
        CloseWlanBatProcess();
    }

    void CCJGWWiFiTestThread::OnInitThread(WPARAM wParam,LPARAM lParam)
    {
        m_pWiFiTestDlg = (CCJGWWiFiTestDlg*)wParam;
        mpWiFiTestConfig = (PS_WLAN_CONFIG)lParam;

        if (std::wstring::npos != mpWiFiTestConfig->msMonitorInfo.m_strModelName.find(L"CMW"))
            m_pCMW500EquipmentControll = new JGW_CMW500EquipmentControll;
    }

    void CCJGWWiFiTestThread::OnRunThread(WPARAM wParam,LPARAM lParam)
    {
#if 1
        if (InitEquipment() 
            && ConnectDevice() 
            && InitWlanModuleView() 
            && ConnectDevice() 
            && LoadWiFiDut() 
            && RunWiFiTest())
        {
            m_pWiFiTestDlg->PutAsyncMessage(WM_TEST_PASS_MSG);
            goto TEST_END;
        }
#else
        if (InitEquipment() && ConnectDevice()  )
        {
            m_pWiFiTestDlg->PutAsyncMessage(WM_TEST_PASS_MSG);
            goto TEST_END;
        }
#endif
        m_pWiFiTestDlg->PutAsyncMessage(WM_TEST_FAIL_MSG);
TEST_END:
        mWiFiQmsl.UnLoadWiFiDut();
        DisConnectDevice();
        CloseWlanBatProcess();
    }

    void CCJGWWiFiTestThread::CloseWlanBatProcess()
    {
        if (NULL != mhWlanBatInstance && (DWORD)mhWlanBatInstance >= 32)
        {
            TerminateProcess(mhWlanBatInstance,-1);
            mhWlanBatInstance = NULL;
        }
    }

    void CCJGWWiFiTestThread::SendMsgToDisplay(const wchar_t* strMsg,bool bInsertItem /* = true */)
    {
        m_pWiFiTestDlg->PutAsyncStringMessage(WM_HWND_TEST_LIST_INFO_OK_MSG,strMsg,bInsertItem?1:0);
    }

    bool CCJGWWiFiTestThread::ConnectDevice()
    {
        SendMsgToDisplay(_T("Connect Device@---@---@---@W"));
        std::wstring strComPort,strValue;
        std::vector<std::wstring> vEnumPidVid;
        size_t nCount = 0;
        int nComPort = 0;
        //! DeviceDesc
        for (int i = 0;i < 50;i ++)
        {
            vEnumPidVid.clear();
            nCount = JGW_GetUsbEnumPidVidVectotToServiceName(L"qcusbser",vEnumPidVid);
            if (nCount <= 0) { Sleep(1000);continue;}
            for (size_t nIndex = 0;nIndex < nCount;nIndex ++)
            {
                strValue = JGW_GetRegUsbInfoToTag(vEnumPidVid.at(nIndex).c_str(),L"DeviceDesc");
                if (NULL != JGW_WStrCaseWStr(strValue.c_str(),L"Diag"))
                {
                    JGW_GetComportToUsbPidVid(vEnumPidVid.at(nIndex).c_str(),nComPort);
                    if (nComPort <= 0) { Sleep(1000);continue;}
                    if (mWiFiQmsl.ConnectDevice(16))
                    {
                        SendMsgToDisplay(_T("Connect Device@---@---@---@P"),false);
                        return true;
                    }
                }
            }

        }
        
        SendMsgToDisplay(_T("Connect Device@---@FAIL@---@F"),false);

        return false;
    }

    bool CCJGWWiFiTestThread::DisConnectDevice()
    {
        SendMsgToDisplay(_T("DisConnect Device@---@---@---@P"));
        mWiFiQmsl.DisConnectDevice();
        return true;
    }

    bool CCJGWWiFiTestThread::InitWlanModuleView()
    {
        SendMsgToDisplay(_T("Init Wlan Module@---@正在初始化....@---@W"));
        for (int i = 0;i < 3;i ++)
        {
            mhWlanBatInstance = ShellExecute(NULL,L"open",
                mpWiFiTestConfig->mstrWlanBatFilePath.c_str(),
                NULL,
                mpWiFiTestConfig->mstrWlanBatDir.c_str(),
                SW_HIDE);
            //! 等待Bat文件运行成功
            Sleep(3000);
            if ((DWORD)mhWlanBatInstance >= 32)
            {
                SendMsgToDisplay(_T("Init Wlan Module@---@PASS@---@P"),false);
                return true;
            }
        }
        SendMsgToDisplay(_T("Init Wlan Module@---@FAIL@---@F"),false);

        return false;
    }

    bool CCJGWWiFiTestThread::LoadWiFiDut()
    {
        SendMsgToDisplay(_T("Load WiFi Dut@---@---@---@W"));
        for (int i = 0;i < 20;i ++,Sleep(1000))
        {
            if (mWiFiQmsl.LoadWiFiDut(mpWiFiTestConfig->mstrBoardDataFilePath.c_str()))
            {
                SendMsgToDisplay(_T("Load WiFi Dut@---@---@---@P"),false);
                return true;
            }
        }
        SendMsgToDisplay(_T("Load WiFi Dut@---@FAIL@---@F"),false);

        return false;
    }

    bool CCJGWWiFiTestThread::InitEquipment()
    {
        SendMsgToDisplay(_T("初始化仪器@---@正在初始化仪器..@---@W"));
        if (!m_bInitEquipment)
        {
            m_pCMW500EquipmentControll->SetGPIB(
                mpWiFiTestConfig->msMonitorInfo.m_cNIGPIBType,
                mpWiFiTestConfig->msMonitorInfo.m_nGpibInterface,
                mpWiFiTestConfig->msMonitorInfo.m_nPrimaryAddress,
                mpWiFiTestConfig->msMonitorInfo.m_nSecAddress);
            m_bInitEquipment = true;
        }

        if (m_pCMW500EquipmentControll->ResetMode() 
            && m_pCMW500EquipmentControll->WLAN_InitMode())
        {
            SendMsgToDisplay(_T("初始化仪器@---@OK@---@P"),false);
            return true;
        }
        SendMsgToDisplay(_T("初始化仪器@---@FAIL@---@F"),false);

        return false;
    }

    bool CCJGWWiFiTestThread::RunWiFiTest()
    {
        SendMsgToDisplay(_T("Start WiFi Test@---@---@---@W"));
        int nIndex = 0;

        for (std::vector<S_WLAN_MODE_CONFIG>::iterator it = mpWiFiTestConfig->mvsWlanModeConfig.begin();
            it != mpWiFiTestConfig->mvsWlanModeConfig.end();
            ++ it)
        {
            nIndex = 0;    
            mnTxChain = it->mnTxChain;
            mnRfPort = it->mnRfPort;
            for (size_t nDataRateIndex = 0;nDataRateIndex < it->mvWlanRxDataRate.size();nDataRateIndex ++)
            {
                meWlanTxDataRate = it->mvWlanTxDataRate.at(nDataRateIndex);
                meWlanHtMode = JGWWiFiTestConfig::GetWlanHTModeToTxDataRate(meWlanTxDataRate);
                meWlanMode = JGWWiFiTestConfig::GetWlanModeToTxDataRate(meWlanTxDataRate);
                meWlanRxDataRate = it->mvWlanRxDataRate.at(nDataRateIndex);               
                for (std::vector<float>::iterator itChannel = it->mvChannel.begin();
                    itChannel != it->mvChannel.end();
                    ++ itChannel,nIndex ++)
                {
                    mfChannel = itChannel[0];
                    mpsWlanTestItemKPI = &(it[0].mvWlanTestItemKPI.at(nIndex));
                    if (!WiFiChannelTest())
                    {
                        return false;
                    }
                }
            }
        }
        SendMsgToDisplay(_T("Stop WiFi Test@---@---@---@W"));
        return true;
    }

    bool CCJGWWiFiTestThread::WiFiChannelTest()
    {
        std::wstring strTemp;
        bool bResult = true;
        //! 2412_B_0
        JGW_FormatWString(strTemp,L"%s TxChain: %d@--@%.0f@--@W",JGWWiFiTestConfig::GetWlanDateRateNameToDataRate(meWlanTxDataRate),mnTxChain,mfChannel);
        SendMsgToDisplay(strTemp.c_str());

        for (int i = 0;i < mpWiFiTestConfig->mnErrorReTest; i ++)
        {
            bResult = WiFiTxTestChannel();
            if (bResult) break;
            Sleep(1000);
        }
        DisableWiFiTxTestDevice();

        if (!bResult) return false;

        for (int i = 0;i < mpWiFiTestConfig->mnErrorReTest; i ++)
        {
            bResult = WiFiRxTestChannel();
            if (bResult) break;;
            Sleep(1000);
        }

        return bResult;
    }

    bool CCJGWWiFiTestThread::InitWiFiTxTestEquipment()
    {
        bool bResult = true;

        m_pCMW500EquipmentControll->SetRxLoss(mpsWlanTestItemKPI->mfRxLoss);
        m_pCMW500EquipmentControll->SetRxInPutPort(mnRfPort);

        float fExpectPower = mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfExpectPower;
        if (fExpectPower <= mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfExpectPower)
        {
            fExpectPower = (E_B_WLAN_MODE == meWlanMode)?mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPower:mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPower + 8;
        }
        m_pCMW500EquipmentControll->WLAN_SetExpectedPowerAndUserMarg(fExpectPower,mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPowerRange + 2);
        m_pCMW500EquipmentControll->WLAN_SetWlanMode(meWlanHtMode);
        m_pCMW500EquipmentControll->WLAN_SetFrequency(mfChannel);

        return bResult;
    }
    /*
    [b]
    TxChain=
    RfPort=
    BandWidth=
    Channel=
    RxLoss
    */
    bool CCJGWWiFiTestThread::WiFiTxTestChannel()
    {
        //! 初始化仪器
        if (!InitWiFiTxTestEquipment())
        {
            return false;
        }
        //! 初始化设备
        if (!InitWiFiTxTestDevice())
        {
            return false;
        }

        if (!CheckWiFiTxTestData())
        {
            return false;
        }

        return true;
    }

    bool CCJGWWiFiTestThread::InitWiFiTxTestDevice()
    {

        if (!mWiFiQmsl.StartWiFiTx(mnTxChain,mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPower,mfChannel,meWlanHtMode,meWlanTxDataRate))
        {
            SendMsgToDisplay(_T("Start WiFi Tx Device@---@FAIL@---@F"));
            return false;
        }
        return true;
    }

    bool CCJGWWiFiTestThread::DisableWiFiTxTestDevice()
    {
        if (!mWiFiQmsl.DisableWiFiTx())
        {
            SendMsgToDisplay(_T("Disbale WiFi Tx Device@---@FAIL@---@F"));
            return false;
        }
        return true;
    }

    bool CCJGWWiFiTestThread::WiFiRxTestChannel()
    {
        m_pCMW500EquipmentControll->SetRxLoss(mpsWlanTestItemKPI->mfRxLoss);
        m_pCMW500EquipmentControll->SetTxOutPutPort(mnRfPort);
        if (!m_pCMW500EquipmentControll->WLAN_InitRxTest(mfChannel,mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfRssi,meWlanRxDataRate))
        {
            SendMsgToDisplay(_T("Init WiFi Rx Equipment@---@FAIL@---@F"));
            return false;
        }

        if (!mWiFiQmsl.StartWiFiRx(mnTxChain,mfChannel,meWlanHtMode,meWlanRxDataRate))
        {
            SendMsgToDisplay(_T("Init WiFi Rx Device@---@FAIL@---@F"));
            return false;
        }
        Sleep(1000);
        float rssi = 0;int txbTotalPackets = 0,txbGoodPackets = 0;
        if (!mWiFiQmsl.GetWiFiRxData(rssi,txbTotalPackets,txbGoodPackets))
        {
            SendMsgToDisplay(_T("Get WiFi Rx Data@---@FAIL@---@F"));
            return false;
        }

        m_pCMW500EquipmentControll->WLAN_CloseRxTest();

        if (CheckWiFiRxRssi((int)rssi,mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfRssi - mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfRssiRange,mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfRssi + mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfRssiRange) && CheckWiFiPER(txbGoodPackets,txbTotalPackets,mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfPER))
        {
            return true;
        }

        return false;
    }

    bool CCJGWWiFiTestThread::CheckWiFiTxTestData()
    {
        float fModuleData[20] = {0};
        float fSpeMask[20] = {0};
        //! 获取测试数据
        if (!m_pCMW500EquipmentControll->WLAN_GetTestResult(meWlanMode,fModuleData,fSpeMask))
        {
            SendMsgToDisplay(_T("Get WiFi Tx Test Data@---@FAIL@---@F"));
            return false;
        }

        if (E_B_WLAN_MODE == meWlanMode)
        {
            if (CheckTxPower(mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPower - mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPowerRange,mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPower + mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPowerRange,fModuleData[3])
                && CheckEVM_B(L"EVM Peak",fModuleData[4])
                && CheckEVM_B(L"EVM RMS",fModuleData[5])
                && CheckFreq_Error_B(fModuleData[6])
                && CheckTransmitSpectrumMask(L"TSM_AB(Transmit Spectrum Mask)",fSpeMask[0],-999)
                && CheckTransmitSpectrumMask(L"TSM_CD(Transmit Spectrum Mask)",fSpeMask[1],-999)
                && CheckTransmitSpectrumMask(L"TSM_DC(Transmit Spectrum Mask)",fSpeMask[2],-999)
                && CheckTransmitSpectrumMask(L"TSM_BA(Transmit Spectrum Mask)",fSpeMask[3],-999)
                && CheckTransmitSpectrumMaskResult(fSpeMask[4])
                )
            {
                return true;
            }
        }
        /*
        <2_Margin_AB>
        <3_Margin_BC>
        <4_Margin_CD>
        <5_Margin_DE>
        <6_Margin_ED>
        <7_Margin_DC>
        <8_Margin_CB>
        <9_Margin_BA>
        */
        else if (E_G_WLAN_MODE == meWlanMode || E_N_WLAN_MODE == meWlanMode)
        {
            if (CheckTxPower(mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPower - mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPowerRange,mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPower + mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPowerRange,fModuleData[2])
                && CheckEVM(L"EVM All Carriers(54 Mbps, 64-QAM)",fModuleData[3],-9999,-25)
                && CheckEVM(L"EVM Data Carriers(54 Mbps, 64-QAM)",fModuleData[4],-9999,-25)
                && CheckEVM(L"EVM Pilot Carriers(54 Mbps, 64-QAM)",fModuleData[5],-9999,-25)
                && CheckFreq_Error_B(fModuleData[6])
                && CheckTransmitSpectrumMask(L"TSM_AB(Transmit Spectrum Mask)",fSpeMask[0],-999)
                && CheckTransmitSpectrumMask(L"TSM_BC(Transmit Spectrum Mask)",fSpeMask[1],-999)
                && CheckTransmitSpectrumMask(L"TSM_CD(Transmit Spectrum Mask)",fSpeMask[2],-999)
                && CheckTransmitSpectrumMask(L"TSM_DE(Transmit Spectrum Mask)",fSpeMask[3],-999)
                && CheckTransmitSpectrumMask(L"TSM_ED(Transmit Spectrum Mask)",fSpeMask[4],-999)
                && CheckTransmitSpectrumMask(L"TSM_DC(Transmit Spectrum Mask)",fSpeMask[5],-999)
                && CheckTransmitSpectrumMask(L"TSM_CB(Transmit Spectrum Mask)",fSpeMask[6],-999)
                && CheckTransmitSpectrumMask(L"TSM_BA(Transmit Spectrum Mask)",fSpeMask[7],-999)
                && CheckTransmitSpectrumMaskResult(fSpeMask[8])
                )
            {
                return true;
            }
        }
        else if (E_AC_WLAN_MODE == meWlanMode)
        {
            if (CheckTxPower(mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPower - mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPowerRange,mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPower + mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPowerRange,fModuleData[3])
                && CheckEVM(L"EVM All Carriers",fModuleData[4],-9999,-25)
                && CheckEVM(L"EVM Data Carriers",fModuleData[5],-9999,-25)
                && CheckEVM(L"EVM Pilot Carriers",fModuleData[6],-9999,-25)
                && CheckFreq_Error_B(fModuleData[7])
                && CheckTransmitSpectrumMask(L"TSM_AB(Transmit Spectrum Mask)",fSpeMask[0],-999)
                && CheckTransmitSpectrumMask(L"TSM_BC(Transmit Spectrum Mask)",fSpeMask[1],-999)
                && CheckTransmitSpectrumMask(L"TSM_CD(Transmit Spectrum Mask)",fSpeMask[2],-999)
                && CheckTransmitSpectrumMask(L"TSM_DE(Transmit Spectrum Mask)",fSpeMask[3],-999)
                && CheckTransmitSpectrumMask(L"TSM_ED(Transmit Spectrum Mask)",fSpeMask[4],-999)
                && CheckTransmitSpectrumMask(L"TSM_DC(Transmit Spectrum Mask)",fSpeMask[5],-999)
                && CheckTransmitSpectrumMask(L"TSM_CB(Transmit Spectrum Mask)",fSpeMask[6],-999)
                && CheckTransmitSpectrumMask(L"TSM_BA(Transmit Spectrum Mask)",fSpeMask[7],-999)
                && CheckTransmitSpectrumMaskResult(fSpeMask[8])
                )
            {
                return true;
            }
        }

        return false;
    }

    bool CCJGWWiFiTestThread::CheckTxPower(float fMinPower,float fMaxPower,float fPower)
    {
        std::wstring strTemp;
        bool bResult = (fPower >= fMinPower && fPower <= fMaxPower);

        JGW_FormatWString(strTemp,L"Tx Power@%.2f dBm@%.2f dBm@%.2f dBm@%s",fMinPower,fPower,fMaxPower,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }

    bool CCJGWWiFiTestThread::CheckEVM_B(const wchar_t* testProject,float fEVM,float fMinEVM /* = 0 */,float fMaxEVM /* = 35 */)
    {
        std::wstring strTemp;
        bool bResult = (fEVM >= fMinEVM && fEVM <= fMaxEVM);

        JGW_FormatWString(strTemp,L"%s@%.2f%%@%.2f%%@%.2f%%@%s",testProject,fMinEVM,fEVM,fMaxEVM,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }

    bool CCJGWWiFiTestThread::CheckFreq_Error_B(float fFreqError,float fMinFreqError /* = -60000 */,float fMaxFreqError /* = 60000 */)
    {
        std::wstring strTemp;
        bool bResult = (fFreqError >= fMinFreqError && fFreqError <= fMaxFreqError);

        JGW_FormatWString(strTemp,L"Freq Error@%.2f Hz@%.2f Hz@%.2f Hz@%s",fMinFreqError,fFreqError,fMaxFreqError,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }

    bool CCJGWWiFiTestThread::CheckTransmitSpectrumMask(const wchar_t* testProject,float fValue,float fMinValue /* = -40 */,float fMaxValue /* = 0 */)
    {
        std::wstring strTemp;
        bool bResult = (fValue >= fMinValue && fValue <= fMaxValue);

        JGW_FormatWString(strTemp,L"%s@%.2f dB@%.2f dB@%.2f dB@%s",testProject,fMinValue,fValue,fMaxValue,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }

    bool CCJGWWiFiTestThread::CheckTransmitSpectrumMaskResult(float fValue)
    {
        std::wstring strTemp;
        bool bResult = (fValue >= 0 && fValue <= 100);

        JGW_FormatWString(strTemp,L"TSM(Transmit Spectrum Mask)@0 %%@%.2f%%@100 %%@%s",fValue,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }
    /*
    Characteristics Refer to IEEE 802.11-2007, section... Specified Limit
    EVM 15.4.7.9/18.4.7.8 Transmit Modulation Accuracy < 35%
    Frequency Error 15.4.7.5/18.4.7.4 Transmit Center Frequency Tolerance < ±25 ppm
    Chip Clock Error 15.4.7.6/18.4.7.5 Chip Clock Frequency Tolerance < ±25 ppm
    */
    /*
    Characteristics Refer to IEEE 802.11-2007, section... Specified Limit
    EVM (RMS) 802.11a/g: 17.3.9.6.3 Transmitter Constellation Error < -5 dB (6 Mbps)
    < -8 dB (9 Mbps)
    < -10 dB (12 Mbps)
    < -13 dB (18 Mbps)
    < -16 dB (24 Mbps)
    < -19 dB (36 Mbps)
    < -22 dB (48 Mbps)
    < -25 dB (54 Mbps)
    Frequency Error 802.11a: 17.3.9.4 Transmit Center Frequency Tolerance
    802.11g: 19.4.7.2 Transmit Center Frequency Tolerance
    < ±20 ppm
    < ±25 ppm
    Symbol Clock Error 802.11a: 17.3.9.5 Symbol Clock Frequency Tolerance
    802.11g: 19.4.7.3 Symbol Clock Frequency Tolerance
    < ±20 ppm
    < ±25 ppm

    EVM (RMS) 20.3.21.7.3 Transmitter Constellation Error < -5 dB (BPSK, 1/2)
    < -10 dB (QPSK, 1/2)
    < -13 dB (QPSK, 3/4)
    < -16 dB (16-QAM, 1/2)
    < -19 dB (16-QAM, 3/4)
    < -22 dB (64-QAM, 2/3)
    < -25 dB (64-QAM, 3/4)
    < -28 dB (64-QAM, 5/6)
    Frequency Error 20.3.21.4 Transmit Center Frequency Tolerance < ±25 ppm (2.4 GHz band)
    < ±20 ppm (5 GHz band)
    Symbol Clock Error 20.3.21.6 Symbol Clock Frequency Tolerance < ±25 ppm (2.4 GHz band)
    < ±20 ppm (5 GHz band)

    802.11a/g -16 to -1, 1 to 16 (center)
    -26 to -17, 17 to 26 (side)
    -2 dB
    -4 dB
    2 dB
    2 dB
    802.11n (20 MHz) -16 to -1, 1 to 16 (center)
    -28 to -17, 17 to 28 (side)
    -2 dB
    -4 dB
    2 dB
    2 dB
    802.11n (40 MHz, HT) -42 to -2, 2 to 42 (center)
    -58 to -43, 43 to 58 (side)
    -2 dB
    -4 dB
    2 dB
    2 dB
    */
    bool CCJGWWiFiTestThread::CheckEVM(const wchar_t* testProject,float fValue,float fMinValue,float fMaxValue)
    {
        std::wstring strTemp;
        bool bResult = (fValue >= fMinValue && fValue <= fMaxValue);

        JGW_FormatWString(strTemp,L"%s@%.2f dB@%.2f dB@%.2f dB@%s",testProject,fMinValue,fValue,fMaxValue,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }

    bool CCJGWWiFiTestThread::CheckWiFiRxRssi(int value,int minValue,int maxValue)
    {
        std::wstring strTemp;
        bool bResult = (value >= minValue && value <= maxValue);

        JGW_FormatWString(strTemp,L"WiFi Rssi@%d@%d@%d@%s",minValue,value,maxValue,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }

    bool CCJGWWiFiTestThread::CheckWiFiPER(int txbGoodPackets,int txbTotalPackets,float fPercentValue)
    {
        std::wstring strTemp;
        int txFailPackets = txbTotalPackets - txbGoodPackets;
        float fPerValue = (float)txFailPackets * 100.00f/ (float) txbTotalPackets;
        bool bResult = (fPerValue >= 0 && fPerValue <= fPercentValue);

        JGW_FormatWString(strTemp,L"WiFi PER(recv=%d,total=%d)@0%%@%.2f%%@%.2f%%@%s",txbGoodPackets,txbTotalPackets,fPerValue,fPercentValue,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }
}

