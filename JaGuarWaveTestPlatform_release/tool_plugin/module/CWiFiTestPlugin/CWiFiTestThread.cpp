#include "StdAfx.h"
#include "CWiFiTestThread.h"
#include "JGWWiFiTestConfig.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
namespace JGW
{
    CCWiFiTestThread::CCWiFiTestThread(void):m_pCMW500EquipmentControll(NULL),m_bInitEquipment(false),mhWlanBatProcess(NULL)
    {
    }


    CCWiFiTestThread::~CCWiFiTestThread(void)
    {
        if (m_pCMW500EquipmentControll)
        {
            delete m_pCMW500EquipmentControll;
            m_pCMW500EquipmentControll = NULL;
        }
        CloseWlanBatProcess();
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCWiFiTestThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCWiFiTestThread::OnInitThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCWiFiTestThread::OnRunThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCWiFiTestThread::OnInitThread(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpWiFiTestConfig = (PS_WLAN_CONFIG)lParam;

        if (std::wstring::npos != mpWiFiTestConfig->msMonitorInfo.m_strModelName.find(L"CMW"))
            m_pCMW500EquipmentControll = new JGW_CMW500EquipmentControll;
    }

    std::wstring CCWiFiTestThread::GetSerialNumber()
    {
        return mstrSN;
    }

    void CCWiFiTestThread::OnRunThread(WPARAM wParam,LPARAM lParam)
    {
#if 1
        mstrStartTestTime = JGW_W2A(JGW_GetCurrentTimeStamp());
        mstrSN = L"";
        if (InitEquipment() 
            && ConnectDevice() 
            && GetSerialNumberTest()
            && CheckMESSNStatus()
            && InitWlanModuleView() 
            && ConnectDevice() 
            && LoadWiFiDut() 
            && RunWiFiTest()
            && SerialNumberNextStationMES()
            && MESSaveTestRecords(true))
        {
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
            goto TEST_END;
        }
#else
        if (InitEquipment() && ConnectDevice()  )
        {
            m_pWiFiTestDlg->PutAsyncMessage(WM_TEST_PASS_MSG);
            goto TEST_END;
        }
#endif
		if (!mstrSN.empty()) MESSaveTestRecords(false);
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
TEST_END:
        mWiFiQmsl.UnLoadWiFiDut();
        DisConnectDevice();
        CloseWlanBatProcess();
    }

    void CCWiFiTestThread::SendListMsgToWnd(const wchar_t* strMsg,bool bInsert /* = true */)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg,LPARAM(bInsert?1:0));
    }

    void CCWiFiTestThread::CloseWlanBatProcess()
    {
        JGW_TerminateProcess(mhWlanBatProcess);
    }

    void CCWiFiTestThread::SendMsgToDisplay(const wchar_t* strMsg,bool bInsertItem /* = true */)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg,LPARAM(bInsertItem?1:0));
    }

    bool CCWiFiTestThread::ConnectDevice()
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
                    if (mWiFiQmsl.ConnectDevice(nComPort))
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

    bool CCWiFiTestThread::GetSerialNumberTest()
    {
        SendMsgToDisplay(_T("Get SN@---@---@---@W"));
        Sleep(3 * 1000);
        for (int i = 0;i < 5;i ++)
        {
            if (mWiFiQmsl.ReadSN(mstrSN,14))
            {
                std::wstring strTemp;
                JGW_FormatWString(strTemp,_T("Get SN@---@%s@---@P"),mstrSN.c_str());
                SendMsgToDisplay(strTemp.c_str(),false);
                return true;
            }
        }
        SendMsgToDisplay(_T("Get SN@---@Fail@---@F"),false);
        return false;
    }

    bool CCWiFiTestThread::SerialNumberNextStationMES()
    {
        if (mstrSN.empty()) return false;
        SendMsgToDisplay(_T("SN Next Station@---@---@---@W"));
        if (!GetMESDataBaseAdo()->NextStationToSN(JGW_W2A(mstrSN)))
        {
            PELOG4WW(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str());
            SendMsgToDisplay(_T("SN Next Station@---@Fail@---@F"),false);
            return false;
        }
        std::wstring strTemp;
        JGW_FormatWString(strTemp,_T("SN Next Station@---@%s@---@P"),mstrSN.c_str());
        SendMsgToDisplay(strTemp.c_str(),false);
        return true;
    }

    bool CCWiFiTestThread::CheckMESSNStatus()
    {
        SendMsgToDisplay(_T("Check MES SN Status@---@---@---@W"));
        if (!GetMESDataBaseAdo()->CheckSNStatus(JGW_W2A(mstrSN)))
        {
            PELOG4WW(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str());
            SendMsgToDisplay(_T("Check MES SN Status@---@Fail@---@F"),false);
            return false;
        }
        std::wstring strTemp;
        JGW_FormatWString(strTemp,_T("Check MES SN Status@---@%s@---@P"),mstrSN.c_str());
        SendMsgToDisplay(strTemp.c_str(),false);
        return true;
    }

    bool CCWiFiTestThread::MESSaveTestRecords(bool bTestStatus)
    {
        mstrEndTestTime = JGW_W2A(JGW_GetCurrentTimeStamp());
        SendMsgToDisplay(_T("MES SaveTestRecords@---@---@---@W"));
        if (!GetMESDataBaseAdo()->SaveTestRecordsMES(JGW_W2A(mstrSN),mstrStartTestTime,mstrEndTestTime,bTestStatus?"PASS":"FAIL","","ADD"))
        {
            PELOG4WW(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str());
            SendMsgToDisplay(_T("MES SaveTestRecords@---@Fail@---@F"),false);
            return false;
        }
        std::wstring strTemp;
        JGW_FormatWString(strTemp,_T("MES SaveTestRecords@---@%s@---@P"),mstrSN.c_str());
        SendMsgToDisplay(strTemp.c_str(),false);
        return true;
    }

    bool CCWiFiTestThread::DisConnectDevice()
    {
        SendMsgToDisplay(_T("DisConnect Device@---@---@---@P"));
        mWiFiQmsl.DisConnectDevice();
        return true;
    }

    bool CCWiFiTestThread::InitWlanModuleView()
    {
        SendMsgToDisplay(_T("Init Wlan Module@---@正在初始化....@---@W"));
        for (int i = 0;i < 3;i ++)
        {
            mhWlanBatProcess = JGW_CreateProcess(mpWiFiTestConfig->mstrWlanBatFilePath.c_str(),mpWiFiTestConfig->mstrWlanBatDir.c_str(),STARTF_USESHOWWINDOW,SW_HIDE);
            //! 等待Bat文件运行成功
            Sleep(3000);
            if (JGW_CheckProcessIsRun(mhWlanBatProcess))
            {
                SendMsgToDisplay(_T("Init Wlan Module@---@PASS@---@P"),false);
                return true;
            }
        }
        SendMsgToDisplay(_T("Init Wlan Module@---@FAIL@---@F"),false);

        return false;
    }

    bool CCWiFiTestThread::LoadWiFiDut()
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
        if (!JGW_CheckProcessIsRun(mhWlanBatProcess))
            SendMsgToDisplay(_T("Init Wlan Module@---@FAIL@---@F"),false);
        else
            SendMsgToDisplay(_T("Load WiFi Dut@---@FAIL@---@F"),false);

        return false;
    }

    bool CCWiFiTestThread::InitEquipment()
    {
        SendMsgToDisplay(_T("初始化仪器@---@正在初始化仪器..@---@W"));
        if (!m_bInitEquipment)
        {
            switch (mpWiFiTestConfig->msMonitorInfo.m_cInterfaceType)
            {
            case E_GPIB_HIVE:
                m_pCMW500EquipmentControll->SetGPIB(
                    mpWiFiTestConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_cNIGPIBType,
                    mpWiFiTestConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nGpibInterface,
                    mpWiFiTestConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nPrimaryAddress,
                    mpWiFiTestConfig->msMonitorInfo.msNIGPIBInstrumentInfo.m_nSecAddress);
                m_bInitEquipment = true;
                break;
            case E_VISA_HIVE:
                m_pCMW500EquipmentControll->SetVisa(
                    mpWiFiTestConfig->msMonitorInfo.msVISAInstrumentInfo.mstrVisaAddress.c_str());
                m_bInitEquipment = true;
                break;
            case E_TCP_IP_HIVE:
                m_pCMW500EquipmentControll->SetTCPIP(
                    mpWiFiTestConfig->msMonitorInfo.msTCPIPInstrumentInfo.mstrIPAddress.c_str(),
                    mpWiFiTestConfig->msMonitorInfo.msTCPIPInstrumentInfo.mnSubInstrument);
                m_bInitEquipment = true;
                break;
            default:
                m_bInitEquipment = false;
            }     
        }
        for (int i = 0;i < 3;i ++)
        {
            if (m_bInitEquipment && m_pCMW500EquipmentControll->ResetMode() 
                && m_pCMW500EquipmentControll->WLAN_InitMode())
            {
                m_pCMW500EquipmentControll->SetInstrumentSys(E_WLAN_NAME);
                SendMsgToDisplay(_T("初始化仪器@---@OK@---@P"),false);
                return true;
            }
            SendMsgToDisplay(_T("初始化仪器@---@FAIL@---@F"),false);
            Sleep(200);
        }
        
        return false;
    }

    bool CCWiFiTestThread::RunWiFiTest()
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
                for (std::vector<int>::iterator itChannel = it->mvFrequency.begin();
                    itChannel != it->mvFrequency.end();
                    ++ itChannel,nIndex ++)
                {
                    mnFreq = itChannel[0];
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

    bool CCWiFiTestThread::WiFiChannelTest()
    {
        std::wstring strTemp;
        bool bResult = true;
        //! 2412_B_0
        JGW_FormatWString(strTemp,L"%s TxChain: %d@--@%d@--@W",JGWWiFiTestConfig::GetWlanDateRateNameToDataRate(meWlanTxDataRate),mnTxChain,mnFreq);
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

    bool CCWiFiTestThread::InitWiFiTxTestEquipment()
    {
        bool bResult = true;

        m_pCMW500EquipmentControll->SetRxLoss(mpsWlanTestItemKPI->mfRxLoss);
        m_pCMW500EquipmentControll->SetRxInPutPort(mnRfPort);

        float fExpectPower = mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfExpectPower;
//         if (fExpectPower <= mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfExpectPower)
//         {
//             fExpectPower = (E_B_WLAN_MODE == meWlanMode)?mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPower:mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPower + 8;
//         }
        m_pCMW500EquipmentControll->WLAN_SetExpectedPowerAndUserMarg(fExpectPower,mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPowerRange + 2);
        m_pCMW500EquipmentControll->WLAN_SetWlanMode(meWlanHtMode);
        m_pCMW500EquipmentControll->WLAN_SetFrequency(mnFreq);

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
    bool CCWiFiTestThread::WiFiTxTestChannel()
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
		mbCheckTSM = false;
        //! 延时等待切换稳定
		for (int i = 0;i < 10 && !mbCheckTSM;i ++)
		{
			mbCheckTSM = true;
			if (CheckWiFiTxTestData()) return true;
			Sleep(1000);
		}
        return false;
    }

    bool CCWiFiTestThread::InitWiFiTxTestDevice()
    {

        if (!mWiFiQmsl.StartWiFiTx(mnTxChain,mpsWlanTestItemKPI->msWlanTxTestItemKpi.mfPower,mnFreq,meWlanHtMode,meWlanTxDataRate))
        {
            SendMsgToDisplay(_T("Start WiFi Tx Device@---@FAIL@---@F"));
            return false;
        }
        return true;
    }

    bool CCWiFiTestThread::DisableWiFiTxTestDevice()
    {
        if (!mWiFiQmsl.DisableWiFiTx())
        {
            SendMsgToDisplay(_T("Disbale WiFi Tx Device@---@FAIL@---@F"));
            return false;
        }
        return true;
    }

    bool CCWiFiTestThread::WiFiRxTestChannel()
    {
        m_pCMW500EquipmentControll->SetRxLoss(mpsWlanTestItemKPI->mfRxLoss);
        m_pCMW500EquipmentControll->SetTxOutPutPort(mnRfPort);
        if (!m_pCMW500EquipmentControll->WLAN_InitRxTest(mnFreq,mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfRssi,meWlanRxDataRate))
        {
            SendMsgToDisplay(_T("Init WiFi Rx Equipment@---@FAIL@---@F"));
            return false;
        }

        if (!mWiFiQmsl.StartWiFiRx(mnTxChain,mnFreq,meWlanHtMode,meWlanRxDataRate))
        {
            SendMsgToDisplay(_T("Init WiFi Rx Device@---@FAIL@---@F"));
            return false;
        }
        Sleep(500);
        float rssi = 0;
        int txbTotalPackets = 0,txbGoodPackets = 0;
        if (!mWiFiQmsl.GetWiFiRxData(rssi,txbTotalPackets,txbGoodPackets))
        {
            SendMsgToDisplay(_T("Get WiFi Rx Data@---@FAIL@---@F"));
            return false;
        }

        m_pCMW500EquipmentControll->WLAN_CloseRxTest();

        if (CheckWiFiRxRssi(rssi,mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfRssi - mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfRssiRange,mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfRssi + mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfRssiRange) /*&& CheckWiFiPER(txbGoodPackets,txbTotalPackets,mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfPER)*/)
        {
            return true;
        }

        return false;
    }

    bool CCWiFiTestThread::CheckWiFiTxTestData()
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

    bool CCWiFiTestThread::CheckTxPower(float fMinPower,float fMaxPower,float fPower)
    {
        std::wstring strTemp;
        bool bResult = (fPower >= fMinPower && fPower <= fMaxPower);

        JGW_FormatWString(strTemp,L"Tx Power@%.2f dBm@%.2f dBm@%.2f dBm@%s",fMinPower,fPower,fMaxPower,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }

    bool CCWiFiTestThread::CheckEVM_B(const wchar_t* testProject,float fEVM,float fMinEVM /* = 0 */,float fMaxEVM /* = 35 */)
    {
        std::wstring strTemp;
        bool bResult = (fEVM >= fMinEVM && fEVM <= fMaxEVM);

        JGW_FormatWString(strTemp,L"%s@%.2f%%@%.2f%%@%.2f%%@%s",testProject,fMinEVM,fEVM,fMaxEVM,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }

    bool CCWiFiTestThread::CheckFreq_Error_B(float fFreqError,float fMinFreqError /* = -60000 */,float fMaxFreqError /* = 60000 */)
    {
        std::wstring strTemp;
        bool bResult = (fFreqError >= fMinFreqError && fFreqError <= fMaxFreqError);

        JGW_FormatWString(strTemp,L"Freq Error@%.2f Hz@%.2f Hz@%.2f Hz@%s",fMinFreqError,fFreqError,fMaxFreqError,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }

    bool CCWiFiTestThread::CheckTransmitSpectrumMask(const wchar_t* testProject,float fValue,float fMinValue /* = -40 */,float fMaxValue /* = 0 */)
    {
        std::wstring strTemp;
        bool bResult = (fValue >= fMinValue && fValue <= fMaxValue);

        JGW_FormatWString(strTemp,L"%s@%.2f dB@%.2f dB@%.2f dB@%s",testProject,fMinValue,fValue,fMaxValue,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());
		mbCheckTSM = bResult;

        return bResult;
    }

    bool CCWiFiTestThread::CheckTransmitSpectrumMaskResult(float fValue)
    {
        std::wstring strTemp;
        bool bResult = (fValue >= 0 && fValue <= 100);

        JGW_FormatWString(strTemp,L"TSM(Transmit Spectrum Mask)@0 %%@%.2f%%@100 %%@%s",fValue,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());
		mbCheckTSM = bResult;
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
    bool CCWiFiTestThread::CheckEVM(const wchar_t* testProject,float fValue,float fMinValue,float fMaxValue)
    {
        std::wstring strTemp;
        bool bResult = (fValue >= fMinValue && fValue <= fMaxValue);

        JGW_FormatWString(strTemp,L"%s@%.2f dB@%.2f dB@%.2f dB@%s",testProject,fMinValue,fValue,fMaxValue,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }

    bool CCWiFiTestThread::CheckWiFiRxRssi(float value,float minValue,float maxValue)
    {
        std::wstring strTemp;
        bool bResult = (value >= minValue && value <= maxValue);

        JGW_FormatWString(strTemp,L"WiFi Rssi(%.2f)@%.2f@%.2f@%.2f@%s",mpsWlanTestItemKPI->msWlanRxTestItemKpi.mfRssi,minValue,value,maxValue,bResult?L"P":L"F");
        SendMsgToDisplay(strTemp.c_str());

        return bResult;
    }

    bool CCWiFiTestThread::CheckWiFiPER(int txbGoodPackets,int txbTotalPackets,float fPercentValue)
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

