#include "StdAfx.h"
#include "CJGW_StationHandleRequestThread.h"
#include "json\CJGW_RepStatusJson.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/CUtf8String.hpp>
#include "CJGW_StationServiceThread.h"
#include <ShellAPI.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <poco/Net/FilePartSource.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/URI.h>
#include <string>
#include <sstream>

namespace JGW
{
    CCJGW_StationHandleRequestThread::CCJGW_StationHandleRequestThread(void) : mpsHandleReqThreadParam(NULL),mstrTestLogFilePath("C://01_19_08_192.168.133.65_to_192.168.133.36.log"),mnStatusTime(0)
    {
//         mvFullCounterPath.push_back(L"\\Processor Information(_Total)\\% Processor Utility");
//         mvFullCounterPath.push_back(L"\\Processor Information(0,0)\\% Processor Utility");
//         mvFullCounterPath.push_back(L"\\Processor Information(0,1)\\% Processor Utility");
//         mvFullCounterPath.push_back(L"\\Processor Information(0,2)\\% Processor Utility");
//         mvFullCounterPath.push_back(L"\\Processor Information(0,3)\\% Processor Utility");
        mvFullCounterPath.push_back(L"\\Processor Information(_Total)\\% Processor Time");
        mvFullCounterPath.push_back(L"\\Processor Information(0,0)\\% Processor Time");
        mvFullCounterPath.push_back(L"\\Processor Information(0,1)\\% Processor Time");
        mvFullCounterPath.push_back(L"\\Processor Information(0,2)\\% Processor Time");
        mvFullCounterPath.push_back(L"\\Processor Information(0,3)\\% Processor Time");
        mpCPUMemDiskStatusImp = GetCPUMemDiskStatusImp();
    }


    CCJGW_StationHandleRequestThread::~CCJGW_StationHandleRequestThread(void)
    {
        ReleaseCPUMemDiskStatusImp(mpCPUMemDiskStatusImp);
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_StationHandleRequestThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCJGW_StationHandleRequestThread::OnStartHandleReq)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_StationHandleRequestThread::OnStartHandleReq(WPARAM wParam,LPARAM lParam)
    {
        mpsHandleReqThreadParam = (PS_STATION_HANDLE_REQ_THREAD_PARAM)wParam;
        mpsStationMonitorUiInfo = (PS_STATION_MONITOR_UI_INFO)lParam;
        OnHandleStationSocketCmd(mpsHandleReqThreadParam->mstrCmd);
    }

    std::string CCJGW_StationHandleRequestThread::BuildPingLogFilePath()
    {
        std::string strPingLogFilePath;
        JGW_FormatString(strPingLogFilePath,"%sping_%s.log",JGW_W2A_W(JGW_GetApplicationFolder()).c_str(),JGW_W2A(JGW_GetFileNameToCurrentTimeStamp()).c_str());
        return strPingLogFilePath;
    }

    std::string CCJGW_StationHandleRequestThread::BuildStationLogFilePath()
    {
        std::string strStationLogFilePath;
        CCJGW_CriticalSectionAutoLock lock(mpsStationMonitorUiInfo->mIPAddressCriticalSectionLock);
        JGW_FormatString(strStationLogFilePath,"%s%s_%s_to_%s.log",JGW_W2A_W(JGW_GetApplicationFolder()).c_str(),JGW_W2A(JGW_GetFileNameToCurrentTimeStamp()).c_str(),JGW_W2A(mpsStationMonitorUiInfo->mstrIPAddress).c_str(),mpsHandleReqThreadParam->mstrSocketAddres.c_str());
        return strStationLogFilePath;
    }

    std::string CCJGW_StationHandleRequestThread::BuildStatusLogFilePath()
    {
        std::string strStatusLogFilePath;
        JGW_FormatString(strStatusLogFilePath,"%sstatus_%s.log",JGW_W2A_W(JGW_GetApplicationFolder()).c_str(),JGW_W2A(JGW_GetFileNameToCurrentTimeStamp()).c_str());
        return strStatusLogFilePath;
    }

