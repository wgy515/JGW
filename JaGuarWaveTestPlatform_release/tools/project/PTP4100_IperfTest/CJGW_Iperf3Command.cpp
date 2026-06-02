#include "StdAfx.h"
#include "CJGW_Iperf3Command.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

#include <fstream>
#include <iostream>
namespace JGW
{
    CCJGW_Iperf3Command::CCJGW_Iperf3Command(void) : mpIperf3TestInfoFunc(NULL),mbIperfRunning(false),mnSeekgLog(0),mbMultiThread(false),mnConnectedCount(0)
    {
        mvEofChars.push_back('>');
        mvEofChars.push_back('#');
        mvEofChars.push_back('$');
    }


    CCJGW_Iperf3Command::~CCJGW_Iperf3Command(void)
    {
        StopIperf3();
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_Iperf3Command,CCMessageThread)

    END_MYTHREAD_MESSAGE_MAP()

    bool CCJGW_Iperf3Command::StartIper3Server(const std::wstring& strIperf3Path /* = L"tooliperf3.exe" */)
    {
        // iperf3 server default iperf3.exe -s --logfile xx.log -i 1 -f m 
        std::string strIperApplicationPath;
        std::string strApplicationFolder;

        mArgs.clear();
        mnSeekgLog = 0;
        mbMultiThread = false;
        BuildIperfTestLogPath();

        mArgs.push_back("-s");
        mArgs.push_back("--logfile");
        mArgs.push_back(JGW_W2A(mstrIperfTestLogPath));
        mArgs.push_back("-i");
        mArgs.push_back("1");
        mArgs.push_back("-f");
        mArgs.push_back("m");
        strApplicationFolder = JGW_W2A(JGW_GetApplicationFolder());
        JGW_FormatString(strIperApplicationPath,"%s%s",strApplicationFolder.c_str(),JGW_W2A(strIperf3Path).c_str());

        TerminateMessageThread();
        Sleep(250);
        mbIperfRunning = mRedirectConsole.LaunchConsole(strIperApplicationPath,mArgs,strApplicationFolder);
        if (mbIperfRunning)
        {
            CreateMessageThread(NULL,NULL,true);
        }
        return mbIperfRunning;
    }

    void CCJGW_Iperf3Command::RegisterCallbackIperf3TestInfo(Iperf3TestInfoFunc func)
    {
        mpIperf3TestInfoFunc = func;
    }

    bool CCJGW_Iperf3Command::StartIperf3Client(const std::wstring& strIPAddress,size_t nTestTimeSec,size_t nThreadCount,const std::wstring& strExtParam /* = L"-w 8M -l 65000" */,const std::wstring& strIperf3Path /* = L"tooliperf3.exe" */)
    {
        // iperf3 client default iperf3.exe -c 127.0.0.1 -t 10 -P 4 --logfile xx.log -i 1 -f m -w 8M -l 65000
        std::string strIperApplicationPath,strApplicationFolder,strTemp;

        mArgs.clear();
        mnSeekgLog = 0;
        mbMultiThread = false;
        BuildIperfTestLogPath();

        mArgs.push_back("-c");
        mArgs.push_back(JGW_W2A(strIPAddress));
        mArgs.push_back("-t");
        JGW_FormatString(strTemp,"%d",nTestTimeSec);
        mArgs.push_back(strTemp);
        mArgs.push_back("-P");
        JGW_FormatString(strTemp,"%d",nThreadCount);
        mArgs.push_back(strTemp);
        mArgs.push_back("--logfile");
        mArgs.push_back(JGW_W2A(mstrIperfTestLogPath));
        mArgs.push_back("-i");
        mArgs.push_back("1");
        mArgs.push_back("-f");
        mArgs.push_back("m");
        std::vector<std::wstring> vstrTemp;
        JGW_ParserStrW(strExtParam.c_str(),L" ",vstrTemp);
        for (std::vector<std::wstring>::iterator it = vstrTemp.begin();it != vstrTemp.end();++it)
        {
            mArgs.push_back(JGW_W2A(it[0]));
        }

        strApplicationFolder = JGW_W2A(JGW_GetApplicationFolder());
        JGW_FormatString(strIperApplicationPath,"%s%s",strApplicationFolder.c_str(),JGW_W2A(strIperf3Path).c_str());
        TerminateMessageThread();
        Sleep(250);
        mbIperfRunning = mRedirectConsole.LaunchConsole(strIperApplicationPath,mArgs,strApplicationFolder);
        if (mbIperfRunning)
        {
            CreateMessageThread(NULL,NULL,true);
        }
        return mbIperfRunning;
    }

