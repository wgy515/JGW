#include "StdAfx.h"
#include "CWiFiCalTestThread.h"
#include "../../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>

namespace JGW
{
    CCWiFiCalTestThread::CCWiFiCalTestThread(void) : mpAsyncWndMessage(NULL),mpsWiFiCalConfig(NULL),mSerialComPort("")
    {
    }


    CCWiFiCalTestThread::~CCWiFiCalTestThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCWiFiCalTestThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCWiFiCalTestThread::OnInitThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCWiFiCalTestThread::OnRunThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCWiFiCalTestThread::OnInitThread(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsWiFiCalConfig = (LPS_WIFI_CAL_CONFIG)lParam;

        //WCNTester_CommonSCPIWLAN wcnTester;
        //wcnTester.getCWFreqErr(5500.0,30);
        /*WCNTester_CommonSCPIWLAN wcnTester;
        std::vector<S_STATION_CAL_PATH> vsStationCalPath;


        vsStationCalPath.push_back(mpsWiFiCalConfig->mvsStationCalPath.at(0));
        vsStationCalPath.push_back(mpsWiFiCalConfig->mvsStationCalPath.at(0));
        vsStationCalPath.push_back(mpsWiFiCalConfig->mvsStationCalPath.at(0));
        vsStationCalPath.push_back(mpsWiFiCalConfig->mvsStationCalPath.at(0));




        wcnTester.SetTRxLoss(true,vsStationCalPath);
        wcnTester.SetTRxLoss(false,vsStationCalPath);*/
        /*mpWiFiTestConfig = (PS_WLAN_CONFIG)lParam;

        if (std::wstring::npos != mpWiFiTestConfig->msMonitorInfo.m_strModelName.find(L"CMW"))
        m_pCMW500EquipmentControll = new JGW_CMW500EquipmentControll;*/
    }

    void CCWiFiCalTestThread::OnRunThread(WPARAM wParam,LPARAM lParam)
    {
        //! 初始化连接过程
        if (/*!InitWiFiCalSerail() || */!ConnectDut() || !DutLog()
            || !WlanLoadDut() 
            || !WlanSetLocalMac())
        {
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            goto TEST_END;
        }

        if (!Wlan5GCalibration() || !Wlan2GCalibration() || !WlanSaveBDF2())
        {
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            goto TEST_END;
        }

        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
TEST_END:
        mWCNTestIPQ8070Dut.DutLog(false);
        mSerialComPort.CloseSerialComPort();
        mWCNTestIPQ8070Dut.DisconnectWlanTester();
    }

    bool CCWiFiCalTestThread::InitWiFiCalSerail()
    {
        //! ping CPE是否已经与电脑连接
        if (!PingServiceIPAddr())
        {
            return false;
        }
        //! 连接调试串口
        if (!OpenSerialPort())
        {
            return false;
        }

        //! 读取串口消息查询关键字 发送\r\n 查询OpenWrt login字符
        //! 登录设备后台程序中
        if (!LoginDeviceBackground())
        {
            return false;
        }
        //! 发送切换到WIFI校准模式串口命令
        if (!InitWiFiCalModelToDevice())
        {
            return false;
        }

        return true;
    }

    bool CCWiFiCalTestThread::ConnectDut()
    {
        return GeneralTestTemplate(L"Connect Dut",[&]()->bool{
            return mWCNTestIPQ8070Dut.ConnectDut(QLIB_TARGET_TYPE_APQ,8070,"COM30001",true);
        });
    }

    bool CCWiFiCalTestThread::DutLog()
    {
        return GeneralTestTemplate(L"Dut Log",[&]()->bool{
            mWCNTestIPQ8070Dut.DutLog(true);
            return true;
        });
    }

