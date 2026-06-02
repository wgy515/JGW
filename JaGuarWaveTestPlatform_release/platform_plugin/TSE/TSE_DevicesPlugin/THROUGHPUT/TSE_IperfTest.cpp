#include "StdAfx.h"
#include "TSE_IperfTest.h"
#include <fstream>
#include <JGW_FoundationFunc/jgw_timer.hpp>

namespace JGW
{
    CTSE_IperfTest::CTSE_IperfTest(void)
    {
    }


    CTSE_IperfTest::~CTSE_IperfTest(void)
    {
    }

    const wchar_t* CTSE_IperfTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_IperfTest\",\
                \"TestName\": \"iperf测试\",\
                \"IperfCommand\": \"iperf 测试命令\",\
                \"IperfLogEnvironment\": \"iperf测试log保存的环境变量\",\
                \"IperfTestRangeEnvironment\": \"iperf 测试速率范围环境变量\",\
                \"IperfTestTimeEnvironment\": \"iperf 测试时间环境变量\",\
                \"IperfCommandTimeOutEnvironment\": \"iperf测试超时时间环境变量\",\
                \"SNEnvironment\": \"SN环境变量\",\
                \"SaveTestLogging\": \"是否保存测试log\"\
                }";
    }

    bool CTSE_IperfTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"IperfCommand"))
        {
            mstrIperfCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IperfLogEnvironment"))
        {
            mstrIperfLogEnviroment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IperfTestRangeEnvironment"))
        {
            mstrIperfTestRangeEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IperfTestTimeEnvironment"))
        {
            mstrIperfTestTimeEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IperfCommandTimeOutEnvironment"))
        {
            mstrIperfCommandTimeOutEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SNEnvironment"))
        {
            mstrSNEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SaveTestLogging"))
        {
            mbSaveTestLogging = (_ttoi(strParamValue) == 1);
        }
        else
        {
            return false;
        }
        return true;
    }

    void CTSE_IperfTest::GetEnvironmentCommand(std::wstring& strCommand)
    {
        std::wstring strTemp = strCommand,strEnvironmentValue,strEnvironmentName,strReplaceSrc;
        int nStart = strTemp.find(L"$$_"),nEnd = 0;

        while (std::wstring::npos != nStart)
        {
            nEnd = strTemp.find(L"_$$",nStart + 3);
            if (nEnd == std::wstring::npos) return ;
            strEnvironmentName = strTemp.substr(nStart + 3,nEnd - nStart - 3);
            strEnvironmentValue = GetGlobalEnvironment()->GetString(strEnvironmentName);
            JGW_FormatWString(strReplaceSrc,L"$$_%s_$$",strEnvironmentName.c_str());
            JGW_ReplaceStringW(strCommand,strReplaceSrc,strEnvironmentValue);
            nStart = strTemp.find(L"$$_",nEnd + 3);
        } 
    }

    void CTSE_IperfTest::BuildIperfTestLogPath()
    {
        JGW_FormatWString(mstrIperfTestLogPath,L"%s\\log\\iperf\\%s\\%d_iperf.txt",JGW_GetApplicationFolder(),JGW_GetTimeFolder().c_str(),JGW_GetTimeOfDay());
        JGW_CreateDirectory(mstrIperfTestLogPath.c_str(),false);
        GetGlobalEnvironment()->PutString(mstrIperfLogEnviroment,mstrIperfTestLogPath);
    }

    bool CTSE_IperfTest::HandIperfTestLog(const std::wstring& strIperfLog,std::wstring& strID,size_t& start,size_t& end,double& speed)
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


    void CTSE_IperfTest::UpdateIperfLogInfo()
    {
        std::wifstream iFileStream;
        std::wstring strTemp,strID;
        size_t start = 0,end = 0;
        double speed = 0.00;
        wchar_t szReadBuf[256] = {0};

        iFileStream.open(mstrIperfTestLogPath.c_str());
        if (!iFileStream.is_open()) return ;
        iFileStream.seekg(mnSeekgLog);

        while (iFileStream.getline(szReadBuf,256))
        {
            LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,szReadBuf);
            mnSeekgLog = iFileStream.tellg();
            if (NULL != wcsstr(szReadBuf,L"[SUM]")) mbMultThread = true;
            if (mbMultThread && NULL == wcsstr(szReadBuf,L"[SUM]")) continue;
            if (!HandIperfTestLog(szReadBuf,strID,start,end,speed)) continue;

            if (0 == start && mnTestTotalTimes == end)
            {
                mfTestSpeed = static_cast<float>(speed);
            }
        }
        iFileStream.close();
    }

    bool CTSE_IperfTest::ReadIperfCMDProcessPipe(CCJGW_ProcessPipe* pProcessPipe,std::wstring& strRead,size_t nTimeOutSec /* = 10 */)
    {
        if (!pProcessPipe) return false;
        strRead = L"";
        int readSize = 0;
        jgw_timer tm;
        std::wstring strTemp;

        while(tm.elapsed() < nTimeOutSec)
        {
            readSize = pProcessPipe->recv(strTemp);
            if( -1 == readSize )
            {
                LogE(_T("error: process closed!!!"));
                return false;
            }
            else if ( readSize > 0 )
            {
                if( strTemp.length() <= 0 ) continue;
                strRead += strTemp;
                LogI_F(_T("%s"),strTemp.c_str());
                if (strRead.length() > 1 && (strRead.at(strRead.length() - 1) == _T('>') || strRead.at(strRead.length() - 1) == _T('$') || strRead.at(strRead.length() - 1) == _T('#')))
                {
                    UpdateIperfLogInfo();
                    return true;
                }
            }
            else
            {
                UpdateIperfLogInfo();
                Sleep(1000);
            }
        }
        UpdateIperfLogInfo();
        pProcessPipe->CloseProcessPipe();
        return false;
    }


    bool CTSE_IperfTest::TSE_Run()
    {
        mbTestStatus = false;
        mbMultThread = false;
        mnSeekgLog = 0;
        mfTestSpeed = 0;
        BuildIperfTestLogPath();
        std::wstring strIperfCommand(mstrIperfCommand),strRead,strTemp;
        mnTestTotalTimes = GetGlobalEnvironment()->GetInt(mstrIperfTestTimeEnvironment);
        mfTestMinSpeed = GetGlobalEnvironment()->GetFloat(mstrIperfTestRangeEnvironment);
        GetEnvironmentCommand(strIperfCommand);
        size_t nTimeOutSec = GetGlobalEnvironment()->GetInt(mstrIperfCommandTimeOutEnvironment);

        if(!CTSE_DevicesGlobalResource::GetProcessCMDPipe()->timed_send(strIperfCommand,nTimeOutSec)) return false;

        if (!ReadIperfCMDProcessPipe(CTSE_DevicesGlobalResource::GetProcessCMDPipe(),strRead,nTimeOutSec))
        {
            return false;
        }
        //! 判断速率是否达标
        JGW_FormatWString(strTemp,L"%.0f Mbits/sec",mfTestSpeed);
        LogU(E_TSE_SHOW_CONTENT_INFO,strTemp.c_str());
        mbTestStatus = (mfTestSpeed >= mfTestMinSpeed);
        return mbTestStatus;
    }

    bool CTSE_IperfTest::TSE_Exit()
    {
        if (!mbSaveTestLogging) return true;
        std::wstring strSN = GetGlobalEnvironment()->GetString(mstrSNEnvironment);
        std::wstring strTestName = GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME);
        mMoveTestLogging.MoveTestLogging(strTestName.c_str(),strSN.c_str(),mstrIperfTestLogPath.c_str(),mbTestStatus);
        return true;
    }
}