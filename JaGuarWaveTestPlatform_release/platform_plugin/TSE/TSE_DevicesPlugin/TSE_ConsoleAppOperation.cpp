#include "StdAfx.h"
#include "TSE_ConsoleAppOperation.h"
#include "TSE_DevicesGlobalResource.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <regex>
namespace JGW
{
    CTSE_ConsoleAppOperation::CTSE_ConsoleAppOperation(void) : mstrProcessPipeEnvironment(L"TSE_PROCESS_PIPE")
    {
    }


    CTSE_ConsoleAppOperation::~CTSE_ConsoleAppOperation(void)
    {
    }

    const wchar_t* CTSE_ConsoleAppOperation::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ConsoleAppOperation\",\
                \"TestName\": \"Console Application operation\",\
                \"CommandCount\": \"command count\",\
                \"ProcessPipeEnvironment\": \"process pipe Environment\",\
                \"ConsoleAppPath\": \"Console Application path\",\
                \"TimeOut1\": \"single time out 1:index\",\
                \"ErrorReTestCount1\": \"sigle error retest count 1:index\",\
                \"ErrorTestIntervalMSec1\": \"if error test interval msec 1:index\",\
                \"Command1\": \"command 1:index\",\
                \"FindOk1\": \"find ok string 1:index\",\
                \"WaitCommandEnd1\": \"wait command end 1:index\",\
                \"RegexRule1\": \"check ok Regex Rule 1:index\",\
                \"ResultEnvironment1\": \"save result 1:index\"\
                }";
    }

    bool CTSE_ConsoleAppOperation::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if( TSE_PARAM_NAME_EQUAL(_T("CommandCount"),strParamName) )
        {
            m_nCommandCount = _ttoi(strParamValue);
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("ConsoleAppPath"),strParamName) )
        {
            m_strConsoleAppPath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("ProcessPipeEnvironment"),strParamName) )
        {
            mstrProcessPipeEnvironment = strParamValue;
        }
        else
        {
            m_mParam[strParamName] = strParamValue;
        }      
        return true;
    }

    bool CTSE_ConsoleAppOperation::TSE_Init()
    {
        std::wstring strTimeOutParamName;
        std::wstring strErrorReTestCountParamName;
        std::wstring strErrorTestIntervalMSecParamName;
        std::wstring strCommandParamName;
        std::wstring strFindOkParamName;
        std::wstring strRegexRuleParamName;
        std::wstring strResultEnvironmentParamName;
        std::wstring strWaitCommandEndParamName;


        m_vCommand.clear();
        S_CONSOLE_COMMAND_INFO sCnsoleCommandInfo;

        for ( int i = 1;i <= m_nCommandCount;i ++)
        {
            sCnsoleCommandInfo.mnTimeOut = 10;
            sCnsoleCommandInfo.mnErrorReTestCount = 1;
            sCnsoleCommandInfo.mnErrorTestIntervalMSec = 0;
            sCnsoleCommandInfo.mstrCommand = L"";
            sCnsoleCommandInfo.mstrFindOk = L"";
            sCnsoleCommandInfo.mstrRegexRule = L"";
            sCnsoleCommandInfo.mstrResultEnvironment = L"";
            sCnsoleCommandInfo.mbWaitCommandEnd = true;

            JGW_FormatWString(strTimeOutParamName,_T("TimeOut%d"),i);
            JGW_FormatWString(strErrorReTestCountParamName,_T("ErrorReTestCount%d"),i);
            JGW_FormatWString(strErrorTestIntervalMSecParamName,_T("ErrorTestIntervalMSec%d"),i);
            JGW_FormatWString(strCommandParamName,_T("Command%d"),i);
            JGW_FormatWString(strFindOkParamName,_T("FindOk%d"),i);
            JGW_FormatWString(strRegexRuleParamName,_T("RegexRule%d"),i);
            JGW_FormatWString(strResultEnvironmentParamName,_T("ResultEnvironment%d"),i);
            JGW_FormatWString(strWaitCommandEndParamName,_T("WaitCommandEnd%d"),i);

            if( m_mParam.end() != m_mParam.find(strTimeOutParamName) )
            {
                sCnsoleCommandInfo.mnTimeOut = _ttoi(m_mParam[strTimeOutParamName].c_str());
                if (0 == sCnsoleCommandInfo.mnTimeOut)  sCnsoleCommandInfo.mnTimeOut = GetGlobalEnvironment()->GetInt(m_mParam[strTimeOutParamName]);
            }
            if( m_mParam.end() != m_mParam.find(strErrorReTestCountParamName) )
            {
                sCnsoleCommandInfo.mnErrorReTestCount = _ttoi(m_mParam[strErrorReTestCountParamName].c_str());
            }
            if( m_mParam.end() != m_mParam.find(strErrorTestIntervalMSecParamName) )
            {
                sCnsoleCommandInfo.mnErrorTestIntervalMSec = _ttoi(m_mParam[strErrorTestIntervalMSecParamName].c_str());    
            }
            if( m_mParam.end() != m_mParam.find(strWaitCommandEndParamName) )
            {
                sCnsoleCommandInfo.mbWaitCommandEnd = 1 == _ttoi(m_mParam[strWaitCommandEndParamName].c_str());  
            }
            if( m_mParam.end() != m_mParam.find(strCommandParamName) )
            {
                sCnsoleCommandInfo.mstrCommand = m_mParam[strCommandParamName];
            }
            if( m_mParam.end() != m_mParam.find(strFindOkParamName) )
            {
                sCnsoleCommandInfo.mstrFindOk = m_mParam[strFindOkParamName];
            }
            if( m_mParam.end() != m_mParam.find(strRegexRuleParamName) )
            {
                sCnsoleCommandInfo.mstrRegexRule = m_mParam[strRegexRuleParamName];
            }
            if( m_mParam.end() != m_mParam.find(strResultEnvironmentParamName) )
            {
                sCnsoleCommandInfo.mstrResultEnvironment = m_mParam[strResultEnvironmentParamName];
            }
            //JGW_ReplaceStringW(sCnsoleCommandInfo.mstrCommand,_T("$PATH$"),m_strCommandPath);      
            m_vCommand.push_back(sCnsoleCommandInfo);
        }

        return true;
    }

    bool CTSE_ConsoleAppOperation::TSE_Run()
    {
        CCJGW_ProcessPipe* pProcessPipe = &mcProcessPipe;
        if (!mstrProcessPipeEnvironment.empty())
        {
            pProcessPipe = (CCJGW_ProcessPipe*)(GetGlobalEnvironment()->GetInt(mstrProcessPipeEnvironment));
            if (NULL == pProcessPipe)
            {
                pProcessPipe = &mcProcessPipe;
                GetGlobalEnvironment()->PutInt(mstrProcessPipeEnvironment,(int)(&mcProcessPipe));
            }
        }

        for (auto it = m_vCommand.begin();it != m_vCommand.end();it++)
        {
            if(!ReadWriteConsoleApplication(pProcessPipe,&(it[0])))
            {
                /*Sleep(it->mnErrorTestIntervalMSec);*/
                Sleep(mnIntervalTimeMS);
                return false;
            }              
        }

        return true;
    }

    bool CTSE_ConsoleAppOperation::StartConsoleApplication(CCJGW_ProcessPipe* pProcessPipe)
    {
        if (pProcessPipe->CheckProcessIsRun())
        {
            return true;
        }

        std::wstring strApplicationFilePath (m_strConsoleAppPath);
        if (m_strConsoleAppPath.empty()) 
        {
            m_strConsoleAppPath = L"cmd.exe";
            strApplicationFilePath = JGW_RealativePathToAbsPath(m_strConsoleAppPath.c_str());
        }

        if (!pProcessPipe->InitProcessPipe(strApplicationFilePath.c_str(),L"",JGW_GetApplicationFolder())) { LogE(L"初始化控制台进程失败\n"); return false;}
        CTSE_DevicesGlobalResource::ReadCMDProcessPipeEOF(pProcessPipe,strApplicationFilePath);

        return true;
    }

    bool CTSE_ConsoleAppOperation::ReadWriteConsoleApplication(CCJGW_ProcessPipe* pProcessPipe,PS_CONSOLE_COMMAND_INFO psConsoleCommandInfo)
    {
        jgw_timer tm;
        std::wstring strRead,strCommand;
        int nTestIndex = 0;

        while(tm.elapsed() < psConsoleCommandInfo->mnTimeOut && nTestIndex++ < psConsoleCommandInfo->mnErrorReTestCount)
        {
            strCommand = psConsoleCommandInfo->mstrCommand;
            GetEnvironmentCommandString(strCommand);
            LogI_F(_T("send msg: %s\n"),strCommand.c_str());

            if (0 == psConsoleCommandInfo->mstrCommand.compare(L"exit") && !pProcessPipe->CheckProcessIsRun())
            {
                return true;
            }

            if (!StartConsoleApplication(pProcessPipe))
            {
                return false;
            }

            if (0 == psConsoleCommandInfo->mstrCommand.compare(L"EXIT_CMD"))
            {
                pProcessPipe->CloseProcessPipe();
                return true;
            }

            if(!pProcessPipe->timed_send(strCommand,10)) 
            { 
                Sleep(100); 
                continue;
            }
            if (psConsoleCommandInfo->mbWaitCommandEnd)
            {
                ReadConsoleApplication(pProcessPipe,strRead,(psConsoleCommandInfo->mnTimeOut / psConsoleCommandInfo->mnErrorReTestCount) > 1 ? (psConsoleCommandInfo->mnTimeOut / psConsoleCommandInfo->mnErrorReTestCount) : 1);
                if( psConsoleCommandInfo->mstrFindOk.empty() && psConsoleCommandInfo->mstrRegexRule.empty() ) return true;
                if (strRead.at(strRead.length() - 1) == _T('>'))
                {
                    JGW_EraseFristToRemoveCharW(strRead,L'\n');
                    JGW_EraseAfterToRemoveCharW(strRead,L'\n'); 
                }    
                JGW_EraseLastAndFristTwoCharsW(strRead);

                if (CheckReadResult(strRead,psConsoleCommandInfo))
                {
                    if (!psConsoleCommandInfo->mstrResultEnvironment.empty()) GetGlobalEnvironment()->PutString(psConsoleCommandInfo->mstrResultEnvironment,strRead);
                    return true;
                }
            }
            else
            {
                if (CheckReadConsoleResult(pProcessPipe,psConsoleCommandInfo,strRead))
                {
                    //if (!psConsoleCommandInfo->mstrResultEnvironment.empty()) GetGlobalEnvironment()->PutString(psConsoleCommandInfo->mstrResultEnvironment,strRead);
                    return true;
                }
            }
            Sleep(psConsoleCommandInfo->mnErrorTestIntervalMSec);
        }

        LogE(strRead.c_str());

        return false;
    }

    bool CTSE_ConsoleAppOperation::CheckReadResult(const std::wstring& strRead,PS_CONSOLE_COMMAND_INFO psConsoleCommandInfo)
    {
        //! 未找到
        if (std::wstring::npos == psConsoleCommandInfo->mstrFindOk.find(L"&&&") && std::wstring::npos == psConsoleCommandInfo->mstrFindOk.find(L"|||"))
        {
            if (NULL == JGW_WStrCaseWStr(strRead.c_str(),psConsoleCommandInfo->mstrFindOk.c_str()))
            {
                return false;
            }
        }

        if (std::wstring::npos != psConsoleCommandInfo->mstrFindOk.find(L"&&&"))
        {
            std::vector<std::wstring> vstrTemp;
            JGW_ParserStrW(psConsoleCommandInfo->mstrFindOk.c_str(),L"&&&",vstrTemp);
            for (size_t i = 0;i < vstrTemp.size();i ++)
            {
                if (NULL == JGW_WStrCaseWStr(strRead.c_str(),vstrTemp[i].c_str())) return false;
            }
        }


        if (std::wstring::npos != psConsoleCommandInfo->mstrFindOk.find(L"|||"))
        {
            std::vector<std::wstring> vstrTemp;
            JGW_ParserStrW(psConsoleCommandInfo->mstrFindOk.c_str(),L"|||",vstrTemp);
            for (size_t i = 0;i < vstrTemp.size();i ++)
            {
                if (NULL != JGW_WStrCaseWStr(strRead.c_str(),vstrTemp[i].c_str())) return true;
            }
            return false;
        }

        if (!psConsoleCommandInfo->mstrRegexRule.empty())
        {
            std::wregex partten(psConsoleCommandInfo->mstrRegexRule);
            if (!regex_match(strRead,partten))
            {
                LogE_F(L"rule:%s,value:%s,regex_match fail",psConsoleCommandInfo->mstrRegexRule.c_str(),strRead.c_str());
                return false;
            }
        }
        return true;
    }

    bool CTSE_ConsoleAppOperation::ReadConsoleApplication(CCJGW_ProcessPipe* pProcessPipe,std::wstring& strRead,size_t nTimeOutSec /* = 10 */)
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
                //trim::ctrim(strTemp);  //! JGW_ReplaceStringW(strTemp,_T("\r"),_T("\t")); //! JGW_ReplaceStringW(strTemp,_T("\n"),_T("\t"));
                if( strTemp.length() <= 0 ) continue;
                strRead += strTemp;
                LogI_F(_T("%s"),strTemp.c_str());
                JGW_EraseLastAndFristTwoCharsW(strRead);
                JGW_EraseLastAndFristTwoCharsW(strRead,' ',' ');
                if (strRead.length() > 1 && (strRead.at(strRead.length() - 1) == _T('>') || strRead.at(strRead.length() - 1) == _T('$') || strRead.at(strRead.length() - 1) == _T('#')))
                {
                    return true;
                }
            }
            Sleep(200);
        }
        if (strTemp.length() > 0)
        {
            return true;
        }
        pProcessPipe->CloseProcessPipe();
        return false;
    }

    bool CTSE_ConsoleAppOperation::CheckReadConsoleResult(CCJGW_ProcessPipe* pProcessPipe,PS_CONSOLE_COMMAND_INFO psConsoleCommandInfo,std::wstring& strRead)
    {
        jgw_timer tm;
        int nRecvLen = 0;
        std::wstring strTemp,strReadTemp,strCheckReadString;
        int timeOutSec = (psConsoleCommandInfo->mnTimeOut / psConsoleCommandInfo->mnErrorReTestCount) > 1 ? (psConsoleCommandInfo->mnTimeOut / psConsoleCommandInfo->mnErrorReTestCount) : 1;
        while(tm.elapsed() < timeOutSec)
        {
            strTemp = L"";
            nRecvLen = pProcessPipe->recv(strTemp);
            if( -1 == nRecvLen )
            {
                LogE(_T("error: process closed!!!"));
                return false;
            }
            else if (nRecvLen > 0 && strTemp.length() > 0)
            {
                strReadTemp += strTemp;

                if( psConsoleCommandInfo->mstrFindOk.empty() && psConsoleCommandInfo->mstrRegexRule.empty() ) return true;

                strCheckReadString = strReadTemp;
                if (strCheckReadString.at(strCheckReadString.length() - 1) == _T('>'))
                {
                    JGW_EraseFristToRemoveCharW(strCheckReadString,L'\n');
                    JGW_EraseAfterToRemoveCharW(strCheckReadString,L'\n'); 
                }    
                JGW_EraseLastAndFristTwoCharsW(strCheckReadString);
                JGW_EraseLastAndFristTwoCharsW(strCheckReadString,' ',' ');

                if (CheckReadResult(strCheckReadString,psConsoleCommandInfo))
                {
                    if (!psConsoleCommandInfo->mstrResultEnvironment.empty()) GetGlobalEnvironment()->PutString(psConsoleCommandInfo->mstrResultEnvironment,strCheckReadString);
                    strRead = strCheckReadString;
                    LogI(strCheckReadString.c_str());
                    return true;
                }

                if (strReadTemp.length() > 1 && (strReadTemp.at(strReadTemp.length() - 1) == _T('>') || strReadTemp.at(strReadTemp.length() - 1) == _T('$') || strReadTemp.at(strReadTemp.length() - 1) == _T('#')))
                {
                    LogI(strReadTemp.c_str());
                    //strRead = strCheckReadString;
                    return false;
                }
            }
            Sleep(200);
        }
        if (strCheckReadString.length() > 0)
        {
            strRead = strCheckReadString;
            return false;
        }
        pProcessPipe->CloseProcessPipe();
        return false;
    }
}
