#include "StdAfx.h"
#include "TSE_SendPipeCommand.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/ctrim.h>
#include <regex>
#include "TSE_DevicesGlobalResource.h"
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
namespace JGW
{
#define  SUITE_IsTrue(x,y)\
    if( !(x) )\
    return y;

    CTSE_SendPipeCommand::CTSE_SendPipeCommand(void):m_nCommandCount(0),m_strCommandLine(_T(""))
    {
    }


    CTSE_SendPipeCommand::~CTSE_SendPipeCommand(void)
    {
    }

    const wchar_t* CTSE_SendPipeCommand::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SendPipeCommand\",\
                \"TestName\": \"发送管道命令\",\
                \"CommandCount\": \"命令数量\",\
                \"CommandPath\": \"命令发送路径,默认不配置\",\
                \"Command1\": \"命令，1是索引\",\
                \"Recv1\": \"接收的命令返回信息比对 1是索引\",\
                \"SendCounts1\": \"发送失败或者读取比对失败发送次数 1是索引\",\
                \"TimeOut1\": \"超时时间设置 1是索引\",\
                \"Environment1\": \"接收的命令返回信息保存环境变量 1是索引\"\
                }";
    }
    // "Application":"程序路径","CommandLine":"命令行参数",
    bool CTSE_SendPipeCommand::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if( TSE_PARAM_NAME_EQUAL(_T("CommandCount"),strParamName) )
        {
            m_nCommandCount = _ttoi(strParamValue);
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("Application"),strParamName) )
        {
            m_strApplicationPath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("CommandLine"),strParamName) )
        {
            m_strCommandLine = strParamValue;
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("CommandPath"),strParamName) )
        {
            m_strCommandPath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else
        {
            m_mParam[strParamName] = strParamValue;
        }      
        return true;
    }

    bool CTSE_SendPipeCommand::TSE_Init()
    {
        std::wstring strCommandParam;
        std::wstring strRecvParam;
		std::wstring strErrorRecv;
        std::wstring strTimeOut;
        std::wstring strErrorReTest;
        std::wstring strEnvironment;

        m_vCommand.clear();
        S_COMMAND_PIPE sCommandPipe;

        for ( int i = 1;i <= m_nCommandCount;i ++)
        {
            sCommandPipe.m_nTimeOut = 1;
            sCommandPipe.m_nSendCounts = 1;
            sCommandPipe.mstrRecv = L"";
            sCommandPipe.mstrCommand = L"";
            sCommandPipe.mstrEnvironment = L"";
			sCommandPipe.mstrErrorRecv = L"";

            JGW_FormatWString(strCommandParam,_T("Command%d"),i);
            JGW_FormatWString(strRecvParam,_T("Recv%d"),i);
			JGW_FormatWString(strErrorRecv,_T("ErrorRecv%d"),i);
            JGW_FormatWString(strErrorReTest,_T("SendCounts%d"),i);
            JGW_FormatWString(strTimeOut,_T("TimeOut%d"),i);
            JGW_FormatWString(strEnvironment,_T("Environment%d"),i);

            if( m_mParam.end() != m_mParam.find(strCommandParam) )
            {
                sCommandPipe.mstrCommand = m_mParam[strCommandParam];
            }
            if( m_mParam.end() != m_mParam.find(strEnvironment) )
            {
                sCommandPipe.mstrEnvironment = m_mParam[strEnvironment];
            }
            if( m_mParam.end() != m_mParam.find(strRecvParam) )
            {
                sCommandPipe.mstrRecv = m_mParam[strRecvParam];         
            }
			if( m_mParam.end() != m_mParam.find(strErrorRecv) )
			{
				sCommandPipe.mstrErrorRecv = m_mParam[strErrorRecv];         
			}
            if( m_mParam.end() != m_mParam.find(strErrorReTest) )
            {
                sCommandPipe.m_nSendCounts = _ttoi(m_mParam[strErrorReTest].c_str());
            }
            if( m_mParam.end() != m_mParam.find(strTimeOut) )
            {
                sCommandPipe.m_nTimeOut = _ttoi(m_mParam[strTimeOut].c_str());
                if (0 == sCommandPipe.m_nTimeOut)  sCommandPipe.m_nTimeOut = GetGlobalEnvironment()->GetInt(m_mParam[strTimeOut]);
            }
            JGW_ReplaceStringW(sCommandPipe.mstrCommand,_T("$PATH$"),m_strCommandPath);      
            m_vCommand.push_back(sCommandPipe);
        }

        return true;
    }

    bool CTSE_SendPipeCommand::TSE_Run()
    {
        m_pCMDPipe = CTSE_DevicesGlobalResource::GetProcessCMDPipe();
        if (!m_pCMDPipe)
        {
            LogE(L"打开控制台失败\n");
            Sleep(mnIntervalTimeMS);
            return false;
        }

        for (auto it = m_vCommand.begin();it != m_vCommand.end();it++)
        {
            if(!WriteReadPide( it[0] )) 
            {
                Sleep(mnIntervalTimeMS);
                return false;
            }    
        }
        return true;
    }

    void CTSE_SendPipeCommand::GetEnvironmentCommand(std::wstring& strCommand)
    {
        std::wstring strTemp = strCommand,strEnvironmentValue,strEnvironmentName,strReplaceSrc;
        int nStart = strTemp.find(L"$$_"),nEnd = 0;
        /*
        JGW_ReplaceStringW(strTemp,L"$$_",L"");
        JGW_ReplaceStringW(strTemp,L"_$$",L"");
        strCommand = GetGlobalEnvironment()->GetString(strTemp.c_str());
        */
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

    bool CTSE_SendPipeCommand::WriteReadPide(S_COMMAND_PIPE scommand)
    {
        jgw_timer tm;
        std::wstring strRead;
        int i = 0;

        while( tm.elapsed() < scommand.m_nTimeOut && i++ < scommand.m_nSendCounts )
        {
            JGW_ReplaceStringW(scommand.mstrCommand,_T("$TSE_FTM_CLIENT_SOCKET_IP_ADDRESS$"),GetGlobalEnvironment()->GetString(TSE_FTM_CLIENT_SOCKET_IP_ADDRESS));
            GetEnvironmentCommand(scommand.mstrCommand);
            LogI_F(_T("send msg: %s\n"),scommand.mstrCommand.c_str());

            if (0 == scommand.mstrCommand.compare(L"EXIT_CMD"))
            {
                m_pCMDPipe->CloseProcessPipe();
                return true;
            }
            m_pCMDPipe = CTSE_DevicesGlobalResource::GetProcessCMDPipe();
            if(!m_pCMDPipe->timed_send(scommand.mstrCommand,scommand.m_nTimeOut)) { Sleep(100); continue;}

            CTSE_DevicesGlobalResource::ReadCMDProcessPipeEOF(m_pCMDPipe,strRead,scommand.m_nTimeOut);
            if( scommand.mstrRecv.empty() && scommand.mstrEnvironment.empty()) return true;

            if (strRead.at(strRead.length() - 1) == _T('>'))
            {
                JGW_EraseFristToRemoveCharW(strRead,L'\n');
                JGW_EraseAfterToRemoveCharW(strRead,L'\n'); 
            }    
            JGW_EraseLastAndFristTwoCharsW(strRead);
            //! JGW_ReplaceStringW(strRead,scommand.strCommand,_T(""));
            //if (NULL != JGW_WStrCaseWStr(strRead.c_str(),scommand.mstrRecv.c_str()))
			if (!scommand.mstrErrorRecv.empty())
			{
				if (FindRecvResult(strRead,scommand.mstrErrorRecv))
				{
					Sleep(100);
					continue;
				}
			}

            if (FindRecvResult(strRead,scommand.mstrRecv))
            {
                if (!scommand.mstrEnvironment.empty()) GetGlobalEnvironment()->PutString(scommand.mstrEnvironment,strRead);
                return true;
            }
			Sleep(100);
            /*std::tr1::wregex regExpress(scommand.strRecv,std::tr1::regex_constants::icase);
            if(regex_match(strRead,regExpress)) return true;*/
        }
        LogE(strRead.c_str());
        //         if( m_pCMDPipe->timed_send(std::string("cls\n"),2) )
        //         {
        //             CTSE_DevicesGlobalResource::ReadCMDProcessPipeEOF(m_pCMDPipe,strRead);
        //         }

        return false;
    }

    bool CTSE_SendPipeCommand::FindRecvResult(const std::wstring& strRead,const std::wstring& strCompare)
    {
		if (strCompare.empty())
		{
			return true;
		}
        //! 未找到
        if (std::wstring::npos == strCompare.find(L"&&&") && std::wstring::npos == strCompare.find(L"|||"))
        {
            return (NULL != JGW_WStrCaseWStr(strRead.c_str(),strCompare.c_str()));
        }

        if (std::wstring::npos != strCompare.find(L"&&&"))
        {
            std::vector<std::wstring> vstrTemp;
            JGW_ParserStrW(strCompare.c_str(),L"&&&",vstrTemp);
            for (size_t i = 0;i < vstrTemp.size();i ++)
            {
                if (NULL == JGW_WStrCaseWStr(strRead.c_str(),vstrTemp[i].c_str())) return false;
            }
        }


        if (std::wstring::npos != strCompare.find(L"|||"))
        {
            std::vector<std::wstring> vstrTemp;
            JGW_ParserStrW(strCompare.c_str(),L"|||",vstrTemp);
            for (size_t i = 0;i < vstrTemp.size();i ++)
            {
                if (NULL != JGW_WStrCaseWStr(strRead.c_str(),vstrTemp[i].c_str())) return true;
            }
            return false;
        }

        return true;
    }
}