    void CCJGW_StationHandleRequestThread::WritePingLogFileToPingLog(const std::string& strPingLog)
    {
        std::ofstream fileStream;
        fileStream.imbue(std::locale(""));
        fileStream.open(mstrPingLogFilePath.c_str(),std::ios_base::app);
        if (!fileStream.is_open()) return ;
        fileStream << strPingLog << "\n";
        fileStream.close();
    }

    void CCJGW_StationHandleRequestThread::WriteStatusLogFileToStatusLog(const std::string& strStatusLog)
    {
        std::ofstream fileStream;
        fileStream.imbue(std::locale(""));
        fileStream.open(mstrStatusLogFilePath.c_str(),std::ios_base::app);
        if (!fileStream.is_open()) return ;
        fileStream << strStatusLog;
        fileStream.close();
    }

    void CCJGW_StationHandleRequestThread::OnHandleStationSocketCmd(const std::string& strCmd)
    {
        mstrPingLogFilePath = BuildPingLogFilePath();
        mstrStatusLogFilePath = BuildStatusLogFilePath();
        mvIperfLogInfo.clear();
        mnStatusTime = 1;
        //! WriteStatusLogFileToStatusLog("time rssi txMcs rxMcs txSec rxSec txDropRate rxDropRate cpuAll   cpu0   cpu1   cpu2   cpu3  memTotal memUsed memFree  txSpeed         rxSpeed \n");
        WriteStatusLogFileToStatusLog("time rssi txMcs rxMcs txSec rxSec txDropRate rxDropRate cpuTemp radioTemp cpuFreq cpuAll   cpu0   cpu1   cpu2   cpu3  memTotal memUsed memFree  txSpeed         rxSpeed \n");
        mICMPSocket.ResetICMPSeq();
        mpCPUMemDiskStatusImp->SystemCpuInit(mvFullCounterPath);
        //! iperf 命令
        if (JGW_StrCaseStr(strCmd.c_str(),"iperf"))
        {
            JGW_KillProcessToName(L"iperf3.exe");
            JGW_KillProcessToName(L"iperf.exe");
            //! 客户端
            if (JGW_StrCaseStr(strCmd.c_str(),"-c"))
            {
                Sleep(1000);
                OnHandleIperfClientRequest(strCmd);
            }
            //! 服务端
            else if (JGW_StrCaseStr(strCmd.c_str(),"-s"))
            {
                //! Sleep(1000);
                OnHandleIperfServiceRequest(strCmd);
            }
        }
        //！ ping 命令
        else if (JGW_StrCaseStr(strCmd.c_str(),"ping"))
        {
            OnHandlePingRequest(strCmd);
        }
        OnReportLogFileJsonInfo();
        mpCPUMemDiskStatusImp->SystemCpuUnInit();
        mpsHandleReqThreadParam->mbThreadIsWorking = false;
    }