    void CCJGW_Iperf3Command::BuildIperfTestLogPath()
    {
        JGW_FormatWString(mstrIperfTestLogPath,L"%s\\log\\iperf3\\%s\\%d_iperf.txt",JGW_GetApplicationFolder(),JGW_GetTimeFolder().c_str(),JGW_GetTimeOfDay());
        JGW_CreateDirectory(mstrIperfTestLogPath.c_str(),false);
    }

    void CCJGW_Iperf3Command::StopIperf3()
    {
        TerminateMessageThread();
        Sleep(250);
        mRedirectConsole.KillConsole();
        mbIperfRunning = false;
    }

    bool CCJGW_Iperf3Command::CheckIper3IsRunning()
    {
        return mRedirectConsole.ConsoleisRunning();
    }

    bool HandIperfTestLog(const std::wstring& strIperfLog,std::wstring& strID,size_t& start,size_t& end,float& speed)
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
            speed = static_cast<float>(_ttof (strIperfLog.substr(id + 1).c_str()));
            return true;
        }
        return false;
    }

    bool CCJGW_Iperf3Command::GetIperf3Speed(float& fSpeed)
    {
        std::wifstream iFileStream;
        wchar_t szReadBuf[256] = {0};

        fSpeed = 0.00f;
        iFileStream.open(mstrIperfTestLogPath.c_str());
        if (!iFileStream.is_open()) return false;

        std::wstring strID;
        size_t start,end;
        iFileStream.seekg(mnSeekgLog);//! connected to
        while (iFileStream.getline(szReadBuf,256))
        {
#if 0
            mnSeekgLog = iFileStream.tellg();
            std::wcout << szReadBuf << std::endl;
            if (NULL != wcsstr(szReadBuf,L"[SUM]")) mbMultiThread = true;
            if (mbMultiThread && NULL == wcsstr(szReadBuf,L"[SUM]")) continue;
            if (!HandIperfTestLog(szReadBuf,strID,start,end,fSpeed)) continue;
            if (mpIperf3TestInfoFunc) mpIperf3TestInfoFunc(fSpeed);
#else
            mnSeekgLog = iFileStream.tellg();
            std::wcout << szReadBuf << std::endl;
            if (NULL != wcsstr(szReadBuf,L"connected to")) mnConnectedCount++;
            if (mnConnectedCount > 1) mbMultiThread = true;
            if (1 == mnConnectedCount) mbMultiThread = false;
            if (mbMultiThread && NULL == wcsstr(szReadBuf,L"[SUM]")) continue;
            if (!HandIperfTestLog(szReadBuf,strID,start,end,fSpeed)) continue;
            if (mpIperf3TestInfoFunc) mpIperf3TestInfoFunc(fSpeed);
            //if (1 == mnConnectedCount) mbMultiThread = false;
            mnConnectedCount = 0;
#endif
        }
        iFileStream.close();

        return true;
    }

    void CCJGW_Iperf3Command::RunThread()
    {
        int count = 0;
        int totalCount = 0;
        float fSpeed = 0.00f;
        std::string temp;
        std::string strRecvA;
        std::vector<char> vRemoveChars;

        vRemoveChars.push_back('\r');
        vRemoveChars.push_back('\n');
        vRemoveChars.push_back(' ');
        vRemoveChars.push_back('\t');

        while (mbIperfRunning)
        {
            count = mRedirectConsole.RedirectOut(temp);
            if (-1 == count)
            {
                mRedirectConsole.KillConsole();
                GetIperf3Speed(fSpeed);
                break;
            }
#if 0
            else if (count > 0)
            {
                totalCount += count;
                strRecvA += temp;
                std::string result(strRecvA);
                EraseChars(result,vRemoveChars);

                if (!strRecvA.empty() && strRecvA.at(strRecvA.length() - 1) == _T('>'))
                {
                    strRecvA.erase(0,strRecvA.find('\n') + 1);
                    strRecvA.erase(strRecvA.find_last_of('\n') + 1);
                }
                if (result.length() > 1 && IsFindCharVector(result[result.length() - 1],mvEofChars))
                {
                    EraseChars(strRecvA,vRemoveChars);
                    break;
                }
            }
#endif
            GetIperf3Speed(fSpeed);
            Sleep(100);
        }
        GetIperf3Speed(fSpeed);
    }
}
