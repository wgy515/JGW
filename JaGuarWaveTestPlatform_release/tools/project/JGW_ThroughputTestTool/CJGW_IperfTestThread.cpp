#include "StdAfx.h"
#include "CJGW_IperfTestThread.h"
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
namespace JGW
{
    //! 角度  时间(s) 速率（Mbits/sec）
    CCJGW_IperfTestThread::CCJGW_IperfTestThread(void) : mpsIperfToolConfig(NULL),mpAsyncWndMessage(NULL),mcDonglesComPort(mcSerialComPort)
    {
    }


    CCJGW_IperfTestThread::~CCJGW_IperfTestThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_IperfTestThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCJGW_IperfTestThread::InitIperfTestThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCJGW_IperfTestThread::RunIperfTestThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_IperfTestThread::InitIperfTestThread(WPARAM wParam,LPARAM lParam)
    {
        mpsIperfToolConfig = (PS_IperfToolConfig)wParam;
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)lParam;
    }
    //! 60 * 60 * 24 * 30
    void CCJGW_IperfTestThread::RunIperfTest()
    {
        std::wstring strRead;
        if (E_SERVICE_TYPE == mpsIperfToolConfig->mnTerminalType) mpsIperfToolConfig->mstrTestTime = L"25920000";
        mpsIperfToolConfig->mnTestTimeIndex = 0;
        CCJGW_ProcessPipe* pProcessPipe = GetProcessCMDPipe();
        jgw_timer jt;
        
        //! 检查iperf测试时间
        for (int i = 0;i < 10 && mpsIperfToolConfig->mbStartTestStatus;i ++)
        {
            JGW::JGW_KillProcessToName(L"iperf3.exe");
            JGW::JGW_KillProcessToName(L"iperf.exe");  
            Sleep(1 * 1000);
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"Start Iperf Test ...");
            if (!pProcessPipe->send(mpsIperfToolConfig->mstrIperfCommand))
            {
                mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"Send Iperf Command Fail !!!");
                return ;
            }
            ReadIperfCMDProcessPipe(pProcessPipe,strRead,mpsIperfToolConfig->mnTestTime + 30);
            UpdateIperfLogInfo();    
            //! 说明已经正在测试否则
            if (jt.elapsed() >= mpsIperfToolConfig->mnTestTime)
            {
                mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"Iperf Test Complete...");
                break;
            }
            jt.restart();
        }
        JGW::JGW_KillProcessToName(L"iperf3.exe");
        JGW::JGW_KillProcessToName(L"iperf.exe");  
        //! Sleep(1 * 1000);
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"End Iperf Test ...");  
    }

    void CCJGW_IperfTestThread::BuildIperfLog()
    {
        if (mfIperfLog.is_open()) { mfIperfLog.flush();mfIperfLog.close();}
        std::wstring strIperfLogName;
        mfIperfLog.imbue(std::locale(""));
        JGW_FormatWString(strIperfLogName,L"%sTestLog\\THROUGHPUT\\%s\\%s\\throughput_data.csv",JGW_GetApplicationFolder(),JGW_GetTimeFolder().c_str(),mstrTimeStamp.c_str());
        JGW_CreateDirectory(strIperfLogName.c_str(),false);
        mfIperfLog.open(strIperfLogName);
        mfIperfLog << L"Channel" << L","; //! 信道
        mfIperfLog << L"tx_sector" << L",";
        mfIperfLog << L"rx_sector" << L",";
        mfIperfLog << L"attenuator(dbm)" << L","; //! 衰减器的值
        mfIperfLog << L"角度" << L",";
        mfIperfLog << L"Time(s)" << L",";
        mfIperfLog << L"速率（Mbits/sec)" << L",";
        mfIperfLog << L"RSSI" << std::endl;
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strIperfLogName.c_str());
    }

    void CCJGW_IperfTestThread::ExecAttenutorTest()
    {
        mnTrunTableRelMoveAngle = 0;
        mpsIperfToolConfig->mstrAttenuatordbm = L"";
        mpsIperfToolConfig->mstrTurntableAngles = L"";
        float fAttenutor = mpsIperfToolConfig->msAttenutorParam.mfAttenutorStart;

        //! 调整到起始角度
        if (mpsIperfToolConfig->mbEnableTrunTable)
        {
            RelMoveTrunTable(mpsIperfToolConfig->msTrunTableParam.mTurnTableAngleStart);
        }
#if 1
        do
        {
            //! 调整衰减器值
            if (mpsIperfToolConfig->mbEnableAttenutor)
            {
                SetAuttenuatorValue(fAttenutor);
                mpAsyncWndMessage->PutAsyncMessage(WM_HWND_CLEAR_BPS_INFO);
            } 
            fAttenutor += mpsIperfToolConfig->msAttenutorParam.mfAttenutorStep;
            int nTurnTableAngle = mpsIperfToolConfig->msTrunTableParam.mTurnTableAngleStart;

            do 
            {
                //! 调整转台角度
                if (mpsIperfToolConfig->mbEnableTrunTable)
                {
                    JGW_FormatWString(mpsIperfToolConfig->mstrTurntableAngles,L"%d",nTurnTableAngle);
                    if (nTurnTableAngle != mpsIperfToolConfig->msTrunTableParam.mTurnTableAngleStart)
                        RelMoveTrunTable(mpsIperfToolConfig->msTrunTableParam.mTurnTableStep);  
                }
                //! 
                RunIperfTest();
                nTurnTableAngle += mpsIperfToolConfig->msTrunTableParam.mTurnTableStep;
            } while (abs(nTurnTableAngle) <= abs(mpsIperfToolConfig->msTrunTableParam.mTurnTableAngleEnd) && mpsIperfToolConfig->mbStartTestStatus && mpsIperfToolConfig->mbEnableTrunTable);

            //! 调整到起始角度
            if (mpsIperfToolConfig->mbEnableTrunTable)
            {
                RelMoveTrunTable(-(mnTrunTableRelMoveAngle - mpsIperfToolConfig->msTrunTableParam.mTurnTableAngleStart));
            }
        } while (fAttenutor <= mpsIperfToolConfig->msAttenutorParam.mfAttenutorEnd && mpsIperfToolConfig->mbStartTestStatus && mpsIperfToolConfig->mbEnableAttenutor);
#else

        //! 衰减器
        for (float fAttenutor = mpsIperfToolConfig->msAttenutorParam.mfAttenutorStart;fAttenutor <= mpsIperfToolConfig->msAttenutorParam.mfAttenutorEnd && mpsIperfToolConfig->mbStartTestStatus;fAttenutor += mpsIperfToolConfig->msAttenutorParam.mfAttenutorStep)
        {
            //! 调整衰减器值
            if (mpsIperfToolConfig->mbEnableAttenutor)
            {
                SetAuttenuatorValue(fAttenutor);
                mpAsyncWndMessage->PutAsyncMessage(WM_HWND_CLEAR_BPS_INFO);
            } 

            //! 转台
            for (int nTurnTableAngle = mpsIperfToolConfig->msTrunTableParam.mTurnTableAngleStart;nTurnTableAngle <= mpsIperfToolConfig->msTrunTableParam.mTurnTableAngleEnd && mpsIperfToolConfig->mbStartTestStatus;
                nTurnTableAngle += mpsIperfToolConfig->msTrunTableParam.mTurnTableStep)
            {
                //! 调整转台角度
                if (mpsIperfToolConfig->mbEnableTrunTable)
                {
                    JGW_FormatWString(mpsIperfToolConfig->mstrTurntableAngles,L"%d",nTurnTableAngle);
                    if (nTurnTableAngle != mpsIperfToolConfig->msTrunTableParam.mTurnTableAngleStart)
                        RelMoveTrunTable(mpsIperfToolConfig->msTrunTableParam.mTurnTableStep);  
                }
                //! 
                RunIperfTest();
                if (!mpsIperfToolConfig->mbEnableTrunTable) break;
            }
            //! 调整到起始角度
            if (mpsIperfToolConfig->mbEnableTrunTable)
            {
                RelMoveTrunTable(-(mnTrunTableRelMoveAngle - mpsIperfToolConfig->msTrunTableParam.mTurnTableAngleStart));
            }
            if (!mpsIperfToolConfig->mbEnableAttenutor) break;
        }
#endif
        //! 归零
        if (mpsIperfToolConfig->mbEnableTrunTable)
        {
            RelMoveTrunTable(-mnTrunTableRelMoveAngle);
        }
    }
    //! 以当前角度为相对零度 测试完成后将角度调整到原值
    void CCJGW_IperfTestThread::RunIperfTestThread(WPARAM wParam,LPARAM lParam)
    {
        JGW_FormatWString(mstrTimeStamp,L"%u",JGW_GetTimeOfDay());
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"Start Iperf Test Thread ...");
        CCJGW_ProcessPipe* pProcessPipe = GetProcessCMDPipe();
        pProcessPipe->CloseProcessPipe();
        BuildIperfLog();

        if (mpsIperfToolConfig->mvTxSector.empty() && mpsIperfToolConfig->mvRxSector.empty())
        {
            if (mpsIperfToolConfig->mbReadOmniRSSI)
            {
                if (!ConnectDonglesComPort())
                {
                    mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"Warning Read RSSI Fail");
                }
            }
            
            mpsIperfToolConfig->mstrCurrentTxSector = L"";
            mpsIperfToolConfig->mstrCurrentRxSector = L"";
            //             SendTxSector(mpsIperfToolConfig->mstrCurrentTxSector);
            //             SendRxSector(mpsIperfToolConfig->mstrCurrentRxSector);
            ExecAttenutorTest();
        }
        else if (!mpsIperfToolConfig->mvTxSector.empty() && mpsIperfToolConfig->mvRxSector.empty())
        {
            if (!ConnectDonglesComPort()) goto END;
            mpsIperfToolConfig->mstrCurrentRxSector = L"";
            SendRxSector(mpsIperfToolConfig->mstrCurrentRxSector);
            for (std::vector<std::wstring>::iterator ittx = mpsIperfToolConfig->mvTxSector.begin();
                ittx != mpsIperfToolConfig->mvTxSector.end();
                ++ ittx)
            {
                mpsIperfToolConfig->mstrCurrentTxSector = ittx[0];
                SendTxSector(mpsIperfToolConfig->mstrCurrentTxSector);
                ExecAttenutorTest();
            }
        }
        else if (mpsIperfToolConfig->mvTxSector.empty() && !mpsIperfToolConfig->mvRxSector.empty())
        {
            if (!ConnectDonglesComPort()) goto END;
            mpsIperfToolConfig->mstrCurrentTxSector = L"";
            SendTxSector(mpsIperfToolConfig->mstrCurrentTxSector);
            for (std::vector<std::wstring>::iterator itrx = mpsIperfToolConfig->mvRxSector.begin();
                itrx != mpsIperfToolConfig->mvRxSector.end();
                ++ itrx)
            {
                mpsIperfToolConfig->mstrCurrentRxSector = itrx[0];
                SendRxSector(mpsIperfToolConfig->mstrCurrentRxSector);
                ExecAttenutorTest();
            }
        }
        else
        {
            if (!ConnectDonglesComPort()) goto END;
            for (std::vector<std::wstring>::iterator ittx = mpsIperfToolConfig->mvTxSector.begin();
                ittx != mpsIperfToolConfig->mvTxSector.end();
                ++ ittx)
            {
                mpsIperfToolConfig->mstrCurrentTxSector = ittx[0];
                SendTxSector(mpsIperfToolConfig->mstrCurrentTxSector);
                for (std::vector<std::wstring>::iterator itrx = mpsIperfToolConfig->mvRxSector.begin();
                    itrx != mpsIperfToolConfig->mvRxSector.end();
                    ++ itrx)
                {
                    mpsIperfToolConfig->mstrCurrentRxSector = itrx[0];
                    SendRxSector(mpsIperfToolConfig->mstrCurrentRxSector);
                    ExecAttenutorTest();
                }
            }
        }
