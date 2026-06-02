#include "StdAfx.h"
#include "PTP4100WiFi6TestThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <regex>
#include <poco/Stopwatch.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_FilePath.h>

//#define ERROR_RETRY_TEST 3
namespace JGW
{
    CPTP4100WiFi6TestThread::CPTP4100WiFi6TestThread(void) : mpsTestParam(NULL),mnChannel(6135)
    {
        mcQMSLConfigXml.LoadQMSLConfigXml();
        mnChannel = GetTestChannel();
    }


    CPTP4100WiFi6TestThread::~CPTP4100WiFi6TestThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CPTP4100WiFi6TestThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_TEST_START_MSG,&CPTP4100WiFi6TestThread::OnStartWiFi6TestThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CPTP4100WiFi6TestThread::OnStartWiFi6TestThread(WPARAM wParam,LPARAM lParam)
    {
        mpsTestParam = (PS_TEST_PARAM)wParam;
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)lParam;

        SendTestLogToWindow(L"Start WiFi Test ...");
        std::wstring strTemp;
        Poco::Stopwatch sw;

        if (NULL == mpsTestParam || NULL == mpAsyncWndMessage)
        {
            MessageBox(FindWindow(NULL,NULL), L"Invalid thread param", L"Error", MB_ICONERROR);
            return ;
        }

        sw.start();
#ifndef RX_SINGLE_DEBUG
        if (!CheckSocketConnect(mpsTestParam->mstrTxIPQAddress, mpsTestParam->mstrTxUsbNetCardAddress))
        {
            SendTestLogToWindow(L"Connect Tx Socket error.");
            goto TEST_END;
        }
#endif
#ifndef TX_SINGLE_DEBUG
        if (!CheckSocketConnect(mpsTestParam->mstrRxIPQAddress, mpsTestParam->mstrRxUsbNetCardAddress))
        {
            SendTestLogToWindow(L"Connect Rx Socket error.");
            goto TEST_END;
        }
#endif
#ifndef RX_SINGLE_DEBUG
        if (!CheckSSHConnect(mcTxSshShell,mpsTestParam->mstrTxIPQAddress,mpsTestParam->mstrTxUsbNetCardAddress))
        {
            SendTestLogToWindow(L"Connect Tx SSH error.");
            goto TEST_END;
        }
#endif
#ifndef TX_SINGLE_DEBUG
        if (!CheckSSHConnect(mcRxSshShell,mpsTestParam->mstrRxIPQAddress,mpsTestParam->mstrRxUsbNetCardAddress))
        {
            SendTestLogToWindow(L"Connect Rx SSH error.");
            goto TEST_END;
        }
#endif
#ifndef RX_SINGLE_DEBUG
        // 1 ¡¢ switch WiFi FTM
        if (!SwitchWiFiFtm(mcTxSshShell,JGW_W2A(mpsTestParam->mstrTxUsbNetCardAddress)))
        {
            SendTestLogToWindow(L"Switch Tx WiFi FTM error.");
            goto TEST_END;
        }
#endif
#ifndef TX_SINGLE_DEBUG
        if (!SwitchWiFiFtm(mcRxSshShell,JGW_W2A(mpsTestParam->mstrRxUsbNetCardAddress)))
        {
            SendTestLogToWindow(L"Switch Rx WiFi FTM error.");
            goto TEST_END;
        }
#endif
#ifndef RX_SINGLE_DEBUG
        if (!CheckPTP4100UsbConnect(mcTxSshShell))
        {
            SendTestLogToWindow(L"TX PTP4100 usb Not plugged into the IPQ device.");
            goto TEST_END;
        }
#endif
#ifndef TX_SINGLE_DEBUG
        if (!CheckPTP4100UsbConnect(mcRxSshShell))
        {
            SendTestLogToWindow(L"RX PTP4100 usb Not plugged into the IPQ device.");
            goto TEST_END;
        }
#endif
#ifndef RX_SINGLE_DEBUG
        if (!ConnectQMSLPhone(mcTxQmslWlandDevice,mpsTestParam->mstrTxIPQAddress))
        {
            SendTestLogToWindow(L"Connect Rx qmsl phone error.");
            goto TEST_END;
        }
#endif
#ifndef TX_SINGLE_DEBUG
        if (!ConnectQMSLPhone(mcRxQmslWlandDevice,mpsTestParam->mstrRxIPQAddress))
        {
            SendTestLogToWindow(L"Connect Tx qmsl phone error.");
            goto TEST_END;
        }
#endif
#ifndef RX_SINGLE_DEBUG
        if (!LoadDut6G(mcTxQmslWlandDevice))
        {
            SendTestLogToWindow(L"load wifi tx dut error.");
            goto TEST_END;
        }
#endif
#ifndef TX_SINGLE_DEBUG
        if (!LoadDut6G(mcRxQmslWlandDevice))
        {
            SendTestLogToWindow(L"load wifi rx dut error.");
            goto TEST_END;
        }
#endif