    bool CCWiFiCalTestThread::WlanLoadDut()
    {
        return GeneralTestTemplate(L"Wlan Load Dut",[&]()->bool{
            return mWCNTestIPQ8070Dut.WlanLoadDut(mpsWiFiCalConfig->mstreepFName);
        });
    }
    //! WIFI0_MAC  5G  WIFI1_MAC 2.4G
    bool CCWiFiCalTestThread::WlanSetInstance(WLAN_INSTANCE instance /* = WLAN0 */)
    {
        return GeneralTestTemplate(L"Wlan Set Instance",[&]()->bool{
            return mWCNTestIPQ8070Dut.WlanSetInstance(instance);
        });
    }

    bool CCWiFiCalTestThread::WlanSetDutTxMacAddressBssId(const std::string& rxStation /* = "00.03.7F.44.55.68" */,const std::string& txStation /* = "00.03.7F.44.55.69" */,const std::string& bssId /* = "00.03.7F.44.55.70" */,unsigned int deviceIndex /* = 0 */)
    {
        return GeneralTestTemplate(L"Wlan Set Dut Tx Mac Address BssId",[&]()->bool{
            return mWCNTestIPQ8070Dut.WlanSetDutTxMacAddressBssId(rxStation,txStation,bssId,deviceIndex);
        });
    }

    bool CCWiFiCalTestThread::WlanSetLocalMac()
    {
        return GeneralTestTemplate(L"Wlan Set Local Mac",[&]()->bool{
            return mWCNTestIPQ8070Dut.WlanSetLocalMac(mpsWiFiCalConfig->mstrWlan5GMac,mpsWiFiCalConfig->mstrWlan2_4GMac);
        });
    }


    bool CCWiFiCalTestThread::PingServiceIPAddr()
    {
        return GeneralTestTemplate(L"ping cpe device",[&]() -> bool{
            for (int i = 0;i < 60;i ++)
            {
                if (0 == i % 3)
                {
                    msTestListMsg.mstrValue = L".";
                }
                else
                {
                    msTestListMsg.mstrValue += L".";
                }
                SendTestListMsg();

                if (mICMPSocket.Ping(mpsWiFiCalConfig->mstrServiceIPAddr.c_str(),mpsWiFiCalConfig->mstrCurrentHostIPAddr.c_str()))
                {
                    return true;
                }
            }
            return false;
        });
    }

    bool CCWiFiCalTestThread::OpenSerialPort()
    {
        return GeneralTestTemplate(L"open debug serial port",[&]()->bool{
            for (int i = 0;i < 20;i ++)
            {
                mpsWiFiCalConfig->mstrSerailComPort = JGW_GetComPortToUsbServiceName(L"Ser2pl",L"VID_067B");
                if (!mpsWiFiCalConfig->mstrSerailComPort.empty() && mSerialComPort.OpenSerialComPort(mpsWiFiCalConfig->mstrSerailComPort.c_str(),mpsWiFiCalConfig->mnnBaudRate))
                {
                    return true;
                }
                Sleep(1000);
                SendTestListMsg();
            }
            return false;
        });
    }

    bool CCWiFiCalTestThread::LoginDeviceBackground()
    {
        return GeneralTestTemplate(L"Login Device",[&]()->bool{
            std::string strRead,strTemp;

            if (!mSerialComPort.WriteCommand("\r\n"))
            {
                return false;
            }

            for (int i = 0;i < 10 * 5;i ++)
            {
                mSerialComPort.ReadResponseCommandToComPort(strTemp);
                strRead += strTemp;
                //! 已经登录设备
                if (NULL != JGW_StrCaseStr(strRead.c_str(),"root@OpenWrt:"))
                {
                    return true;
                }
                //! 开始登录设备
                if (NULL != JGW_StrCaseStr(strRead.c_str(),"OpenWrt login:"))
                {
                    //mSerialComPort.WriteCommand(mpsWiFiCalConfig->mstrUserName);
                    mSerialComPort.SetEndString("Password:");
                    strTemp = mpsWiFiCalConfig->mstrUserName + "\r\n";
                    std::string read;
                    /*if (*/mSerialComPort.ReadRespCommandToWriteCommand(strTemp.c_str(),read);
                    {
                        if (NULL != JGW_StrCaseStr(read.c_str(),"Password:"))
                        {
                            strTemp = mpsWiFiCalConfig->mstrPassword + "\r\n";
                            mSerialComPort.WriteCommand(strTemp);
                        }
                    }
                    mSerialComPort.SetEndString("");
                }
                Sleep(200);
            }
            return true;
        });
    }