    void CCJGW_StationHandleRequestThread::OnHandlePingRequest(const std::string& strCmd)
    {
#if 0
        m_pCMDPipe = GetProcessCMDPipe();
        if (!m_pCMDPipe) return ;
        std::string strBuildPingCmd = "hrping -n 1",strJson;
        CCJGW_StationServiceThread* pStationServiceThread = (CCJGW_StationServiceThread*)mpsHandleReqThreadParam->mpMessageThread;
        float fLatency = -1.00f;
        std::wstring strRead;
        strBuildPingCmd += strCmd.substr(strCmd.find(' '));

        while (!mpsHandleReqThreadParam->mbStopReqThreadWork)
        {
            fLatency = -1.00f;
            m_pCMDPipe->send(strBuildPingCmd);
            ReadCMDProcessPipeEOF(m_pCMDPipe,strRead,10);

            if (std::wstring::npos != strRead.find(L"time="))
            {
                const wchar_t* p = _tcsstr(const_cast<wchar_t*>(strRead.c_str()),L"time=");
                p += wcslen(L"time=");
                fLatency = static_cast<float>(_ttof(p));
            }
            strJson = BuildRspPingJson(fLatency);
            pStationServiceThread->SendJsonCommand(strJson,mpsHandleReqThreadParam->mClientSocket);

            {
                CCJGW_CriticalSectionAutoLock lock(mpsStationMonitorUiInfo->mStationStatusJsonCriticalSectionLock);
                pStationServiceThread->SendJsonCommand(mpsStationMonitorUiInfo->mstrStationStatusJson,mpsHandleReqThreadParam->mClientSocket);
            }
            Sleep(900);
        }
#else
        std::vector<std::string> vstrTemp;
        std::string strHostAddr,strJson;
        JGW_ParserStrA(strCmd.c_str()," ",vstrTemp);
        int a = -1,b = -1,c = -1,d = -1;
        char s[200] = {0};

        for (size_t i = 0;i < vstrTemp.size();i ++)
        {
            if (std::string::npos == vstrTemp[i].find('.')) continue;
            sscanf(vstrTemp[i].c_str(),"%d.%d.%d.%d%s",&a,&b,&c,&d,s);
            if(a > 255 || a < 0 || b > 255 || b < 0 || c > 255 || c < 0 || d > 255 || d < 0) continue;
            if(s[0]!=0) continue;
            strHostAddr = vstrTemp[i];
        }
        if (strHostAddr.empty()) return;

        while (!mpsHandleReqThreadParam->mbStopReqThreadWork)
        {
            OnReportPingJsonInfo(strHostAddr);
            OnReportSystemJsonInfo();
            OnReportStatusJsonInfo();
            Sleep(900);
        }
#endif
    }

    void CCJGW_StationHandleRequestThread::OnReportStatusJsonInfo()
    {
        CCJGW_StationServiceThread* pStationServiceThread = (CCJGW_StationServiceThread*)mpsHandleReqThreadParam->mpMessageThread;

        std::string strJson,strStatusLog;
        {
            CCJGW_CriticalSectionAutoLock lock(mpsStationMonitorUiInfo->mStationStatusJsonCriticalSectionLock);
            strJson = mpsStationMonitorUiInfo->mstrStationStatusJson;
        }
        {
            CCJGW_CriticalSectionAutoLock lock(mpsStationMonitorUiInfo->mTrafficSpeedCriticalSectionLock);
            CCJGW_RepStatusJson reqStatusJson;
            reqStatusJson.FromJsonToString(strJson);
            reqStatusJson.tx_speed = mpsStationMonitorUiInfo->mstrUploadSpeed;
            reqStatusJson.rx_speed = mpsStationMonitorUiInfo->mstrDownloadSpeed;
            strJson = reqStatusJson.BuildStatusJson();
            //! mnStatusTime
            //! time rssi txMcs rxMcs txSec rxSec txDropRate rxDropRate cpuAll   cpu0   cpu1   cpu2   cpu3  memTotal memUsed memFree  txSpeed         rxSpeed
            //! time rssi txMcs rxMcs txSec rxSec txDropRate rxDropRate cpuTemp radioTemp cpuFreq cpuAll   cpu0   cpu1   cpu2   cpu3  memTotal memUsed memFree  txSpeed         rxSpeed
            JGW_FormatString(strStatusLog,"%4d %4d %5d %5d %5d %5d %10.2f %10.2f %5d %5d %5d %6.2f  %5.1f  %5.1f  %5.1f  %5.1f  %8I64u %7I64u %7I64u  %s %s\n",mnStatusTime++,reqStatusJson.rssi,reqStatusJson.tx_mcs,reqStatusJson.rx_mcs,reqStatusJson.tx_sector,reqStatusJson.rx_sector,reqStatusJson.tx_drop_rate,reqStatusJson.rx_drop_rate,mReqSystemInfoJson.cpu_temp,mReqSystemInfoJson.radio_temp,mReqSystemInfoJson.cpu_freq,mReqSystemInfoJson.cpuAll,mReqSystemInfoJson.cpu0,mReqSystemInfoJson.cpu1,mReqSystemInfoJson.cpu2,mReqSystemInfoJson.cpu3,mReqSystemInfoJson.mem_total,mReqSystemInfoJson.mem_used,mReqSystemInfoJson.mem_free,reqStatusJson.tx_speed.c_str(),reqStatusJson.rx_speed.c_str());
            WriteStatusLogFileToStatusLog(strStatusLog);
        }
        mpsHandleReqThreadParam->mbStopReqThreadWork = !pStationServiceThread->SendJsonCommand(strJson,mpsHandleReqThreadParam->mClientSocket);
    }