        size_t uTotalPackets = 0,uGoodPackets = 0;
        int nRssi = 0;
        float fEvm = 0.00f;

        BuildWiFiTestLog();
        JGW_FormatWString(strTemp, L"TxPower\tTxAttenVga1\tTxAttenVga2\tRxAtten\tTotalPackets\tGoodPackets\tRSSI\tEVM");
        SendTestLogToWindow(strTemp);
        JGW_FormatWString(strTemp, L"TxPower,TxAttenVga1,TxAttenVga2,RxAtten,TotalPackets,GoodPackets,RSSI,EVM");
        mfIperfLog << strTemp << std::endl;

        // 0 ~ 18
        for (int nTxPower = mpsTestParam->mnTxPowerStart; nTxPower <= mpsTestParam->mnTxPowerEnd; nTxPower+=mpsTestParam->mnTxPowerStep)
        {
#ifndef RX_SINGLE_DEBUG
            if (!StartQMSLTx(nTxPower))
            {
                SendTestLogToWindow(L"SetUpDutTxDetails2 error.");
                goto TEST_END;
            }       
#endif
            // 0 ~ 0x1F
            for (unsigned char uTxAttenVga1 = mpsTestParam->muTxAttenVga1Start; uTxAttenVga1 <= mpsTestParam->muTxAttenVga1End; uTxAttenVga1 += mpsTestParam->muTxAttenVga1Step)
            {
                // 0 ~ 0x1F
                for (unsigned char uTxAttenVga2 = mpsTestParam->muTxAttenVga2Start; uTxAttenVga2 <= mpsTestParam->muTxAttenVga2End; uTxAttenVga2 += mpsTestParam->muTxAttenVga2Step)
                {
#ifndef RX_SINGLE_DEBUG
                    if (!SettingTxSerialVGA1AndVGA2Atten(uTxAttenVga1, uTxAttenVga2))
                    {
                        SendTestLogToWindow(L"set tx serial vga1 vga2 attenuation error.");
                        goto TEST_END;
                    }
#endif  
                    int nRxAttenError = mpsTestParam->mnErrorRetryTest;
                    // 0 ~ 0x1F
                    for (unsigned char nRxAtten = mpsTestParam->muRxAttenStart; nRxAtten <= mpsTestParam->muRxAttenEnd; nRxAtten += mpsTestParam->muRxAttenStep)
                    {
                        for (int nErrorRetryTest = 0;nErrorRetryTest < mpsTestParam->mnErrorRetryTest;nErrorRetryTest++)
                        {

#ifndef TX_SINGLE_DEBUG
                            if (!SettingRxSerialAtten(nRxAtten))
                            {
                                SendTestLogToWindow(L"set rx serial attenuation error.");
                                goto TEST_END;
                            }
#endif
#ifndef TX_SINGLE_DEBUG
                            if (!StartQMSLRx())
                            {
                                SendTestLogToWindow(L"set continue rx error.");
                                goto TEST_END;
                            }                                    
#endif
                            Sleep(mpsTestParam->mnPacketCollectionTime);
#ifndef TX_SINGLE_DEBUG
                            if (!mcRxQmslWlandDevice.GetRxReport(uTotalPackets,uGoodPackets,nRssi,fEvm))
                            {
                                SendTestLogToWindow(L"GetRxReport error.");
                                goto TEST_END;
                            }
#endif

                            if (-128 == nRssi || 0 == fEvm || 0 == uTotalPackets || 0 == uGoodPackets)
                            {
                                if (nRssi > -128 && 0 == fEvm) break;
                                continue;
                            }
                            else
                            {
                                break;
                            }
                        }
                        JGW_FormatWString(strTemp, L"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%.2f", nTxPower,uTxAttenVga1,uTxAttenVga2, nRxAtten, uTotalPackets, uGoodPackets, nRssi, fEvm);
                        SendTestLogToWindow(strTemp);
#if 0
                        if (nRssi < mpsTestParam->mnMinRssi /*&& uTotalPackets <= mpsTestParam->mnMinTotalPackets && uGoodPackets <= mpsTestParam->mnMinGoodPackets*/ && fEvm <= mpsTestParam->mnMinEvm)
#else
                        if ((nRssi <= mpsTestParam->mnMinRssi && fEvm >= mpsTestParam->mfMinEvm) || (uTotalPackets <= mpsTestParam->mnMinTotalPackets && uGoodPackets <= mpsTestParam->mnMinGoodPackets))
#endif
                        {
                            nRxAttenError--;
                        }
                        else
                        {
                            nRxAttenError = mpsTestParam->mnErrorRetryTest;
                        }
                        JGW_FormatWString(strTemp, L"%d,%d,%d,%d,%d,%d,%d,%.2f", nTxPower,uTxAttenVga1,uTxAttenVga2, nRxAtten, uTotalPackets, uGoodPackets, nRssi, fEvm);
                        mfIperfLog << strTemp << std::endl;
                        if (0 == nRxAttenError)
                        {
                            break;
                        }
                    }
                }
            }
#ifndef RX_SINGLE_DEBUG
            mcTxQmslWlandDevice.StopTx();
#endif
        }
TEST_END:
        if (mfIperfLog.is_open()) { mfIperfLog.flush();mfIperfLog.close();}
        mcTxSshShell.DisConnectSSHShell();
        mcRxSshShell.DisConnectSSHShell();
        mcTxQmslWlandDevice.CloseDut();
        mcRxQmslWlandDevice.CloseDut();
        mcTxQmslWlandDevice.DisconnectStandaloneWlanCard();
        mcRxQmslWlandDevice.DisconnectStandaloneWlanCard();
        JGW_FormatWString(strTemp, L"Total Test Time:%d s", sw.elapsedSeconds());
        SendTestLogToWindow(strTemp);
        SendTestLogToWindow(L"End WiFi Test");
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
    }

    void CPTP4100WiFi6TestThread::SendTestLogToWindow(const std::wstring& strTestLog)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTestLog.c_str());
    }

    bool CPTP4100WiFi6TestThread::CheckSocketConnect(const std::wstring& strIPAddress,const std::wstring& strSourceAddress,int nTimeOutSec /* = 60 */)
    {
        jgw_timer jt;
        bool bResult = false;
        std::string strIPAddressA = JGW_W2A(strIPAddress);
        std::string strSourceAddressA = JGW_W2A(strSourceAddress);
        std::wstring strTemp;

        do 
        {
            JGW_FormatWString(strTemp,L"Start Ping Dest IP address : %s,  Source interface or IP address : %s..",strIPAddress.c_str(),strSourceAddress.c_str());
            SendTestLogToWindow(strTemp);
            bResult = mIcmpSocket.Ping(strIPAddressA,strSourceAddressA.empty()?NULL:strSourceAddressA.c_str(),2);
            if (!bResult) Sleep(1000);
        } while (jt.elapsed() <= nTimeOutSec && !bResult);

        if (!bResult)
        {
            JGW_FormatWString(strTemp,L"Ping Dest IP address : %s,  Source interface or IP address : %s.Error : %s",strIPAddress.c_str(),strSourceAddress.c_str(),JGW_A2W(mIcmpSocket.GetPingResult()).c_str());
            SendTestLogToWindow(strTemp);
        }

        return bResult;
    }

    bool CPTP4100WiFi6TestThread::CheckSSHConnect(CCJGW_SSHShell& sshShell,const std::wstring& strIPAddress, const std::wstring& strSourceAddress)
    {
        std::string server_name = JGW_W2A(strIPAddress);
        std::string strUserName = "root"; 
        std::string strPassword = "20171015";
        std::string strSourceAddressA = JGW_W2A(strSourceAddress);
        std::wstring strTemp;

        if (!sshShell.ConnectSSHShell(server_name,strUserName,strPassword,strSourceAddressA.empty() ? NULL : strSourceAddressA.c_str()))
        {
            JGW_FormatWString(strTemp, L"Connect SSH  %s error.", strIPAddress.c_str());
            SendTestLogToWindow(strTemp);
            return false;
        }

        return true;
    }

    bool CPTP4100WiFi6TestThread::SwitchWiFiFtm(CCJGW_SSHShell& sshShell,const std::string& strAddress)
    {
        std::string strRead;

        strRead = "";
        sshShell.SendSSHShell("\r\n");
        sshShell.RecvSSHShell(strRead,3);
        SendTestLogToWindow(JGW_A2W(strRead));

        strRead = "";
        sshShell.SendSSHShell("wifi down\r\n");
        sshShell.RecvSSHShell(strRead,3);
        SendTestLogToWindow(JGW_A2W(strRead));

        strRead = "";
        sshShell.SendSSHShell("rmmod monitor\r\n");
        sshShell.RecvSSHShell(strRead, 3);
        SendTestLogToWindow(JGW_A2W(strRead));

        strRead = "";
        sshShell.SendSSHShell("rmmod ecm_wifi_plugin\r\n");
        sshShell.RecvSSHShell(strRead, 3);
        SendTestLogToWindow(JGW_A2W(strRead));

        strRead = "";
        sshShell.SendSSHShell("rmmod wifi_3_0\r\n");
        sshShell.RecvSSHShell(strRead, 3);
        SendTestLogToWindow(JGW_A2W(strRead));

        strRead = "";
        sshShell.SendSSHShell("rmmod qca_ol\r\n");
        sshShell.RecvSSHShell(strRead, 3);
        SendTestLogToWindow(JGW_A2W(strRead));

        strRead = "";
        sshShell.SendSSHShell("insmod qca_ol testmode=1\r\n");
        sshShell.RecvSSHShell(strRead, 3);
        SendTestLogToWindow(JGW_A2W(strRead));

        strRead = "";
        sshShell.SendSSHShell("insmod wifi_3_0\r\n");
        sshShell.RecvSSHShell(strRead, 3);
        SendTestLogToWindow(JGW_A2W(strRead));

        strRead = "";
        JGW_FormatString(strRead,"diag_socket_app -a %s &\r\n",strAddress.c_str());
        sshShell.SendSSHShell(strRead);
        sshShell.RecvSSHShell(strRead, 3);
        SendTestLogToWindow(JGW_A2W(strRead));

        strRead = "";
        sshShell.SendSSHShell("/etc/init.d/ftm start\r\n");
        sshShell.RecvSSHShell(strRead, 3);
        SendTestLogToWindow(JGW_A2W(strRead));

        strRead = "";
        sshShell.SendSSHShell("ftm -n -c /tmp/ftm.conf &\r\n");
        sshShell.RecvSSHShell(strRead, 3);
        SendTestLogToWindow(JGW_A2W(strRead));

        return true;
    }

    bool CPTP4100WiFi6TestThread::ConnectQMSLPhone(CCJGWQMSLWlanDeivce& qmslWlandDevice,const std::wstring& strIPAddress)
    {
        bool bResult = true;
        jgw_timer jt;
        int nTimeOutSec = 30;
        std::string strComPort = JGW_W2A(strIPAddress);

        do
        {
            bResult = qmslWlandDevice.ConnectStandaloneWlanCardQUTS(strComPort);
            if (!bResult)
            {
                Sleep(1000);
            }
        } while (jt.elapsed() <= nTimeOutSec && !bResult);

        return bResult;
    }

    bool CPTP4100WiFi6TestThread::LoadDut6G(CCJGWQMSLWlanDeivce& qmslWlandDevice)
    {
        bool bResult = qmslWlandDevice.LoadDut();
        qmslWlandDevice.SetWlanInstance(mnChannel);
        qmslWlandDevice.WlanSetDutTxMacAddressBssId();
        return bResult;
    }

    bool CPTP4100WiFi6TestThread::CheckPTP4100UsbConnect(CCJGW_SSHShell& sshShell)
    {
        if (!mpsTestParam->mbIsUseSerialPort) return true;
        std::wstring strCommand (L"find /dev -name ttyACM0\r\n");
        std::string strRead;

        for (int index = 0;index < 20;index++)
        {
            sshShell.SendSSHShell("\r\n");
            sshShell.RecvSSHShell(strRead);

            if (!sshShell.SendSSHShell(JGW_W2A(strCommand)))
            {
                SendTestLogToWindow(L"Sending the SSH command failed");
                Sleep(1000);
                continue;
            }

            sshShell.RecvSSHShell(strRead,30);
            JGW_EraseFristToRemoveChar(strRead,L'\n');
            JGW_EraseAfterToRemoveChar(strRead,L'\n'); 
            JGW_EraseLastAndFristTwoChars(strRead);

            if (NULL != JGW_StrCaseStr(strRead.c_str(),"/dev/ttyACM0"))
            {
                strRead = "";
                sshShell.SendSSHShell("stty-coreutils -F /dev/ttyACM0 9600 raw -echo\r\n");
                sshShell.RecvSSHShell(strRead, 3);
                SendTestLogToWindow(JGW_A2W(strRead));
                return true;
            }
            Sleep(1000);
        }
        return false;
    }

    bool CPTP4100WiFi6TestThread::SettingTxSerialVGA1AndVGA2Atten(unsigned char uTxAttenVga1, unsigned char uTxAttenVga2)
    {
        if (!mpsTestParam->mbIsUseSerialPort) return true;
        // ÉèÖÃVGA1 0x01
        // echo -ne '\xFF\xFE\x20\x32\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00\xFE\xFF\x0A'> /dev/ttyACM0
        // ÉèÖÃVGA2  0x01
        // echo -ne '\xFF\xFE\x20\x32\x01\x00\x00\x01\x00\x00\x00\x00\x00\x00\xFE\xFF\x0A'> /dev/ttyACM0
        std::string strSettingDVGA12CommandFormat("echo -ne '\\xFF\\xFE\\x20\\x32\\x01\\x00\\x00\\x%02x\\x%02x\\x00\\x00\\x00\\x00\\x00\\xFE\\xFF\\x0A'> /dev/ttyACM0"),strSettingDVGA12Command,strRead;
        JGW_FormatString(strSettingDVGA12Command,strSettingDVGA12CommandFormat.c_str(),uTxAttenVga2,uTxAttenVga1);

        mcTxSshShell.SendSSHShell("\r\n");
        mcTxSshShell.RecvSSHShell(strRead);

        if (!mcTxSshShell.SendSSHShell(strSettingDVGA12Command))
        {
            SendTestLogToWindow(L"Sending the SSH command failed");
            return false;
        }

        return true;
    }

    bool CPTP4100WiFi6TestThread::SettingRxSerialAtten(unsigned char uRxAtten)
    {
        if (!mpsTestParam->mbIsUseSerialPort) return true;
        //ÉèÖÃRX ATT 0x01
        //echo -ne '\xFF\xFE\x20\x32\x01\x00\x00\x00\x00\x01\x00\x00\x00\x00\xFE\xFF\x0A'> /dev/ttyACM0
        std::string strSettingRxAttCommandFormat("echo -ne '\\xFF\\xFE\\x20\\x32\\x01\\x00\\x00\\x00\\x00\\x%02x\\x00\\x00\\x00\\x00\\xFE\\xFF\\x0A'> /dev/ttyACM0"),strSettingRxAttCommand,strRead;
        JGW_FormatString(strSettingRxAttCommand,strSettingRxAttCommandFormat.c_str(),uRxAtten);

        mcRxSshShell.SendSSHShell("\r\n");
        mcRxSshShell.RecvSSHShell(strRead);

        if (!mcRxSshShell.SendSSHShell(strSettingRxAttCommand))
        {
            SendTestLogToWindow(L"Sending the SSH command failed");
            return false;
        }

        return true;
    }

    void CPTP4100WiFi6TestThread::BuildWiFiTestLog()
    {
        if (mfIperfLog.is_open()) { mfIperfLog.flush();mfIperfLog.close();}
        std::wstring strIperfLogName;
        mfIperfLog.imbue(std::locale(""));
        JGW_FormatWString(strIperfLogName,L"%sTestLog\\%s\\ch%d_wifi_test_log_%u.csv",JGW_GetApplicationFolder(),JGW_GetTimeFolder().c_str(),mnChannel,JGW_GetTimeOfDay());
        JGW_CreateDirectory(strIperfLogName.c_str(),false);
        mfIperfLog.open(strIperfLogName);
    }

    int CPTP4100WiFi6TestThread::GetTestChannel()
    {
        std::vector<S_TLV2_CONFIG>& vTxTlv2Configs = mcQMSLConfigXml.GetTxTlv2Configs();
        for (std::vector<S_TLV2_CONFIG>::iterator it = vTxTlv2Configs.begin();
            it != vTxTlv2Configs.end();++it)
        {
            for (std::vector<S_PARAM_NAME_VALUE>::iterator itVec = it->mvParamNameValues.begin();itVec != it->mvParamNameValues.end();++itVec)
            {
                if (0 == JGW_StrComparenoCaseStr(itVec->mstrParamName.c_str(),"channel"))
                {
                    mnChannel = atoi(itVec->mstrParamValue.c_str());
                    return mnChannel;
                }
            }
        }
        return mnChannel;
    }

    bool CPTP4100WiFi6TestThread::StartQMSLRx()
    {
        std::vector<S_TLV2_CONFIG>& vRxTlv2Configs = mcQMSLConfigXml.GetRxTlv2Configs();
        for (int index = 0;index < mpsTestParam->mnErrorRetryTest;index ++)
        {
            if (vRxTlv2Configs.empty())
            {
                if (mcRxQmslWlandDevice.StartRx())
                {
                    return true;
                }
            }
            else
            {
                mcRxQmslWlandDevice.SetWlanInstance(mnChannel);
                if (mcRxQmslWlandDevice.ExecTlv2ConfigFlow(vRxTlv2Configs))
                {
                    return true;
                }
            } 
        }
        return false;
    }

    bool CPTP4100WiFi6TestThread::StartQMSLTx(int nTxPower0)
    {
        std::vector<S_TLV2_CONFIG>& vTxTlv2Configs = mcQMSLConfigXml.GetTxTlv2Configs();
        for (int index = 0;index < mpsTestParam->mnErrorRetryTest;index++)
        {
            if (vTxTlv2Configs.empty())
            {
                if (mcTxQmslWlandDevice.StartTx(nTxPower0))
                {
                    return true;
                }
            }
            else
            {
                for (std::vector<S_TLV2_CONFIG>::iterator it = vTxTlv2Configs.begin();
                    it != vTxTlv2Configs.end();++it)
                {
                    for (std::vector<S_PARAM_NAME_VALUE>::iterator itVec = it->mvParamNameValues.begin();itVec != it->mvParamNameValues.end();++itVec)
                    {
                        if (0 == JGW_StrComparenoCaseStr(itVec->mstrParamName.c_str(),"txPower0"))
                        {
                            JGW_FormatString(itVec->mstrParamValue,"%d",nTxPower0);
                        }
                    }
                }
                mcTxQmslWlandDevice.SetWlanInstance(mnChannel);
                if (mcTxQmslWlandDevice.ExecTlv2ConfigFlow(vTxTlv2Configs))
                {
                    return true;
                }    
            }
        }
        return false;
    }
}
