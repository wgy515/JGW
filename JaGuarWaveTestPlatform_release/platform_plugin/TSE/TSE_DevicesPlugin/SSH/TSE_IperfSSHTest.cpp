#include "StdAfx.h"
#include "TSE_IperfSSHTest.h"
#include "..\TSE_DevicesGlobalResource.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>

namespace JGW
{
    CTSE_IperfSSHTest::CTSE_IperfSSHTest(void)
    {
    }


    CTSE_IperfSSHTest::~CTSE_IperfSSHTest(void)
    {
    }

    const wchar_t* CTSE_IperfSSHTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_IperfSSHTest\",\
                \"TestName\": \"iperf SSH(AP)\",\
                \"IperfCommand\": \"iperf 测试命令\",\
                \"IperfLogEnvironment\": \"iperf测试log保存的环境变量\",\
                \"IperfTestRangeEnvironment\": \"iperf 测试速率范围环境变量\",\
                \"IperfTestTimeEnvironment\": \"iperf 测试时间环境变量\",\
                \"IperfCommandTimeOutEnvironment\": \"iperf测试超时时间环境变量\",\
                \"SNEnvironment\": \"SN环境变量\",\
                \"SaveTestLogging\": \"是否保存测试log\"\
                }";
    }

    bool CTSE_IperfSSHTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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

    void CTSE_IperfSSHTest::GetEnvironmentCommand(std::wstring& strCommand)
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

    void CTSE_IperfSSHTest::BuildIperfTestLogPath()
    {
        JGW_FormatWString(mstrIperfTestLogPath,L"%s\\log\\iperf\\%s\\%u_iperf.txt",JGW_GetApplicationFolder(),JGW_GetTimeFolder().c_str(),JGW_GetTimeOfDay());
        JGW_CreateDirectory(mstrIperfTestLogPath.c_str(),false);
        GetGlobalEnvironment()->PutString(mstrIperfLogEnviroment,mstrIperfTestLogPath);
        if (mofstream.is_open()) mofstream.close();
        mofstream.open(mstrIperfTestLogPath.c_str(),std::ios::trunc | std::ios::out);
    }

    bool CTSE_IperfSSHTest::HandIperfTestLog(const std::wstring& strIperfLog,std::wstring& strID,size_t& start,size_t& end,double& speed)
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


    void CTSE_IperfSSHTest::UpdateIperfLogInfo(const std::wstring& strBuf)
    {
        std::wifstream iFileStream;
        std::wstring strID;
        size_t start = 0,end = 0;
        double speed = 0.00;

        std::vector<std::wstring> vtemp;
        JGW_ParserStrW(strBuf.c_str(),L"\n",vtemp);

        for (size_t i = 0;i < vtemp.size();i ++)
        {
            const wchar_t* szReadBuf = vtemp[i].c_str();

            LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,szReadBuf);
            if (NULL != wcsstr(szReadBuf,L"[SUM]")) mbMultThread = true;
            if (mbMultThread && NULL == wcsstr(szReadBuf,L"[SUM]")) continue ;
            if (!HandIperfTestLog(szReadBuf,strID,start,end,speed)) continue ;

            if (0 == start && mnTestTotalTimes == end)
            {
                mfTestSpeed = static_cast<float>(speed);
            }
        }  
    }

    bool CTSE_IperfSSHTest::ReadSSHIperfCMD(std::wstring& strRead,size_t nTimeOutSec /* = 10 */)
    {
        strRead = L"";
        int readSize = 0;
        jgw_timer tm;
        //!std::wstring strTemp;

        while(tm.elapsed() < nTimeOutSec)
        {
            readSize = CTSE_DevicesGlobalResource::GetSSHShell()->ReadSSHShell(strRead);
            if(readSize < 0)
            {
                LogE(_T("error: process closed!!!"));
                return false;
            }
            else if (readSize > 0)
            {
                mofstream << strRead;
                mofstream.flush();
                UpdateIperfLogInfo(strRead);
                Sleep(1000);
                if (strRead.length() < 3) continue;
                if (('#' == strRead[strRead.length() - 2] && ']' == strRead[strRead.length() - 3])
                    || ('$' == strRead[strRead.length() - 2] && '~' == strRead[strRead.length() - 3]))
                {
                    return true;
                }
                strRead = L"";
            }
            else
            {
                Sleep(100);
            }
        }
        UpdateIperfLogInfo(strRead);
        return false;
    }

    bool CTSE_IperfSSHTest::TSE_Run()
    {
        mbTestStatus = false;
        mbMultThread = false;
        //mnSeekgLog = 0;
        mfTestSpeed = 0;
        BuildIperfTestLogPath();
        std::wstring strIperfCommand(mstrIperfCommand),strRead,strTemp;
        mnTestTotalTimes = GetGlobalEnvironment()->GetInt(mstrIperfTestTimeEnvironment);
        mfTestMinSpeed = GetGlobalEnvironment()->GetFloat(mstrIperfTestRangeEnvironment);
        GetEnvironmentCommand(strIperfCommand);
        size_t nTimeOutSec = GetGlobalEnvironment()->GetInt(mstrIperfCommandTimeOutEnvironment);

        if (!mofstream.is_open())
        {
            LogE(L"iperf文件打开失败\n,请检查程序是否有可读写权限");
            return false;
        }

        if (!CTSE_DevicesGlobalResource::GetSSHShell()->SendSSHShell(JGW_W2A(strIperfCommand))) return false;

        if (!ReadSSHIperfCMD(strRead,nTimeOutSec))
        {
            return false;
        }
        //! 判断速率是否达标
        JGW_FormatWString(strTemp,L"%.0f Mbits/sec",mfTestSpeed);
        LogU(E_TSE_SHOW_CONTENT_INFO,strTemp.c_str());
        mbTestStatus = (mfTestSpeed >= mfTestMinSpeed);
        return mbTestStatus;
    }

    bool CTSE_IperfSSHTest::TSE_Exit()
    {
        if (mofstream.is_open()) mofstream.close();
        if (!mbSaveTestLogging) return true;
        std::wstring strSN = GetGlobalEnvironment()->GetString(mstrSNEnvironment);
        std::wstring strTestName = GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME);
        mMoveTestLogging.MoveTestLogging(strTestName.c_str(),strSN.c_str(),mstrIperfTestLogPath.c_str(),mbTestStatus);
        return true;
    }
}