    void CCJGW_StationHandleRequestThread::OnReportPingJsonInfo(const std::string& strIPAddress)
    {
        float fLatency = -1.00f;
        CCJGW_StationServiceThread* pStationServiceThread = (CCJGW_StationServiceThread*)mpsHandleReqThreadParam->mpMessageThread;
        if (mICMPSocket.Ping(strIPAddress,mpsStationMonitorUiInfo->mstrIPAddress.empty()?NULL:JGW_W2A(mpsStationMonitorUiInfo->mstrIPAddress).c_str())) fLatency = static_cast<float>(mICMPSocket.GetPingTimeMs());
        WritePingLogFileToPingLog(mICMPSocket.GetPingResult());
        mpsHandleReqThreadParam->mbStopReqThreadWork = !pStationServiceThread->SendJsonCommand(BuildRspPingJson(fLatency),mpsHandleReqThreadParam->mClientSocket);
    }

    std::string CCJGW_StationHandleRequestThread::BuildRspPingJson(float fLatency)
    {
        std::string strJson;
        CCJGW_CriticalSectionAutoLock lock(mpsStationMonitorUiInfo->mMacAddressCriticalSectionLock);
        JGW_FormatString(strJson,"{\"requestCode\":1005,\"mac\":\"%s\",\"latency\":\"%.2f\"}",mpsStationMonitorUiInfo->mstrAnsiMacAddress.c_str(),fLatency);
        return strJson;
    }

    void CCJGW_StationHandleRequestThread::OnReportSystemJsonInfo()
    {
        CCJGW_StationServiceThread* pStationServiceThread = (CCJGW_StationServiceThread*)mpsHandleReqThreadParam->mpMessageThread;
        {
            CCJGW_CriticalSectionAutoLock lock(mpsStationMonitorUiInfo->mMacAddressCriticalSectionLock);
            mReqSystemInfoJson.mac = mpsStationMonitorUiInfo->mstrAnsiMacAddress;
        }
        
        if (mpCPUMemDiskStatusImp->GetSystemCpuCurrentUsage(mvDoubleCPUUsed))
        {
            mReqSystemInfoJson.cpuAll = mvDoubleCPUUsed[0];
            mReqSystemInfoJson.cpu0 = mvDoubleCPUUsed[1];
            mReqSystemInfoJson.cpu1 = mvDoubleCPUUsed[2];
            mReqSystemInfoJson.cpu2 = mvDoubleCPUUsed[3];
            mReqSystemInfoJson.cpu3 = mvDoubleCPUUsed[4];
        }
        
        mpCPUMemDiskStatusImp->GetPhysicalMemoryState(mReqSystemInfoJson.mem_total,mReqSystemInfoJson.mem_used);
        mReqSystemInfoJson.mem_total /= 1024;
        mReqSystemInfoJson.mem_used /= 1024;
        mReqSystemInfoJson.mem_free = mReqSystemInfoJson.mem_total - mReqSystemInfoJson.mem_used;
        mpsHandleReqThreadParam->mbStopReqThreadWork = !pStationServiceThread->SendJsonCommand(mReqSystemInfoJson.BuildSystemJson(),mpsHandleReqThreadParam->mClientSocket);
    }

