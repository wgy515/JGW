#include "StdAfx.h"
#include "CJGW_IPQSshThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
    CCJGW_IPQSshThread::CCJGW_IPQSshThread(bool& bRunTestThread,S_TestParams* psTestParams,CCJGW_AsyncWndMessage* pAsyncWndMessage) : mbRunTestThread(bRunTestThread),mpsTestParams(psTestParams),mpAsyncWndMessage(pAsyncWndMessage),mb11ADIsConnected(false),mbIsUseUbusPrsLinkeInfoCommand(true),mbIsExitIPQSshThread(false)
    {
    }


    CCJGW_IPQSshThread::~CCJGW_IPQSshThread(void)
    {
    }

    bool CCJGW_IPQSshThread::Check11ADConnected()
    {
        return mb11ADIsConnected;
    }

    bool CCJGW_IPQSshThread::ExecuteSshCommand(S_SSH_COMMAND & sSshCommand)
    {
        std::string strCommand (sSshCommand.mstrCommand);
        if (strCommand.empty()) return true;

        for (int nIndex = 0;nIndex < sSshCommand.mnErrorRetry;nIndex++)
        {
            sSshCommand.mstrRead = "";
            if (!mcSSHShell.RWSSHShell(strCommand,sSshCommand.mstrRead))
            {
                if (sSshCommand.mstrRead.empty())
                {
                    SendWindowMsg(L"Failed to send the SSH command : %s",JGW_A2W(strCommand).c_str());
                    return false;
                }
            }

            if (sSshCommand.mvstrCommandOk.empty()) return true;
            //! 判断字符串是与或
            if (sSshCommand.mbIsAndCommandOk)
            {
                for (std::vector<std::string>::iterator it = sSshCommand.mvstrCommandOk.begin();it != sSshCommand.mvstrCommandOk.end();++it)
                {
                    if (NULL == JGW_StrCaseStr(sSshCommand.mstrRead.c_str(),it->c_str()))
                    {
                        return false;
                    }
                }
                return true;
            }
            else
            {
                for (std::vector<std::string>::iterator it = sSshCommand.mvstrCommandOk.begin();it != sSshCommand.mvstrCommandOk.end();++it)
                {
                    if (NULL != JGW_StrCaseStr(sSshCommand.mstrRead.c_str(),it->c_str()))
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool CCJGW_IPQSshThread::StartIPQSshThread()
    {
        mbIsUseUbusPrsLinkeInfoCommand = true;
        //! 1、检查SSH IP是否能PING 通
        Log4WI_F(L"Check SSH (%s) Internet connection ..",mpsTestParams->mstrSSHIPAddr.c_str());
        SendWindowMsg(L"Check SSH (%s) Internet connection ..",mpsTestParams->mstrSSHIPAddr.c_str());
        if (!CheckNetworkConnection(mpsTestParams->mstrSSHIPAddr))
        {
            SendWindowMsg(L"Failed to check the ssh network(%s) connection",mpsTestParams->mstrSSHIPAddr.c_str());
            Log4WE_F(L"Check SSH (%s) Internet connection Failed",mpsTestParams->mstrSSHIPAddr.c_str());
            return false;
        }
        //! 2、连接SSH服务
        Log4WI_F(L"Connecting to the SSH(%s) Service",mpsTestParams->mstrSSHIPAddr.c_str());
        SendWindowMsg(L"Connecting to the SSH(%s) Service",mpsTestParams->mstrSSHIPAddr.c_str());
        if (!mcSSHShell.ConnectSSHShell(JGW_W2A(mpsTestParams->mstrSSHIPAddr),mpsTestParams->mstrUserNameSSH,mpsTestParams->mstrPasswordSSH,NULL,22))
        {
            SendWindowMsg(L"SSH(%s) connection failed",mpsTestParams->mstrSSHIPAddr.c_str());
            Log4WE_F(L"Connecting to the SSH(%s) Service Failed",mpsTestParams->mstrSSHIPAddr.c_str());
            return false;
        }

        if (mpsTestParams->mbIsSaveIPQLog)
        {
            StopIPQLog();
            //! 3、开启测试日志
            SendWindowMsg(L"Start cat /proc/kmsg log");
            if (!StartProcKmsgLog())
            {
                SendWindowMsg(L"cat proc/kmsg failed");
                return false;
            }

            //! 3、开启测试日志
            SendWindowMsg(L"Start logread -f log");
            if (!StartLogReadLog())
            {
                SendWindowMsg(L"logread -f failed");
                return false;
            }
        }

        //! init ssh
        for (std::vector<S_SSH_COMMAND>::iterator it = mpsTestParams->mvInitSSHCommand.begin();it != mpsTestParams->mvInitSSHCommand.end();++it)
        {
            if (!ExecuteSshCommand(it[0]))
            {
                SendWindowMsg(L"Failed to send the SSH command : %s,recv : %s,export : %s",JGW_A2W(it->mstrCommand).c_str(),JGW_A2W(it->mstrRead).c_str(),JGW_A2W(it->mstrCommandOk).c_str());
                return false;
            }
        }
		mcSSHShell.DisConnectSSHShell();

		if (!mcSSHShell.ConnectSSHShell(JGW_W2A(mpsTestParams->mstrSSHIPAddr),mpsTestParams->mstrUserNameSSH,mpsTestParams->mstrPasswordSSH,NULL,22))
		{
			SendWindowMsg(L"SSH(%s) connection failed",mpsTestParams->mstrSSHIPAddr.c_str());
			Log4WE_F(L"Connecting to the SSH(%s) Service Failed",mpsTestParams->mstrSSHIPAddr.c_str());
			return false;
		}

#if 1
        //! 3、开启SSH服务线程，每隔一秒读取11AD 连接状态
        Log4WI_F(L"Enable the SSH(%s) service thread to read the 11AD connection status every %d millisecond",mpsTestParams->mstrSSHIPAddr.c_str(),mpsTestParams->mnCheckSignalTimeIntervalMS);
        SendWindowMsg(L"Enable the SSH(%s) service thread to read the 11AD connection status every %d millisecond",mpsTestParams->mstrSSHIPAddr.c_str(),mpsTestParams->mnCheckSignalTimeIntervalMS);
        if (!mcReadWriteSSHThread.CreateMessageThread(OnReadWriteSSHThreadCallback,this,true))
        {
            SendWindowMsg(L"Description Failed to start the SSH(%s) read/write thread",mpsTestParams->mstrSSHIPAddr.c_str());
            Log4WE_F(L"Description Failed to start the SSH(%s) read/write thread",mpsTestParams->mstrSSHIPAddr.c_str());
            return false;
        }
#endif

        return true;
    }

    void CCJGW_IPQSshThread::StopIPQSshThread()
    {
        mbRunTestThread = false;
        if (mpsTestParams->mbIsSaveIPQLog)
        {
            if (mcSSHShell.IsSSHConnected())
            {
                StopIPQLog();
                ExportIPQLog();
            }
        }
        if (mcSSHShell.IsSSHConnected())
        {
            //! exit ssh
            for (std::vector<S_SSH_COMMAND>::iterator it = mpsTestParams->mvExitSSHCommand.begin();it != mpsTestParams->mvExitSSHCommand.end();++it)
            {
                if (!ExecuteSshCommand(it[0]))
                {
                    SendWindowMsg(L"Failed to send the SSH command : %s,recv : %s,export : %s",JGW_A2W(it->mstrCommand).c_str(),JGW_A2W(it->mstrRead).c_str(),JGW_A2W(it->mstrCommandOk).c_str());
                }
            }
        }
        mb11ADIsConnected = false;    
    }

    void CCJGW_IPQSshThread::TerminateIPQSshThread()
    {
        mcSSHScp.DisConnectSSHSCP();
        mcSSHShell.DisConnectSSHShell();
        mcReadWriteSSHThread.TerminateMessageThread();
    }

    bool CCJGW_IPQSshThread::ExportIPQLog()
    {
        Log4WI_F(L"Connecting to the SCP(%s) Service",mpsTestParams->mstrSSHIPAddr.c_str());
        if (!mcSSHScp.ConnectSSHSCP(JGW_W2A(mpsTestParams->mstrSSHIPAddr),mpsTestParams->mstrUserNameSSH,mpsTestParams->mstrPasswordSSH,NULL,22))
        {
            SendWindowMsg(L"SCP(%s) connection failed",mpsTestParams->mstrSSHIPAddr.c_str());
            Log4WE_F(L"Connecting to the SCP(%s) Service Failed",mpsTestParams->mstrSSHIPAddr.c_str());
            return false;
        }

        std::string strLocalPorcKmsgLogPath;
        JGW_FormatString(strLocalPorcKmsgLogPath,"%s/%u_kmsg.log",JGW_W2A(mpsTestParams->mstrLogFolder).c_str(),mpsTestParams->mnTimeStamp);
        if (!mcSSHScp.GetFileSCP(mstrProcKmsgLogPath,strLocalPorcKmsgLogPath))
        {
            SendWindowMsg(L"get proc/kmsg log(%s) failed",JGW_A2W(mstrProcKmsgLogPath).c_str());
            Log4WE_F(L"get proc/kmsg log(%s) failed",JGW_A2W(mstrProcKmsgLogPath).c_str());
            return false;
        }
        SendWindowMsg(L"proc/kmsg log : %s",JGW_A2W(strLocalPorcKmsgLogPath).c_str());

        std::string strLocalLogReadLogPath;
        JGW_FormatString(strLocalLogReadLogPath,"%s/%u_logread.log",JGW_W2A(mpsTestParams->mstrLogFolder).c_str(),mpsTestParams->mnTimeStamp);
        if (!mcSSHScp.GetFileSCP(mstrLogReadLogPath,strLocalLogReadLogPath))
        {
            SendWindowMsg(L"get logread log(%s) failed",JGW_A2W(mstrLogReadLogPath).c_str());
            Log4WE_F(L"get logread log(%s) failed",JGW_A2W(mstrLogReadLogPath).c_str());
            return false;
        }
        SendWindowMsg(L"logread log : %s",JGW_A2W(strLocalLogReadLogPath).c_str());

        mcSSHScp.DisConnectSSHSCP();
        return true;
    }

    bool CCJGW_IPQSshThread::CheckStartSSHIPQLog(const std::string& strCommand,const std::string& strCheckCommand,const std::string& strSuccessResult)
    {
        jgw_timer jt;
        std::string strRead;
        while (jt.elapsed() < 10)
        {
            if (!mcSSHShell.RWSSHShell(strCommand,strRead))
            {
                Sleep(100);
                continue;
            }

            for (int index = 0;index < 3;index ++)
            {
                if (!mcSSHShell.RWSSHShell(strCheckCommand,strRead))
                {
                    Sleep(100);
                    continue;
                }

                if (std::string::npos != strRead.find(strSuccessResult))
                {
                    return true;
                }
                Sleep(100);
            }
        }
        return false;
    }

    bool CCJGW_IPQSshThread::StartProcKmsgLog()
    {
        std::string strCommand;
        //!  cat /proc/kmsg > file_1.log
        JGW_FormatString(mstrProcKmsgLogPath,"%s/%u_kmsg.log",IPQ_LOG_FOLDER,mpsTestParams->mnTimeStamp);
        JGW_FormatString(strCommand,IPQ_PROC_KMSG_COMMAND_FORMAT,mstrProcKmsgLogPath.c_str());

        if (!CheckStartSSHIPQLog(strCommand,"ls -l\n",JGW_GetFormatString("%u_kmsg.log",mpsTestParams->mnTimeStamp)))
        {
            return false;
        }

        return true;
    }

    bool CCJGW_IPQSshThread::StartLogReadLog()
    {
        std::string strCommand,strRead;
        //!  cat /proc/kmsg > file_1.log
        JGW_FormatString(mstrLogReadLogPath,"%s/%u_logread.log",IPQ_LOG_FOLDER,mpsTestParams->mnTimeStamp);
        JGW_FormatString(strCommand,IPQ_LOGREAD_COMMAND_FORMAT,mstrLogReadLogPath.c_str());

        if (!CheckStartSSHIPQLog(strCommand,"ls -l\n",JGW_GetFormatString("%u_logread.log",mpsTestParams->mnTimeStamp)))
        {
            return false;
        }

        return strRead.empty();
    }

    bool CCJGW_IPQSshThread::StopSSHIPQLog(const std::string& strCommand,const std::string& strSuccessResult)
    {
        std::string strRead;
        for (int index = 0;index < 3;index ++)
        {
            if (!mcSSHShell.RWSSHShell(strCommand,strRead))
            {
                return false;
            }

            if (std::string::npos == strRead.find(strSuccessResult))
            {
                Sleep(100);
                continue;
            }

            int nPid = atoi(strRead.c_str());
            if (nPid > 0)
            {
                mcSSHShell.RWSSHShell(JGW_GetFormatString("kill -15 %d\n",nPid),strRead);
                mcSSHShell.RWSSHShell(JGW_GetFormatString("\n"),strRead);
                return true;
            }
        } 
        return true;
    }

    bool CCJGW_IPQSshThread::StopIPQLog()
    {
        if (!mcSSHShell.IsSSHConnected())
        {
            if (!mcSSHShell.ConnectSSHShell(JGW_W2A(mpsTestParams->mstrSSHIPAddr),mpsTestParams->mstrUserNameSSH,mpsTestParams->mstrPasswordSSH,NULL,22))
            {
                return false;
            }
        }
#if 0
        //! /proc/kmsg
        std::string strRead;
        mcSSHShell.RecvSSHShell(strRead,3);
        if (!mcSSHShell.RWSSHShell(IPQ_GET_CAT_PROC_KMSG_PID,strRead))
        {
            return false;
        }
        int nPid = atoi(strRead.c_str());
        if (nPid > 0)
        {
            mcSSHShell.RWSSHShell(JGW_GetFormatString("kill -15 %d\n",nPid),strRead);
            mcSSHShell.RWSSHShell(JGW_GetFormatString("\n"),strRead);
        }

        if (!mcSSHShell.RWSSHShell(IPQ_GET_LOGREAD_PID,strRead))
        {
            return false;
        }
        nPid = atoi(strRead.c_str());
        if (nPid > 0)
        {
            mcSSHShell.RWSSHShell(JGW_GetFormatString("kill -15 %d\n",nPid),strRead);
            mcSSHShell.RWSSHShell(JGW_GetFormatString("\n"),strRead);
        }
#else
        StopSSHIPQLog(IPQ_GET_CAT_PROC_KMSG_PID,"/proc/kmsg");
        StopSSHIPQLog(IPQ_GET_LOGREAD_PID," logread -f");
#endif
        return true;
    }

    void CCJGW_IPQSshThread::OnReadWriteSSHThreadCallback(void *lpParam)
    {
        CCJGW_IPQSshThread* pIPQSshThread = (CCJGW_IPQSshThread*)lpParam;
        if (NULL != pIPQSshThread)
        {
            pIPQSshThread->ReadWriteSSHThreadCallback();
        }
    }

    /*
    {
    "station-1": {
    "ssid": "doppler_test",
    "peer_mac": " 7C 6C F0 AF FF 95 ",
    "beamforming_rssi": -35,
    "remote_rssi": -53,
    "beacon_rssi": -128,
    "local_rssi": -35,
    "average_rssi": -51,
    "tx_sector": 28,
    "rx_sector": 27,
    "current_mcs": 12,
    "max_mcs": 12,
    "min_mcs": 1,
    "distance": 7,
    "snr": 19,
    "associated_time": 21108,
    "center_frequency": 69120,
    "bandwidth": 2160,
    "channel": 6
    }
    }
    */
    bool CCJGW_IPQSshThread::GetRssiMcsTRxSectorUbusPrsLinkeInfoCommand(int& nRssi,int& nMcs,int& nTxSector,int& nRxSector)
    {
        //! ubus call prs link_info
        std::string strCommand("ubus call prs link_info\n"),strRead;
        if (!mcSSHShell.RWSSHShell(strCommand,strRead))
        {
			if (strRead.empty()) return false;
        }


        Log4WI(JGW_A2W(strRead).c_str());
        if (NULL != JGW_StrCaseStr(strRead.c_str(),"Command failed"))
        {
            mbIsUseUbusPrsLinkeInfoCommand  = false;
            return false;
        }
#if 0
        bool bPrev11ADConnectedStatu = mb11ADIsConnected;
        mb11ADIsConnected = (std::string::npos != strRead.find("\"local_rssi\":"));
        if (bPrev11ADConnectedStatu != mb11ADIsConnected)
        {
            //m11ADConnectEventt.set();
        }

        if (mb11ADIsConnected)
        {
            nRssi = atoi(JGW_GetSubStrToStartEndStr(strRead,"\"local_rssi\":",",","-128").c_str());
            nMcs = atoi(JGW_GetSubStrToStartEndStr(strRead,"\"current_mcs\":",",","-1").c_str());
            nTxSector = atoi(JGW_GetSubStrToStartEndStr(strRead,"\"tx_sector\":",",","-1").c_str());
            nRxSector = atoi(JGW_GetSubStrToStartEndStr(strRead,"\"rx_sector\":",",","-1").c_str());
        }
#else
		if (std::string::npos != strRead.find("\"local_rssi\":"))
		{
			mb11ADIsConnected = true;
			nRssi = atoi(JGW_GetSubStrToStartEndStr(strRead,"\"local_rssi\":",",","-128").c_str());
			nMcs = atoi(JGW_GetSubStrToStartEndStr(strRead,"\"current_mcs\":",",","-1").c_str());
			nTxSector = atoi(JGW_GetSubStrToStartEndStr(strRead,"\"tx_sector\":",",","-1").c_str());
			nRxSector = atoi(JGW_GetSubStrToStartEndStr(strRead,"\"rx_sector\":",",","-1").c_str());
			return true;
		}
		else if (std::string::npos != strRead.find("\"rssi\":"))
		{
			mb11ADIsConnected = true;
			nRssi = atoi(JGW_GetSubStrToStartEndStr(strRead,"\"rssi\":",",","-128").c_str());
			nMcs = atoi(JGW_GetSubStrToStartEndStr(strRead,"\"current_mcs\":",",","-1").c_str());
			nTxSector = atoi(JGW_GetSubStrToStartEndStr(strRead,"\"tx_sector\":",",","-1").c_str());
			nRxSector = atoi(JGW_GetSubStrToStartEndStr(strRead,"\"rx_sector\":",",","-1").c_str());
			return true;
		}
		mb11ADIsConnected = false;
		/*
		{
		"linked": 1,
		"mac": " 20 CE C4 03 C0 3E ",
		"mode": "STA",
		"ssid": "PTP-60G-97BE",
		"channel": 2,
		"tx_rate": 98,
		"rx_rate": 4,
		"tx_bytes": -1055412180,
		"tx_bytes": 176587656,
		"radio_temp": 81,
		"baseband_temp": 78,
		"peer_mac": " 20 CE C4 03 BF 2F ",
		"rssi": -46,
		"average_rssi": -46,
		"tx_sector": 26,
		"rx_sector": 40,
		"current_mcs": 12,
		"tx_mcs": 12,
		"rx_bitrate": 4620,
		"tx_bitrate": 4620,
		"distance": 5,
		"snr": 12,
		"associated_time": 949
		}
		*/
#endif
        return true;
    }

    bool CCJGW_IPQSshThread::GetRssiMcs(int& nRssi,int& nMcs)
    {
        //! 
        std::string strCommand(mpsTestParams->mstrCheck11ADConnectCommand),strRead;
        if (!mcSSHShell.RWSSHShell(strCommand,strRead))
        {
			if (strRead.empty()) return false;
            return false;
        }
        /*
        Station 7c:6c:f0:af:ff:95 (on wlan0)
        inactive time:  0 ms
        rx packets:     0
        tx packets:     0
        signal:         -60 dBm
        signal avg:     -58 dBm
        tx bitrate:     1925.0 MBit/s MCS 7
        rx bitrate:     1155.0 MBit/s MCS 4
        */
        /*
        root@mmwave:~# iw wlan0 station dump
        Station 7c:6c:f0:af:ff:95 (on wlan0)
        inactive time:  0 ms
        rx bytes:       44966
        rx packets:     144
        tx bytes:       4063
        tx packets:     50
        signal:         -49 dBm
        signal avg:     -49 dBm
        tx bitrate:     962.5 MBit/s
        rx bitrate:     385.0 MBit/s
        connected time: 146 seconds
        associated at [boottime]:       1743175.945s
        associated at:  18446727371884570018 ms
        current time:   1743176092037 ms
        */
        bool bPrev11ADConnectedStatu = mb11ADIsConnected;
        mb11ADIsConnected = (std::string::npos != strRead.find(mpsTestParams->mstr11ADConnectedFindString));
        if (bPrev11ADConnectedStatu != mb11ADIsConnected)
        {
            //m11ADConnectEventt.set();
        }

        if (mb11ADIsConnected)
        {
            nRssi = atoi(JGW::JGW_GetSubStrToStartEndStr(strRead,mpsTestParams->mstrSignalConfig,"\n").c_str());
#if 0
            std::string strMCS = JGW::JGW_GetRSubStrToStartEndStr(strRead,mpsTestParams->mstrMcsConfig,"\n");
            strMCS += "\n";
            strMCS = JGW::JGW_GetRSubStrToStartEndStr(strMCS,"MCS","\n");
            JGW_FormatWString(strTemp,L"%d,%d,%d",1,atoi(strSignal.c_str()),atoi(strMCS.c_str()));
#else 
            std::string strReteSpeed = JGW::JGW_GetRSubStrToStartEndStr(strRead,mpsTestParams->mstrMcsConfig,"\n");
            JGW_ReplaceStringA(strReteSpeed," ","");
            Log4WI(JGW_A2W(strReteSpeed).c_str());

            nMcs = -1;
            int nRateSpeed = atoi(strReteSpeed.c_str());
            static int gvRateSpeed[] = {0,385,770,962,1155,1251,1540,1925,2310,2500,3080,3850,4620};
            for (int index = 1;index < _countof(gvRateSpeed);index++)
            {
                if (nRateSpeed >= gvRateSpeed[index - 1] && nRateSpeed < gvRateSpeed[index])
                {
                    nMcs = index - 1;
                    break;
                }
            }
#endif
            return true;
        }


        return true;
    }

    int CCJGW_IPQSshThread::GetPerasoSector(const std::string& strCommand,int nSectorRow)
    {
        std::string strCommandA,strRead;

        JGW_FormatString(strCommandA,strCommand.c_str(),nSectorRow);
        if (!mcSSHShell.RWSSHShell(strCommand,strRead))
        {
			if (strRead.empty()) return false;
            return -1;
        }

        if (std::string::npos != strRead.find("00 "))
        {
            JGW_ReplaceStringA(strRead,"00 ","");
            return static_cast<int>(strtol(strRead.c_str(),NULL,16));
        }
        return -1;
    }

    bool CCJGW_IPQSshThread::GetTRxSector(int& nTxSector,int& nRxSector)
    {
        //! tx sector mibReadTxSector() mib q b 12-8-" + mibRow + -c; 
        //! prs_serial "mib q b 12-8-1-c" 00 1C
        //! strtol()
        std::string strCommand;
        static int gnTxSectorRow = 1;
        static int gnRxSectorRow = 1;

        nTxSector = GetPerasoSector("prs_serial \"mib q b 12-8-%d-c\"",gnTxSectorRow);
        if (-1 == nTxSector || nTxSector == 0xFF)
        {
            for (int i = 0; i < 10; i++)
            {
                nTxSector = GetPerasoSector("prs_serial \"mib q b 12-8-%d-c\"",i);
                if (-1 != nTxSector && nTxSector != 0xFF)
                {
                    gnTxSectorRow = i;
                    break;
                }
            }
        }

        //! rx sector mibReadRxSector() mib q b 12-8-" + mibRow + -d;
        nRxSector = GetPerasoSector("prs_serial \"mib q b 12-8-%d-d\"",gnTxSectorRow);
        if (-1 == nRxSector || nRxSector == 0xFF)
        {
            for (int i = 0; i < 10; i++)
            {
                nTxSector = GetPerasoSector("prs_serial \"mib q b 12-8-%d-d\"",i);
                if (-1 != nRxSector && nRxSector != 0xFF)
                {
                    gnTxSectorRow = i;
                    break;
                }
            }
        }

        return nRxSector != -1 && nRxSector != 0xFF && nTxSector != -1 && nTxSector != 0xFF;
    }

    void CCJGW_IPQSshThread::ReadWriteSSHThreadCallback()
    {
        std::string strRead;
        std::wstring strTemp;
        //! strTemp = L"0,-128,-1,-1,-1";
        int nRssi = -128,nMcs = -1,nTxSector = -1,nRxSector = -1;

        while (mbRunTestThread)
        {
#if 0
            if (!mcSSHShell.SendSSHShell(mpsTestParams->mstrCheck11ADConnectCommand))
            {
                mb11ADIsConnected = false;
                mpAsyncWndMessage->PutAsyncMessage(WM_11AD_CONNECT_STATUS_MSG,0);
                break;
            }
            strRead = "";
            mcSSHShell.RecvSSHShell(strRead,3);
            /*
            Station 7c:6c:f0:af:ff:95 (on wlan0)
            inactive time:  0 ms
            rx packets:     0
            tx packets:     0
            signal:         -60 dBm
            signal avg:     -58 dBm
            tx bitrate:     1925.0 MBit/s MCS 7
            rx bitrate:     1155.0 MBit/s MCS 4
            */
            /*
            root@mmwave:~# iw wlan0 station dump
            Station 7c:6c:f0:af:ff:95 (on wlan0)
            inactive time:  0 ms
            rx bytes:       44966
            rx packets:     144
            tx bytes:       4063
            tx packets:     50
            signal:         -49 dBm
            signal avg:     -49 dBm
            tx bitrate:     962.5 MBit/s
            rx bitrate:     385.0 MBit/s
            connected time: 146 seconds
            associated at [boottime]:       1743175.945s
            associated at:  18446727371884570018 ms
            current time:   1743176092037 ms
            */
            bool bPrev11ADConnectedStatu = mb11ADIsConnected;
            mb11ADIsConnected = (std::string::npos != strRead.find(mpsTestParams->mstr11ADConnectedFindString));
            if (bPrev11ADConnectedStatu != mb11ADIsConnected)
            {
                m11ADConnectEventt.set();
            }

            if (mb11ADIsConnected)
            {
                std::string strSignal = JGW::JGW_GetSubStrToStartEndStr(strRead,mpsTestParams->mstrSignalConfig,"\n");
#if 0
                std::string strMCS = JGW::JGW_GetRSubStrToStartEndStr(strRead,mpsTestParams->mstrMcsConfig,"\n");
                strMCS += "\n";
                strMCS = JGW::JGW_GetRSubStrToStartEndStr(strMCS,"MCS","\n");
                JGW_FormatWString(strTemp,L"%d,%d,%d",1,atoi(strSignal.c_str()),atoi(strMCS.c_str()));
#else 
                std::string strReteSpeed = JGW::JGW_GetRSubStrToStartEndStr(strRead,mpsTestParams->mstrMcsConfig,"\n");
                JGW_ReplaceStringA(strReteSpeed," ","");
                Log4WI(JGW_A2W(strReteSpeed).c_str());
                /*
                //mcs速率表
                static int32_t gMcsTable[16] =
                {
                0,      //mcs0
                385,    //mcs1
                770,    //mcs2
                962,    //mcs3
                1155,   //mcs4
                1251,   //mcs5
                1540,   //mcs6
                1925,   //mcs7
                2310,   //mcs8
                2500,   //mcs9
                3080,   //mcs10
                3850,   //mcs11
                4620    //mcs12
                };
                */

                int nMcs = -1;
                int nRateSpeed = atoi(strReteSpeed.c_str());

                static int gvRateSpeed[] = {0,385,770,962,1155,1251,1540,1925,2310,2500,3080,3850,4620};
                for (int index = 1;index < _countof(gvRateSpeed);index++)
                {
                    if (nRateSpeed >= gvRateSpeed[index - 1] && nRateSpeed < gvRateSpeed[index])
                    {
                        nMcs = index - 1;
                        break;
                    }
                }
#if 0
                if (mmapRateSpeedMcs.end() != mmapRateSpeedMcs.find(nRateSpeed))
                {
                    nMcs = mmapRateSpeedMcs[nRateSpeed];
                }
                JGW_FormatWString(strTemp,L"%d,%d,%d",1,atoi(strSignal.c_str()),nMcs);
#else
                //! 读取TX RX SECTOR
                if (!mcSSHShell.SendSSHShell(mpsTestParams->mstrCheck11ADConnectCommand))
                {
                    mb11ADIsConnected = false;
                    mpAsyncWndMessage->PutAsyncMessage(WM_11AD_CONNECT_STATUS_MSG,0);
                    break;
                }
                strRead = "";
                mcSSHShell.RecvSSHShell(strRead,3);







                JGW_FormatWString(strTemp,L"%d,%d,%d",1,atoi(strSignal.c_str()),nMcs);
#endif
#endif

            }
            else
            {
                strTemp = L"0,-128,-1,-1,-1";
            }
#endif
            if (mbIsUseUbusPrsLinkeInfoCommand)
            {
                if (!GetRssiMcsTRxSectorUbusPrsLinkeInfoCommand(nRssi,nMcs,nTxSector,nRxSector))
                {
                    mb11ADIsConnected = false;
                    mpAsyncWndMessage->PutAsyncStringMessage(WM_11AD_CONNECT_STATUS_MSG,L"0,-128,-1,-1,-1");
                    //break;
                }
            }
            else
            {
                if (!GetRssiMcs(nRssi,nMcs))
                {
                    mb11ADIsConnected = false;
                    mpAsyncWndMessage->PutAsyncStringMessage(WM_11AD_CONNECT_STATUS_MSG,L"0,-128,-1,-1,-1");
                    //break;
                }
                if (mb11ADIsConnected)
                {
                    GetTRxSector(nTxSector,nRxSector);
                }
            }

            if (mb11ADIsConnected)
            {
                JGW_FormatWString(strTemp,L"%d,%d,%d,%d,%d",1,nRssi,nMcs,nTxSector,nRxSector);
            }
            else
            {
                strTemp = L"0,-128,-1,-1,-1";
            }
            Log4WI(strTemp.c_str());
            mpAsyncWndMessage->PutAsyncStringMessage(WM_11AD_CONNECT_STATUS_MSG,strTemp.c_str());
            //pThroughputTestThread->mpAsyncWndMessage->PutAsyncStringMessage(WM_11AD_CONNECT_STATUS_MSG,pThroughputTestThread->mb11ADIsConnected ? 1 : 0);
            Sleep(mpsTestParams->mnCheckSignalTimeIntervalMS);
        }
        SendWindowMsg(L"Error: The SSH(%s) service connection is interrupted",mpsTestParams->mstrSSHIPAddr.c_str());
        mbRunTestThread = false;
    }

    bool CCJGW_IPQSshThread::CheckNetworkConnection(const std::wstring& strIPAddr,const std::wstring& strSourceAddr /* = L"" */,int nTestTimeSec /* = 30 */,int nTestContinuousCount /* = 2 */)
    {
        jgw_timer jt;
        bool bResult = false,bSingleResult = true;
        std::string strIPAddrA = JGW_W2A(strIPAddr);
        std::string strSourceAddrA = JGW_W2A(strSourceAddr);
        std::wstring strTemp;

        while (jt.elapsed() <= nTestTimeSec && !bResult && mbRunTestThread)
        {
            bResult = true;
            for (int index = 0;index < nTestContinuousCount && mbRunTestThread;index++)
            {
                bSingleResult = mIcmpSocket.Ping(strIPAddrA,strSourceAddrA.empty()?NULL:strSourceAddrA.c_str(),1000 * 10);
                SendWindowMsg(L"ping %s %s",strIPAddr.c_str(),JGW_A2W(mIcmpSocket.GetPingResult()).c_str());
                if (bSingleResult)
                {

                    JGW_FormatWString(strTemp,L"%.3lf",mIcmpSocket.GetPingTimeMs());
                    //mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,strTemp.c_str());
                }
                else
                {
                    //mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,L"");
                }
                bResult &= bSingleResult;
                Sleep(900);
            }
        }

        return bResult;
    }

    void CCJGW_IPQSshThread::SendWindowMsg(const std::wstring& strMessage)
    {
        Log4WI(strMessage.c_str());
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strMessage.c_str());
    }

    void CCJGW_IPQSshThread::SendWindowMsg(const wchar_t* szFormat,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,szFormat);
        Log4WI(strLog.c_str());
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strLog.c_str());
    }
}