END:
        mcDonglesComPort.CloseDonglesComPort();
        //! 关闭iperf log
        if (mfIperfLog.is_open()) { mfIperfLog.flush();mfIperfLog.close();}
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
    }

    bool CCJGW_IperfTestThread::HandIperfTestLog(const std::wstring& strIperfLog,std::wstring& strID,size_t& start,size_t& end,double& speed)
    {
        if (std::wstring::npos != strIperfLog.find(L"sec") && std::wstring::npos != strIperfLog.find(L"s/sec") && strIperfLog[0] == '[')
        {

            int id = 0;
            id = strIperfLog.find(L']',2) - 1;
            strID = strIperfLog.substr(1,id);
            start = _ttoi(strIperfLog.substr(id + 2).c_str());
            id = strIperfLog.find(L'-',id);
            end = _ttoi(strIperfLog.substr(id + 1).c_str());
            id = strIperfLog.find(L"s/sec",id + 1);
            id = strIperfLog.rfind(L's',id - 1);
            speed = _ttof (strIperfLog.substr(id + 1).c_str());
            return true;
        }
        return false;
    }


    void CCJGW_IperfTestThread::UpdateIperfLogInfo()
    {
        std::wifstream iFileStream;
        std::wstring strTemp,strID;
        size_t start = 0,end = 0;
        double speed = 0.00;
        bool bEnd = false;

        iFileStream.open(mpsIperfToolConfig->mstrSaveOutPutLog.c_str());
        if (!iFileStream.is_open()) return ;
        iFileStream.seekg(mpsIperfToolConfig->mnSeekgLog);

        while (iFileStream.getline(szReadBuf,READ_BUFFER_MAX_LEN))
        {
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,szReadBuf);
            mpsIperfToolConfig->mnSeekgLog = iFileStream.tellg();
            if (mpsIperfToolConfig->mbMultThread && !wcsstr(szReadBuf,L"[SUM]")) continue;
            if (!HandIperfTestLog(szReadBuf,strID,start,end,speed)) continue;

            if (start + 1 == end)
            {
                int rssi = -128;
                if (mcDonglesComPort.IsOpenDonglesComPort()) mcDonglesComPort.ReadOmniRssi(rssi);
                JGW_FormatWString(strTemp,L"%d,%s,%s,%s,%s,%d,%.2lf,%d",mpsIperfToolConfig->mnChannel,mpsIperfToolConfig->mstrCurrentTxSector.c_str(),mpsIperfToolConfig->mstrCurrentRxSector.c_str(),mpsIperfToolConfig->mstrAttenuatordbm.c_str(),mpsIperfToolConfig->mstrTurntableAngles.c_str(),end,speed,rssi);
                mfIperfLog << strTemp << std::endl;
                mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str());
                JGW_FormatWString(strTemp,L"%s,%d,%.2lf",mpsIperfToolConfig->mstrTurntableAngles.c_str(),end,speed);
                mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_UPDATE_BPS_INFO,strTemp.c_str());
            }
            else if (0 == start && end == mpsIperfToolConfig->mnTestTime)
            {
			    int rssi = -128;
                if (mcDonglesComPort.IsOpenDonglesComPort()) mcDonglesComPort.ReadOmniRssi(rssi);
                //JGW_FormatWString(strTemp,L"%d,%s,%s,%s,%s,%d,%.2lf,%d",mpsIperfToolConfig->mnChannel,mpsIperfToolConfig->mstrCurrentTxSector.c_str(),mpsIperfToolConfig->mstrCurrentRxSector.c_str(),mpsIperfToolConfig->mstrAttenuatordbm.c_str(),mpsIperfToolConfig->mstrTurntableAngles.c_str(),end,speed,rssi);
                JGW_FormatWString(strTemp,L"%d,%s,%s,%s,%s,%s,%.2lf,%d",mpsIperfToolConfig->mnChannel,mpsIperfToolConfig->mstrCurrentTxSector.c_str(),mpsIperfToolConfig->mstrCurrentRxSector.c_str(),mpsIperfToolConfig->mstrAttenuatordbm.c_str(),mpsIperfToolConfig->mstrTurntableAngles.c_str(),L"avg",speed,rssi);
                ///mfIperfLog << strTemp << std::endl;
                mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str());
                
                if (bEnd)
                {
                    mfIperfLog << strTemp << std::endl;
                    ///JGW_FormatWString(strTemp,L"%s,%.2lf",mpsIperfToolConfig->mstrTurntableAngles.empty()?L"0": mpsIperfToolConfig->mstrTurntableAngles.c_str(),speed);
                    ///mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_UPDATE_BPS_INFO,strTemp.c_str());
                    bEnd = false;
                }  
                else
                {
                    bEnd = true;
                }   
            }
        }
        
        iFileStream.close();
    }


    bool CCJGW_IperfTestThread::ReadIperfCMDProcessPipe(CCJGW_ProcessPipe* pProcessPipe,std::wstring& strRead,size_t nTimeOutSec /* = 10 */)
    {
        if (!pProcessPipe) return false;
        strRead = L"";
        int readSize = 0;
        jgw_timer tm;
        std::wstring strTemp;

        while(tm.elapsed() < nTimeOutSec && mpsIperfToolConfig->mbStartTestStatus)
        {
            readSize = pProcessPipe->recv(strTemp);
            if( -1 == readSize )            {
                PELOG4WW(_T("error: process closed!!!\n"));
                return false;
            }
            else if ( readSize > 0 )
            {
                //trim::ctrim(strTemp);  //! JGW_ReplaceStringW(strTemp,_T("\r"),_T("\t")); //! JGW_ReplaceStringW(strTemp,_T("\n"),_T("\t"));
                if( strTemp.length() <= 0 ) continue;
                strRead = strTemp;
                //! mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str());
                //! PDLOG4WW_F(_T("%s"),strTemp.c_str());
                if (strRead.length() > 1 && (strRead.at(strRead.length() - 1) == _T('>') || strRead.at(strRead.length() - 1) == _T('$') || strRead.at(strRead.length() - 1) == _T('#')))
                {
                    return true;
                }
            }
            else
            {
                UpdateIperfLogInfo();
                Sleep(200);
            }
        }
        return false;
    }

    CCJGW_ProcessPipe* CCJGW_IperfTestThread::GetProcessCMDPipe()
    {
        static CCJGW_ProcessPipe gProcessPipe;
        if (gProcessPipe.CheckProcessIsRun()) return &gProcessPipe;

        std::wstring strApplicationName,strApplicationFilePath;
        /*strApplicationName = GetGlobalEnvironment()->GetString(L"ApplicationCMD");*/
        if (strApplicationName.empty()) strApplicationName = L"cmd.exe";
        strApplicationFilePath = JGW_RealativePathToAbsPath(strApplicationName.c_str());

        if (!gProcessPipe.InitProcessPipe(strApplicationFilePath.c_str(),L"",JGW_GetApplicationFolder())) { PELOG4WW(L"初始化CMD进程失败\n"); return NULL;}
        ReadIperfCMDProcessPipe(&gProcessPipe,strApplicationName);

        return &gProcessPipe;
    }

    bool CCJGW_IperfTestThread::ConnectDonglesComPort()
    {
        mcDonglesComPort.CloseDonglesComPort();
        std::wstring strComPort;
        for (int i = 0;i < 50;i ++)
        {
            strComPort = JGW_GetComPortToUsbServiceName(L"usbser",L"VID_2932");
            if (strComPort.empty()) { Sleep(1000); continue; }
            if (mcDonglesComPort.OpenDonglesComPort(strComPort)) return true;
            Sleep(200);
        }
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"Warning Connect Dongle Com Port Fail !!!");
        return false;
    }

    bool CCJGW_IperfTestThread::SendTxSector(const std::wstring& strSector)
    {
        std::string strRead,strTemp;
        bool bResult = false;

        if (strSector.empty())
        {
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"Peraso Send(Tx) : mib s 12-3-1-40 0");
            bResult = mcDonglesComPort.ReadRawWriteCommands("mib s 12-3-1-40 0\n",strRead);
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_GetFormatWString(L"Peraso Recv(Tx) : %s",JGW_A2W(strRead).c_str()).c_str());
        }
        else
        {
            JGW_FormatString(strTemp,"mib s 12-3-1-41 %s00",JGW_W2A(strSector).c_str());

            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"Peraso Send(Tx) : mib s 12-3-1-40 1");
            bResult = mcDonglesComPort.ReadRawWriteCommands("mib s 12-3-1-40 1\n",strRead);
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_GetFormatWString(L"Peraso Recv(Tx) : %s",JGW_A2W(strRead).c_str()).c_str());

            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_GetFormatWString(L"Peraso Send(Tx) : %s",JGW_A2W(strTemp).c_str()).c_str());
            bResult &= mcDonglesComPort.ReadRawWriteCommands(strTemp.c_str(),strRead);
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_GetFormatWString(L"Peraso Recv(Tx) : %s",JGW_A2W(strRead).c_str()).c_str());
        }

        return bResult;
    }

    bool CCJGW_IperfTestThread::SendRxSector(const std::wstring& strSector)
    {
        bool bResult = false;
        std::string strRead,strTemp;

        if (strSector.empty())
        {
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"Peraso Send(Rx) : mib s 12-2-1-40 0");
            bResult = mcDonglesComPort.ReadRawWriteCommands("mib s 12-2-1-40 0\n",strRead);
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_GetFormatWString(L"Peraso Recv(Rx) : %s",JGW_A2W(strRead).c_str()).c_str());
        }
        else
        {
            JGW_FormatString(strTemp,"mib s 12-2-1-41 %s00",JGW_W2A(strSector).c_str());

            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"Peraso Send(Rx) : mib s 12-2-1-40 1");
            bResult = mcDonglesComPort.ReadRawWriteCommands("mib s 12-2-1-40 1\n",strRead);
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_GetFormatWString(L"Peraso Recv(Rx) : %s",JGW_A2W(strRead).c_str()).c_str());

            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_GetFormatWString(L"Peraso Send(Rx) : %s",JGW_A2W(strTemp).c_str()).c_str());
            bResult &= mcDonglesComPort.ReadRawWriteCommands(strTemp.c_str(),strRead);
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_GetFormatWString(L"Peraso Recv(Rx) : %s",JGW_A2W(strRead).c_str()).c_str());

            //JGW_FormatString(strTemp,"mib s 12-2-1-41 %s00",JGW_W2A(strSector).c_str());
            //return mcDonglesComPort.ReadRawWriteCommands("mib s 12-2-1-40 1\n",strRead) && mcDonglesComPort.ReadRawWriteCommands(strTemp.c_str(),strRead);
        }
        return bResult;
    }

    void CCJGW_IperfTestThread::RelMoveTrunTable(int nTurnTableAngle)
    {
        std::wstring strTemp;

        mnTrunTableRelMoveAngle += nTurnTableAngle;
        GetTrunTableSCLLibHelper().RelMove(TURNTABLE_NODE_ID,nTurnTableAngle * PER_ANGLE_TURN, 
            &mpsIperfToolConfig->msTrunTableParam.mfVelocity, 
            &mpsIperfToolConfig->msTrunTableParam.mfAcceleration,
            &mpsIperfToolConfig->msTrunTableParam.mfDeceleration);

        JGW_FormatWString(strTemp,L"TrunTable RelMove : %d",nTurnTableAngle);
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str());

        Sleep(mpsIperfToolConfig->msTrunTableParam.mTurnTableAdjTime);
    }

    void CCJGW_IperfTestThread::AbsMoveTrunTable(int nTurnTableAngle)
    {
        std::wstring strTemp;

        JGW_FormatWString(mpsIperfToolConfig->mstrTurntableAngles,L"%d",nTurnTableAngle);
        mnTrunTableRelMoveAngle += nTurnTableAngle;
        GetTrunTableSCLLibHelper().AbsMove(TURNTABLE_NODE_ID,nTurnTableAngle * PER_ANGLE_TURN, 
            &mpsIperfToolConfig->msTrunTableParam.mfVelocity, 
            &mpsIperfToolConfig->msTrunTableParam.mfAcceleration,
            &mpsIperfToolConfig->msTrunTableParam.mfDeceleration);

        JGW_FormatWString(strTemp,L"TrunTable AbsMove : %d",nTurnTableAngle);
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str());

        Sleep(mpsIperfToolConfig->msTrunTableParam.mTurnTableAdjTime);
    }

    void CCJGW_IperfTestThread::SetAuttenuatorValue(float fAttenutorVal)
    {
        std::wstring strTemp;
        JGW_FormatWString(mpsIperfToolConfig->mstrAttenuatordbm,L"%.2f",fAttenutorVal);
        GetAttenuatorEquipment().SetAuttenuatorValue(fAttenutorVal);

        JGW_FormatWString(strTemp,L"Attenutor Value : %s",mpsIperfToolConfig->mstrAttenuatordbm.c_str());
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str());

        Sleep(mpsIperfToolConfig->msAttenutorParam.mAttenutorAdjTime);
    }
}