    void CCJGW_StationHandleRequestThread::OnStartIperfPrcessToCmd(const std::string& strCmd,bool bIperfServerCmd /* = false */)
    {
        char szCmd[260] = {0};
        char* pBuf = NULL,*pSocketPort = NULL;
        strcpy_s(szCmd,260,strCmd.c_str());
        std::vector<std::string> vstrIperfCmd;
        std::string strIperfCmd,strIperfBaseCmd,strTemp;
        S_IPERF_LOG_INFO sIperfLogInfo;
        std::string strApplicationName = strtok(szCmd," ");

        strcpy_s(szCmd,260,strCmd.c_str());
        pBuf = strstr(szCmd,"-p");
        mvIperfLogInfo.clear();
        //! 如果未找到-p 参数则默认启动单个iperf
        if (NULL == pBuf || NULL == strstr(szCmd,":"))
        {
            sIperfLogInfo.mstrIperfLogFilePath = JGW_RealativePathToAbsPathA("iperf.log");
            JGW_FormatString(strIperfCmd,"%s --logfile \"%s\"",strCmd.c_str(),sIperfLogInfo.mstrIperfLogFilePath.c_str());
            if (bIperfServerCmd) strIperfCmd += " -1";
            JGW_DeleteFile(JGW_A2W(sIperfLogInfo.mstrIperfLogFilePath));
            mvIperfLogInfo.push_back(sIperfLogInfo);
            vstrIperfCmd.push_back(strIperfCmd);
        }//! 找到-P参数则解析
        else
        {
            strIperfBaseCmd = strCmd.substr(0,pBuf - szCmd);
            int nStartSocketPort = atoi(pBuf + 2),nSocketPortCount = 0;
            pSocketPort = strstr(pBuf + 2,":");
            nSocketPortCount = atoi(pSocketPort + 1);
            for (int i = 0;i < nSocketPortCount;i ++)
            {
                JGW_FormatString(strTemp,"iperf_%d.log",nStartSocketPort + i);
                sIperfLogInfo.mstrIperfLogFilePath = JGW_RealativePathToAbsPathA(strTemp);
                JGW_FormatString(strIperfCmd,"%s -p %d --logfile \"%s\"",strIperfBaseCmd.c_str(),nStartSocketPort + i,sIperfLogInfo.mstrIperfLogFilePath.c_str());
                if (bIperfServerCmd) strIperfCmd += " -1";
                JGW_DeleteFile(JGW_A2W(sIperfLogInfo.mstrIperfLogFilePath));
                mvIperfLogInfo.push_back(sIperfLogInfo);
                vstrIperfCmd.push_back(strIperfCmd);
            }
        }

        std::string strFolder(JGW_W2A_W(JGW_GetApplicationFolder()));
        strFolder += "tool\\";
        //! 开启请求service线程
        for (size_t i = 0;i < vstrIperfCmd.size();i ++)
        {
            ShellExecuteA(NULL,"open",strApplicationName.c_str(),vstrIperfCmd[i].c_str(),strFolder.c_str(),SW_HIDE);
        }
    }

    void CCJGW_StationHandleRequestThread::OnHandleIperfServiceRequest(const std::string& strCmd)
    {
        int nIndex = 3;
        OnStartIperfPrcessToCmd(strCmd,true);
        while (!mpsHandleReqThreadParam->mbStopReqThreadWork && nIndex > 0)
        {
            OnReportSystemJsonInfo();
            OnReportStatusJsonInfo();
            Sleep(950);
            if (!CheckIperfProcesssIsRun()) nIndex --;
        }
    }

    void CCJGW_StationHandleRequestThread::OnHandleIperfClientRequest(const std::string& strCmd)
    {
        OnStartIperfPrcessToCmd(strCmd);
        std::string strHost;
        size_t pos = strCmd.find("-c");
        int nIndex = 3;
        for (pos += 2;pos < strCmd.length() && ' ' == strCmd[pos];pos ++) ;
        for (;pos < strCmd.length() && ' ' != strCmd[pos];pos ++) strHost += strCmd[pos];

        while (!mpsHandleReqThreadParam->mbStopReqThreadWork && nIndex > 0)
        {
            OnReportSystemJsonInfo();
            OnReportStatusJsonInfo();
            OnReportPingJsonInfo(strHost);   
            Sleep(950);
            if (!CheckIperfProcesssIsRun()) nIndex --;
        }
    }