    bool CCWiFiCalTestThread::InitWiFiCalModelToDevice()
    {
        return GeneralTestTemplate(L"Init WiFi Cal Model",[&]()->bool{
            std::vector<string> vCommand;
            std::string read,temp;
            JGW_ParserStrA(mpsWiFiCalConfig->mstrWiFiCalSerailCommand.c_str(),"\r\n",vCommand);
            for (size_t i = 0;i < vCommand.size();i ++ )
            {
                temp = vCommand[i] + "\n";
                mSerialComPort.WriteCommand(temp.c_str());
                if (string::npos != temp.find("sleep"))
                {
                    JGW_ReplaceStringA(temp,"sleep ","");
                    //Sleep(atoi(temp.c_str()) * 1000);
                }
                //Sleep(1000);
            }


            for (int i = 0;i < 20 * 5;i ++)
            {
                /*if (*/mSerialComPort.ReadResponseCommandToComPort(temp);/*)*/
                {
                    read += temp;
                    PELOG4WA(temp.c_str());
                    //! 初始化成功标志位 FTMDaemon: Diag_LSM_Init succesful
                    if (NULL != JGW_StrCaseStr(read.c_str(),"FTMDaemon: Diag_LSM_Init succesful"))
                    {
                        return true;
                    }
                }
                Sleep(200);
            }
            return false;
        });
    }

    void CCWiFiCalTestThread::InitTestListMsg(const std::wstring& strProject /* = L"" */)
    {
        msTestListMsg.mstrTestProject = strProject;
        msTestListMsg.mstrMinValue = L"--";
        msTestListMsg.mstrMaxValue = L"--";
        msTestListMsg.mstrValue = L"--";
        msTestListMsg.mstrTestResult = L"--";
    }