    void CCJGW_StationHandleRequestThread::OnReportLogFileJsonInfo()
    {
        CCJGW_StationServiceThread* pStationServiceThread = (CCJGW_StationServiceThread*)mpsHandleReqThreadParam->mpMessageThread;

        std::string strJson;
        mstrTestLogFilePath = BuildStationLogFilePath();
        BuildStationLogFile();
        bool bResult = UploadLogFileToSocket(mpsHandleReqThreadParam->mstrSocketAddres,mstrTestLogFilePath);
        {
             CCJGW_CriticalSectionAutoLock lock(mpsStationMonitorUiInfo->mMacAddressCriticalSectionLock);
             JGW_FormatString(strJson,"{\"requestCode\":1007,\"mac\":\"%s\",\"status\":%d,\"fileName\":\"%s\"}",mpsStationMonitorUiInfo->mstrAnsiMacAddress.c_str(),bResult?1:-1,bResult?JGW_GetFileNameToFilePathA(mstrTestLogFilePath.c_str()).c_str():"");
        }
        pStationServiceThread->SendJsonCommand(strJson,mpsHandleReqThreadParam->mClientSocket);
    }

    void CCJGW_StationHandleRequestThread::WriteStationStreamLogToFile(std::ofstream& stationStream,const std::string& strFilePath)
    {
        char szReadBuf[1024];
        std::ifstream fStream;
        fStream.imbue(std::locale(""));
        fStream.open(strFilePath);
        if (fStream.is_open())
        {
            while (fStream.getline(szReadBuf,1024))
            {
                stationStream << szReadBuf << "\n";
            }
        }
    }

    void CCJGW_StationHandleRequestThread::BuildStationLogFile()
    {
        std::ofstream fileStream;
        fileStream.imbue(std::locale(""));
        fileStream.open(mstrTestLogFilePath.c_str(),std::ios_base::ate);
        if (!fileStream.is_open()) return ;

        fileStream << "########################## iperf result #################################\n" ;
        for (size_t i = 0;i < mvIperfLogInfo.size();i ++)
        {
            WriteStationStreamLogToFile(fileStream,mvIperfLogInfo[i].mstrIperfLogFilePath);
            JGW_DeleteFile(JGW_A2W(mvIperfLogInfo[i].mstrIperfLogFilePath));
        }

        fileStream << "\n\n########################## ping result #################################\n";
        WriteStationStreamLogToFile(fileStream,mstrPingLogFilePath);
        JGW_DeleteFile(JGW_A2W(mstrPingLogFilePath));

        fileStream << "\n\n########################## cpu/mem result #################################\n";
        WriteStationStreamLogToFile(fileStream,mstrStatusLogFilePath);
        JGW_DeleteFile(JGW_A2W(mstrStatusLogFilePath));

        fileStream.close();
    }