    void CCWiFiCalTestThread::SendTestListMsg(bool bInsertItem /* = false */)
    {
        std::wstring strMsg;
        JGW_FormatWString(strMsg,L"%s@%s@%s@%s@%s",msTestListMsg.mstrTestProject.c_str(),msTestListMsg.mstrMinValue.c_str(),msTestListMsg.mstrValue.c_str(),msTestListMsg.mstrMaxValue.c_str(),msTestListMsg.mstrTestResult.c_str());
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg.c_str(),LPARAM(bInsertItem?1:0));
    }

    bool CCWiFiCalTestThread::Wlan5GCalibration()
    {
        //! InitializeDUT-5G
        if (!WlanSetInstance(mpsWiFiCalConfig->ms5GCalParam.meInstance)
            || !WlanSetDutTxMacAddressBssId("00.03.7F.44.55.71","00.03.7F.44.33.80","00.03.7F.C0.FF.EE",0))
        {
            return false;
        }

        //! Crystal_Cal
        if (!SetUpDutTxDetails2() 
            || !GetTxReport2() 
            || !WlanXtalCalInit2() 
            || !WlanXtalCalibration2())
        {
            return false;
        }
        return false;
        //! TPC-5G
        if (!WlanTxCalibrationFWControlled2(mpsWiFiCalConfig->ms5GCalParam.meUniversalPhyID,mpsWiFiCalConfig->ms5GCalParam.meTpcCalScheme))
        {
            return false;
        }
        //! RxGainCal-5G-8chains
        //! chainToCalibrate
        for (size_t index = 0;index < mpsWiFiCalConfig->ms5GCalParam.mvChainToCalibrate.size();++ index)
        {
            if (!WlanRxCalibrationFWControlled2(mpsWiFiCalConfig->ms5GCalParam.meUniversalPhyID,mpsWiFiCalConfig->ms5GCalParam.mvChainToCalibrate[index],WLAN_SYS_5G))
            {
                return false;
            }
        }
        //! NFCal-5G
        /*if (!WlanNoiseFloorCal2(mpsWiFiCalConfig->ms5GCalParam.meUniversalPhyID,WLAN_CHAIN_1234,WLAN_CHAIN_NONE,5745))
        {
        return false;
        }*/

        if (!WlanNoiseFloorCalGenBDF(mpsWiFiCalConfig->ms5GCalParam.mstrWlanNoiseFloorCalChannel,mpsWiFiCalConfig->ms5GCalParam.meUniversalPhyID,WLAN_CHAIN_1234,WLAN_CHAIN_NONE))
        {
            return false;
        }

        return true;
    }

    bool CCWiFiCalTestThread::Wlan2GCalibration()
    {
        //! InitializeDUT-2G
        if (!WlanSetInstance(mpsWiFiCalConfig->ms2GCalParam.meInstance)
            || !WlanSetDutTxMacAddressBssId("00.03.7F.44.55.71","00.03.7F.44.33.80","00.03.7F.C0.FF.EE",0))
        {
            return false;
        }

        //! TPC-2G
        if (!WlanTxCalibrationFWControlled2(mpsWiFiCalConfig->ms2GCalParam.meUniversalPhyID,mpsWiFiCalConfig->ms2GCalParam.meTpcCalScheme))
        {
            return false;
        }
        //! RxGainCal-2G WlanRxCalibrationFWControlled2
        for (size_t index = 0;index < mpsWiFiCalConfig->ms5GCalParam.mvChainToCalibrate.size();++ index)
        {
            if (!WlanRxCalibrationFWControlled2(mpsWiFiCalConfig->ms2GCalParam.meUniversalPhyID,mpsWiFiCalConfig->ms2GCalParam.mvChainToCalibrate[index],WLAN_SYS_2G))
            {
                return false;
            }
        }

        //! NFCal-2G
        /*if (!WlanNoiseFloorCal2(mpsWiFiCalConfig->ms2GCalParam.meUniversalPhyID,WLAN_CHAIN_1234,WLAN_CHAIN_NONE,5745))
        {
            return false;
        }*/

        if (!WlanNoiseFloorCalGenBDF(mpsWiFiCalConfig->ms2GCalParam.mstrWlanNoiseFloorCalChannel,mpsWiFiCalConfig->ms2GCalParam.meUniversalPhyID,WLAN_CHAIN_1234,WLAN_CHAIN_NONE))
        {
            return false;
        }

        return true;
    }

    //! Calibration
    bool CCWiFiCalTestThread::SetUpDutTxDetails2()
    {
        double TxPower = 0.00;
        return GeneralTestTemplate(L"SetUp Dut Tx Details2",[&]()->bool{
            bool result = mWCNTestIPQ8070Dut.SetUpDutTxDetails2(&TxPower,PHY_A0,5500.00,0,primary20,RATE_MCS_0,RateBW_11AC_VHT20,_1Stream,TxPowerForce_CLPC,ContTx99,WLAN_CHAIN_1,WLAN_CHAIN_NONE,0,0,0,5,1500,false,false,1,DPD_Disabled,HCS_Disabled,1,10,ZEROES_PATTERN);
            JGW_FormatWString(msTestListMsg.mstrValue,L"%.2f",TxPower);
            return result;
        });
    }

    bool CCWiFiCalTestThread::GetTxReport2()
    {
        uint TxRGdPkt = 0,TxRput = 0,TxRTemp0 = 0,TxRTemp1 = 0,TxRGainIdx = 0,TxRDacGain = 0,TxRPAcfg = 0,TxRpdadc = 0;
        double rssi = 0;

        if (!GeneralTestTemplate(L"Get Tx Report",[&]()->bool{
            return mWCNTestIPQ8070Dut.GetTxReport2(PHY_A0,WLAN_CHAIN_1,WLAN_CHAIN_NONE,TxRGdPkt,TxRput,TxRTemp0,TxRTemp1,rssi,TxRGainIdx,TxRDacGain,TxRPAcfg,TxRpdadc);
        }))
        {
            return false;
        }
        //! TxRGdPkt TxR good Packets
        if (!GeneralTestTemplate(L"good Packets",[&]()->bool{
            JGW_FormatWString(msTestListMsg.mstrValue,L"%d",TxRGdPkt);
            msTestListMsg.mstrMinValue = L"0";
            msTestListMsg.mstrMaxValue = L"99999";
            return TxRGdPkt >= 0 && TxRGdPkt <= 99999;
        }))
        {
            return false;
        }

        if (!GeneralTestTemplate(L"Throughput Kb",[&]()->bool{
            JGW_FormatWString(msTestListMsg.mstrValue,L"%d",TxRput);
            msTestListMsg.mstrMinValue = L"0";
            msTestListMsg.mstrMaxValue = L"99999";
            return TxRput >= 0 && TxRput <= 99999;
        }))
        {
            return false;
        }

        if (!GeneralTestTemplate(L"ThermCal Chain 0",[&]()->bool{
            JGW_FormatWString(msTestListMsg.mstrValue,L"%d",TxRTemp0);
            msTestListMsg.mstrMinValue = L"0";
            msTestListMsg.mstrMaxValue = L"255";
            return TxRTemp0 >= 0 && TxRTemp0 <= 255;
        }))
        {
            return false;
        }

        if (!GeneralTestTemplate(L"ThermCal Chain 1",[&]()->bool{
            JGW_FormatWString(msTestListMsg.mstrValue,L"%d",TxRTemp1);
            msTestListMsg.mstrMinValue = L"0";
            msTestListMsg.mstrMaxValue = L"255";
            return TxRTemp1 >= 0 && TxRTemp1 <= 255;
        }))
        {
            return false;
        }

        if (!GeneralTestTemplate(L"gainIdx",[&]()->bool{
            JGW_FormatWString(msTestListMsg.mstrValue,L"%d",TxRGainIdx);
            msTestListMsg.mstrMinValue = L"0";
            msTestListMsg.mstrMaxValue = L"100";
            return TxRGainIdx >= 0 && TxRGainIdx <= 100;
        }))
        {
            return false;
        }

        if (!GeneralTestTemplate(L"dacGain",[&]()->bool{
            JGW_FormatWString(msTestListMsg.mstrValue,L"%d",TxRDacGain);
            msTestListMsg.mstrMinValue = L"0";
            msTestListMsg.mstrMaxValue = L"100";
            return TxRDacGain >= 0 && TxRDacGain <= 100;
        }))
        {
            return false;
        }

        if (!GeneralTestTemplate(L"paCfg",[&]()->bool{
            JGW_FormatWString(msTestListMsg.mstrValue,L"%d",TxRPAcfg);
            msTestListMsg.mstrMinValue = L"0";
            msTestListMsg.mstrMaxValue = L"100";
            return TxRPAcfg >= 0 && TxRPAcfg <= 100;
        }))
        {
            return false;
        }
        // TxRpdadc
        if (!GeneralTestTemplate(L"pdadc",[&]()->bool{
            JGW_FormatWString(msTestListMsg.mstrValue,L"%d",TxRpdadc);
            msTestListMsg.mstrMinValue = L"0";
            msTestListMsg.mstrMaxValue = L"100";
            return TxRpdadc >= 0 && TxRpdadc <= 100;
        }))
        {
            return false;
        }

        if (!GeneralTestTemplate(L"rssi",[&]()->bool{
            JGW_FormatWString(msTestListMsg.mstrValue,L"%.2f",rssi);
            msTestListMsg.mstrMinValue = L"-128";
            msTestListMsg.mstrMaxValue = L"128";
            return rssi >= -128 && rssi <= 128;
        }))
        {
            return false;
        }

        return true;
    }

    bool CCWiFiCalTestThread::WlanXtalCalInit2()
    {
        return GeneralTestTemplate(L"WlanXtalCalInit2",[&]()->bool{
            return mWCNTestIPQ8070Dut.WlanXtalCalInit2(PHY_A0,0,1,XTAL_OTP_DISABLE,1000,2000);
        });
    }

    bool CCWiFiCalTestThread::WlanXtalCalibration2()
    {
        uint xtalCapIn = 0, xtalCapOut = 0;
        double freqErrPPM = 0.00;

        return GeneralTestTemplate(L"WlanXtalCalibration2",[&]()->bool{
            return mWCNTestIPQ8070Dut.WlanXtalCalibration2(xtalCapIn,xtalCapOut,freqErrPPM,PHY_A0,5500,WLAN_SINGLE_CHAIN_1,28,0,5,3,false);
        });
    }

    bool CCWiFiCalTestThread::WlanTxCalibrationFWControlled2(WLAN_UniversalPhyID phyId /* = PHY_A0 */, WLAN_TpcCalScheme calScheme /* = ALL_POINTS */)
    {
        return GeneralTestTemplate(L"WlanTxCalibrationFWControlled2",[&]()->bool{
            return mWCNTestIPQ8070Dut.WlanTxCalibrationFWControlled2(phyId,calScheme,4,1,1,false,3.0,-3.0);
        });
    }

    bool CCWiFiCalTestThread::WlanRxCalibrationFWControlled2(WLAN_UniversalPhyID phyId, uint chainToCalibrate, WLAN_SYS_BAND band /* = WLAN_SYS_5G */)
    {
        return GeneralTestTemplate(L"WlanRxCalibrationFWControlled2",[&]()->bool{
            return mWCNTestIPQ8070Dut.WlanRxCalibrationFWControlled2(phyId,chainToCalibrate,band,PromiscuousMode,"010000C0FFEE","020000C0FFEE");
        });
    }

    bool CCWiFiCalTestThread::WlanNoiseFloorCal2(WLAN_UniversalPhyID PhyId /* = PHY_A0 */, WLAN_CHAIN_MASK2 chainMask /* = WLAN_CHAIN_1234 */, WLAN_CHAIN_MASK2 chainMaskH /* = WLAN_CHAIN_NONE */, unsigned short channel /* = 5745 */)
    {
        double NFCAL_Values[8] = {0};
        return GeneralTestTemplate(L"WlanNoiseFloorCal2",[&]()->bool{
            return mWCNTestIPQ8070Dut.WlanNoiseFloorCal2(NFCAL_Values,PhyId,chainMask,chainMaskH,channel,Regular);
        });
    }

    bool CCWiFiCalTestThread::WlanNoiseFloorCalGenBDF(const std::string& strChannel /* = "5180 ,5320,5500,5745"*/,WLAN_UniversalPhyID PhyId/* = PHY_A0 */, WLAN_CHAIN_MASK2 chainMask /* = WLAN_CHAIN_1234 */, WLAN_CHAIN_MASK2 chainMaskH /* = WLAN_CHAIN_NONE */)
    {
        return GeneralTestTemplate(L"WlanNoiseFloorCalGenBDF",[&]()->bool{
            return mWCNTestIPQ8070Dut.WlanNoiseFloorCalGenBDF(strChannel,PhyId,chainMask,chainMaskH,false);
        });
    }

    bool CCWiFiCalTestThread::WlanSaveBDF2()
    {
        return GeneralTestTemplate(L"WlanSaveBDF2",[&]()->bool{
            return mWCNTestIPQ8070Dut.WlanSaveBDF2(BDF_DataFlash);
        });
    }

}