    bool CCJGW_StationHandleRequestThread::UploadLogFileToSocket(const std::string& strSocketAddress,const std::string& strLogFilePath)
    {
        std::string strUrl,strRead,strSourceAddr = JGW_W2A(mpsStationMonitorUiInfo->mstrIPAddress.c_str());
        JGW_FormatString(strUrl,"http://%s/cgi-bin/upload.cgi",strSocketAddress.c_str());

        try
        {
            Poco::URI url(strUrl);
            Poco::Net::HTTPClientSession session(url.getHost(),url.getPort());
            Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST,url.getPath(),Poco::Net::HTTPRequest::HTTP_1_1);

            request.setTransferEncoding(Poco::Net::HTTPMessage::IDENTITY_TRANSFER_ENCODING);
            request.set("upload-type", "100");

            Poco::Net::HTMLForm form;
            form.setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
            form.addPart("sendfile", new Poco::Net::FilePartSource(std::string(U(strLogFilePath.c_str()))));
            form.prepareSubmit(request);
            request.setTransferEncoding(Poco::Net::HTTPMessage::IDENTITY_TRANSFER_ENCODING);

            std::ostringstream ostr;
            form.write(ostr);
            std::string s = ostr.str();

            request.setContentLength(s.length());
            if (!strSourceAddr.empty())
            {
                sockaddr_in sin;
                sin.sin_family = AF_INET;
                sin.sin_port = 0;
                sin.sin_addr.S_un.S_addr = inet_addr(strSourceAddr.c_str());
                Poco::Net::SocketAddress sourceAddress((LPSOCKADDR)&sin, sizeof(sin));
                session.socket().impl()->bind(sourceAddress);
                session.setKeepAlive(true);
                Poco::Timespan time(60*60,0);
                session.setKeepAliveTimeout(time);

                Poco::Net::SocketAddress addr(url.getHost(),url.getPort());
                session.socket().connect(addr,60000000);
                session.socket().setReceiveTimeout(60000000);
                session.socket().setNoDelay(true);
            }
            session.sendRequest(request) << s;

            Poco::Net::HTTPResponse resp;
            std::istream & is = session.receiveResponse(resp);
            strRead = gulp(is);
            Log4AI_F("UploadLogFileToSocket Result : %s",strRead.c_str());

            return NULL == strRead.compare("{\"responseCode\":200,\"resultCode\":1,\"errorCode\":0}");
        }
        catch(Poco::Net::NetException & ex)
        {
            Log4AE_F("Post HTTP Fail(%s)",ex.displayText().c_str()); 
        }
        catch(Poco::Exception& ex)
        {
            Log4AE_F("Post HTTP Fail(%s)",ex.displayText().c_str()); 
        }
        return false;
    }

    bool CCJGW_StationHandleRequestThread::CheckIperfProcesssIsRun()
    {
        if (-1 != JGW_GetProcessID2Name(_T("iperf.exe"))) return true;
        if (-1 != JGW_GetProcessID2Name(_T("iperf3.exe"))) return true;
        return false;
    }

    //     bool CCJGW_StationHandleRequestThread::ReadCMDProcessPipeEOF(CCJGW_ProcessPipe* pProcessPipe,std::wstring& strRead,size_t nTimeOutSec /* = 10 */)
    //     {
    //         if (!pProcessPipe) return false;
    //         strRead = L"";
    //         int readSize = 0;
    //         jgw_timer tm;
    //         std::wstring strTemp;
    // 
    //         while(tm.elapsed() < nTimeOutSec)
    //         {
    //             readSize = pProcessPipe->recv(strTemp);
    //             if( -1 == readSize )
    //             {
    //                 //! LogE(_T("error: process closed!!!"));
    //                 return false;
    //             }
    //             else if ( readSize > 0 )
    //             {
    //                 //trim::ctrim(strTemp);  //! JGW_ReplaceStringW(strTemp,_T("\r"),_T("\t")); //! JGW_ReplaceStringW(strTemp,_T("\n"),_T("\t"));
    //                 if( strTemp.length() <= 0 ) continue;
    //                 strRead += strTemp;
    //                 //! LogI_F(_T("%s"),strTemp.c_str());
    //                 if (strRead.length() > 1 && (strRead.at(strRead.length() - 1) == _T('>') || strRead.at(strRead.length() - 1) == _T('$') || strRead.at(strRead.length() - 1) == _T('#')))
    //                 {
    //                     return true;
    //                 }
    //             }
    //         }
    //         pProcessPipe->CloseProcessPipe();
    //         GetProcessCMDPipe();
    //         return false;
    //     }
    // 
    //     CCJGW_ProcessPipe* CCJGW_StationHandleRequestThread::GetProcessCMDPipe()
    //     {
    //         static CCJGW_ProcessPipe gProcessPipe;
    //         if (gProcessPipe.CheckProcessIsRun()) return &gProcessPipe;
    // 
    //         std::wstring strApplicationName = L"cmd.exe",strApplicationFilePath;
    //         strApplicationFilePath = JGW_RealativePathToAbsPath(strApplicationName.c_str());
    // 
    //         if (!gProcessPipe.InitProcessPipe(strApplicationFilePath.c_str(),L"",JGW_GetApplicationFolder())) { Log4WE(L"初始化CMD进程失败\n"); return NULL;}
    //         ReadCMDProcessPipeEOF(&gProcessPipe,strApplicationName);
    // 
    //         return &gProcessPipe;
    //     }
}
