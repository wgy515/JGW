#include "StdAfx.h"
#include "TSE_QSPRAdaptTest.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <TSE_MessageServer/TSE_MessageServerDllExport.h>
#include <TSE_MSFoundationLib/TSE_MSLogJson.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include "CJGW_HtmlQdartResolveConfig.h"
#include "CJGW_ResolveQdartHtml.h"

//#define DEBUG_TSE_MS_MC
namespace JGW
{
    CTSE_QSPRAdaptTest::CTSE_QSPRAdaptTest(void) : mbFristStartApp(true),mnDefaultRecvTimeOutSec(10),meQSPREngineType(QSPRGUIType),mbSaveTestLogging(false),mbTestResult(false),meShowTestLogType(E_TF_SHOW_FAIL_NOT_PASS_LOG)
    {
    }


    CTSE_QSPRAdaptTest::~CTSE_QSPRAdaptTest(void)
    {
    }

    const wchar_t* CTSE_QSPRAdaptTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_QSPRAdaptTest\",\
                \"TestName\": \"QSPR适配测试\",\
                \"ApplicationPathEnvironment\": \"application path environment\",\
                \"QSPREngineType\": \"qspr engine type 0 : QSPRGUIType,1 : QIAType\",\
                \"XttPathEnvironment\": \"xtt file path environment\",\
                \"TSEGlobalVariableKey\": \"tse global variable key\",\
                \"XttGlobalVariableKey\": \"xtt global variable key\",\
                \"SaveTestLogging\": \"保存测试记录\",\
                \"ShowTestLogType\": \"0:不显示PASS和FAIL LOG,1:TSET FAIL 显示FAIL LOG 但不显示PASS LOG,2:TSET FAIL 显示所有LOG,3:TSET PASS 显示所有LOG\",\
                \"SNEnvironment\": \"sn environment\",\
                \"HtmlResolveRuleFilePath\": \"qdart html resolve rule file path\",\
                \"TestTimeOutSec\": \"test time out second\"\
                }";
    }

    bool CTSE_QSPRAdaptTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"XttPathEnvironment",strParamName))
        {
            mstrXttPathEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ApplicationPathEnvironment"))
        {
            mstrApplicationPathEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TSEGlobalVariableKey"))
        {
            mstrTSEGlobalVariableKey = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"XttGlobalVariableKey",strParamName))
        {
            mstrXttGlobalVariableKey = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"QSPREngineType",strParamName))
        {
            meQSPREngineType = _ttoi(strParamValue) == 0 ? QSPRGUIType : QIAType;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SaveTestLogging"),strParamName))
        {
            mbSaveTestLogging = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("ShowTestLogType"),strParamName))
        {
            meShowTestLogType = (EHTML_SHOW_LOG_TYPE)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("HtmlResolveRuleFilePath"),strParamName))
        {
            mstrHtmlResolveRuleFilePath = JGW_RealativePathToAbsPath(strParamValue);
            CCJGW_HtmlQdartResolveConfig htmlQdartResolveConfig;
            htmlQdartResolveConfig.LoadHtmlQdartResolveConfig(mstrHtmlResolveRuleFilePath,msHtmlQdartResolveConfig);  
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SNEnvironment"),strParamName))
        {
            mstrSNEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"TestTimeOutSec",strParamName))
        {
            mnTestTimeOutSec = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_QSPRAdaptTest::TSE_Run()
    {
        mbTestResult = false;
        mvstrSaveHtmlFilePath.clear();
        mstrRecordTestItemLog.clear();
#ifndef DEBUG_TSE_MS_MC
        if (!StartApplicationProcess())
        {
            return false;
        }
#else
        mstrTSEMSID = "TSEMS_1";
#endif
        //! 检测TSE MC是否已经注册上
        CTSE_ControlMsgBase* pControlMsgBase = ( CTSE_ControlMsgBase*)GetControlMsgBase();
        CTSE_LogMsgBase* pLogMsgBase = NULL;
        if (NULL == pControlMsgBase)
        {
            LogE("Start QSPR Adapt Test Fail!!!");
            GetApplicationProcessErrorMsg();
            mcApplicationPipe.CloseProcessPipe();
            return false;
        }
        if (mbFristStartApp)
        {
            //! 接收到TSE MC注册信息
            if (!HandleWithControlRegMsg(pControlMsgBase))
            {
                LogE("Did not receive the control msg!!!");
                pControlMsgBase->CloseControlMsg();
                mcApplicationPipe.CloseProcessPipe();
                GetApplicationProcessErrorMsg();
                return false;
            }
            mbFristStartApp = false;
        }

        //! 发送XTT相关信息给 TSE MC
        if (!HandleWithControlXttMsg(pControlMsgBase))
        {
            LogE("Load Xtt Msg Fail!!!");
            pControlMsgBase->CloseControlMsg();
            mcApplicationPipe.CloseProcessPipe();
            GetApplicationProcessErrorMsg();
            return false;
        }
        GetApplicationInfoMsg();
        //! 发送全局变量信息给 TSE MC
        if (!HandleWithControlGlobalVariableMsg(pControlMsgBase))
        {
            LogE("Send Global Variable Msg Fail!!!");
            GetApplicationProcessErrorMsg();
            return false;
        }
        GetApplicationInfoMsg();
        //! 发送开始测试信息给TSE MC
        if (!HandleWithControlOperTestMsg(pControlMsgBase))
        {
            LogE("Send Start Test  Msg Fail!!!");
            GetApplicationProcessErrorMsg();
            return false;
        }
        GetApplicationInfoMsg();
        if (!HandleWithLogAndControlMsg(pControlMsgBase))
        { 
            GetApplicationProcessErrorMsg();
            return false;
        }
        mbTestResult = true;
        return true;
    }

    void* CTSE_QSPRAdaptTest::GetControlMsgBase()
    {
        jgw_timer jt;
        while (jt.elapsed() <= mnDefaultRecvTimeOutSec)
        {
            CTSE_ControlMsgBase* pControlMsgBase = (CTSE_ControlMsgBase*)GetTSEControlBaseToID(mstrTSEMSID.c_str());
            if (NULL != pControlMsgBase)
            {
                return pControlMsgBase;
            }
            Sleep(200);
        }
        return NULL;
    }

    bool CTSE_QSPRAdaptTest::StartApplicationProcess()
    {
        if (mcApplicationPipe.CheckProcessIsRun())
        {
            return true;
        }
        mbFristStartApp = true;
        mstrTSEMSID = GetTSEMSID();
        std::wstring strApplicationPath = JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrApplicationPathEnvironment).c_str());
        std::wstring strArg;
        JGW_FormatWString(strArg,L" %s",JGW_A2W(mstrTSEMSID).c_str());
        if(!mcApplicationPipe.InitProcessPipe(strApplicationPath.c_str(),strArg,JGW_GetApplicationFolder()))
        {
            LogE_F(L"Start Process Fail(%s)",strApplicationPath.c_str());
            return false;
        }
        return true;
    }

    bool CTSE_QSPRAdaptTest::HandleWithControlRegMsg(CTSE_ControlMsgBase* pControlMsgBase)
    {
        jgw_timer jt;
        std::string strMsg;
        //! 35  => 30S是重复发送和接收得时间 5S留的定时时间
        while(jt.elapsed() <= 35)
        {
            if (pControlMsgBase->ReadControlMsg(strMsg))
            {
                TSEControlTypeEnum  eControlType;
                if (!CTSE_MSLogJson::GetMSControlType(eControlType,strMsg))
                {
                    continue;
                }
                if (eControlType == OnHandleReadControlMsg(pControlMsgBase,strMsg))
                {
                    return true;
                }
            }
            Sleep(200);
        }
        return false;
    }

    bool CTSE_QSPRAdaptTest::HandleWithControlXttMsg(CTSE_ControlMsgBase* pControlMsgBase)
    {
        std::wstring strXttPath = JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrXttPathEnvironment).c_str());
        std::string strMsg;

        if (!JGW_FileExistsToFilePath(strXttPath.c_str()))
        {
            LogE_F(L"Not Find Xtt File(%s) Fail !!!",strXttPath.c_str());
            return false;
        }

        pControlMsgBase->WriteControlMsg(CTSE_MSLogJson::GetMSXttJson(meQSPREngineType,JGW_W2A(strXttPath)));
        if (!ReadControlRspMsgToExpectControlType(pControlMsgBase,TSEControlXttResultType,strMsg,mnDefaultRecvTimeOutSec))
        {
            LogE(L"Not Recv Xtt Load Result Fail !!!");
            return false;
        }

        S_MC_RSP_JSON smcRspJson;
        if (!CTSE_MSLogJson::GetMCResponseJsonToJson(strMsg,smcRspJson))
        {
            LogE(L"Get Xtt Load Result Fail !!!");
            return false;
        }

        if (!smcRspJson.Result)
        {
            std::wstring strLog = L"Load Xtt Error : ";
            strLog += JGW_A2W(smcRspJson.Error,CP_UTF8).c_str();
            LogE(strLog.c_str());
            return false;
        }

        return true;
    }

    bool CTSE_QSPRAdaptTest::HandleWithControlGlobalVariableMsg(CTSE_ControlMsgBase* pControlMsgBase)
    {
        std::vector<std::wstring> vstrKey,vstrValue;
        JGW_ParserStrW(mstrXttGlobalVariableKey.c_str(),L",",vstrKey);
        JGW_ParserStrW(mstrTSEGlobalVariableKey.c_str(),L",",vstrValue);
        if (vstrKey.size() != vstrValue.size())
        {
            LogE(L"GlobalVariable Config Fail !!!");
            return false;
        }

        std::string strMsg;
        for (size_t i = 0;i < vstrKey.size();i ++)
        {
            pControlMsgBase->WriteControlMsg(CTSE_MSLogJson::GetMSGlobalVariableJson(true,JGW_W2A(vstrKey[i]),GetGlobalEnvironment()->GetAnsiString(vstrValue[i])));
            if (!ReadControlRspMsgToExpectControlType(pControlMsgBase,TSEControlGlobalVariableResultType,strMsg,mnDefaultRecvTimeOutSec))
            {
                LogE(L"Not Recv Global Variable  Result Fail !!!");
                return false;
            }
        }

        return true;
    }

    bool CTSE_QSPRAdaptTest::HandleWithControlOperTestMsg(CTSE_ControlMsgBase* pControlMsgBase)
    {
        std::string strMsg;
        pControlMsgBase->WriteControlMsg(CTSE_MSLogJson::GetMSBaseJson(TSEControlOperTestType));
        if (!ReadControlRspMsgToExpectControlType(pControlMsgBase,TSEControlOperTestResultType,strMsg,/*mnDefaultRecvTimeOutSec*/60))
        {
            LogE(L"Not Start Test Result Fail !!!");
            return false;
        }

        S_MC_RSP_JSON smcRspJson;
        if (!CTSE_MSLogJson::GetMCResponseJsonToJson(strMsg,smcRspJson))
        {
            LogE(L"Get Start Test Result Fail !!!");
            return false;
        }

        if (!smcRspJson.Result)
        {
            std::wstring strLog = L"Start Test Error : ";
            strLog += JGW_A2W(smcRspJson.Error,CP_UTF8).c_str();
            LogE(strLog.c_str());
            return false;
        }

        return true;
    }

    bool CTSE_QSPRAdaptTest::HandleWithLogAndControlMsg(CTSE_ControlMsgBase* pControlMsgBase)
    {
        jgw_timer jt;
        std::string strRead;
        //! 测试失败后，在下次测试执行前停止记录测试数据
        bool bPrevTestResult = true;
        bool bPrevTestEnd = false;
        
        CTSE_LogMsgBase* pLogMsgBase = NULL;
        TSEControlTypeEnum eTSEControlTypeEnum;
        while (jt.elapsed() <= mnTestTimeOutSec)
        {
            if (NULL == pLogMsgBase)
            {
                pLogMsgBase = (CTSE_LogMsgBase*)GetTSELogBaseToID(mstrTSEMSID.c_str());
            }
            else
            {
                while (pLogMsgBase->ReadLog(strRead))
                {
                    if (strRead.empty()) continue;
                    std::string type = strRead.substr(0,1);
                    std::wstring strReadTemp = JGW_A2W(strRead.substr(1,strRead.length()),CP_UTF8);
                    QSPREngineMsgType eQSPREngineMsgType = QSPREngineMsgType(atoi(type.c_str()));
                    switch (eQSPREngineMsgType)
                    {
                    case DebugMsgType:
                        LogI(strReadTemp.c_str());
                        if (!bPrevTestEnd)
                        {
                            mstrRecordTestItemLog << strReadTemp << std::endl;
                        }
                        break;
                    case GlobalVariableMsgType:
                        LogI(strReadTemp.c_str());
                        break;
                    case ProcessMsgType:
                        LogI(strReadTemp.c_str());
                        break;
                    case TestItemStatusMsgType:
                        LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,strReadTemp.c_str());
                        //! Passed
                        if (JGW_WStrCaseWStr(strReadTemp.c_str(),L"Failed"))
                        {
                            bPrevTestResult = false;
                        }
                        if (JGW_WStrCaseWStr(strReadTemp.c_str(),L"Running"))
                        {
                            //！ 如果上次测试结果为TRUE，则清空测试记录
                            if (bPrevTestResult)
                            {
                                mstrRecordTestItemLog.clear();
                            }
                            else //! 如果测试结果false，则停止开始记录测试log
                            {
                                bPrevTestEnd = true;
                            }
                        }
                        break;
                    case TestMessageReceivedType:
                        LogI(strReadTemp.c_str());
                        break;
                    case TestResultFileType:
                        //psRFQualcommInterface->SaveHtmlLogging(strTemp);
                        JGW_ReplaceStringW(strReadTemp,L"\r",L"");
                        JGW_ReplaceStringW(strReadTemp,L"\n",L"");
                        LogU(E_USER_DEFINE_HTMLFILEPATH,strReadTemp.c_str());
                        mvstrSaveHtmlFilePath.push_back(strReadTemp);
                        //strReadTemp.append(L"Html Log:");
                        LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,strReadTemp.c_str());
                        break;
                    }
                    
                }
            }
            while (pControlMsgBase->ReadControlMsg(strRead))
            {
                eTSEControlTypeEnum = OnHandleReadControlMsg(pControlMsgBase,strRead);
                if (TSEControlTestCompleteResultResultType == eTSEControlTypeEnum)
                {
                    S_MC_RSP_JSON smcRspJson;
                    if (!CTSE_MSLogJson::GetMCResponseJsonToJson(strRead,smcRspJson))
                    {
                        LogE(L"Get Test Complete Result Fail !!!");
                        return false;
                    }
                    pControlMsgBase->WriteControlMsg(CTSE_MSLogJson::GetMSBaseJson(TSEControlOperTestResultType));
                    return smcRspJson.Result;
                }
                if (TSEControlExitApplicationType == eTSEControlTypeEnum)
                {
                    mcApplicationPipe.CloseProcessPipe();
                    LogE(L"Test Application Exit !!!");
                    return false;
                }
            }
            Sleep(200);
        }
        //! 测试超时
        pControlMsgBase->CloseControlMsg();
        mcApplicationPipe.CloseProcessPipe();
        return false;
    }

    bool CTSE_QSPRAdaptTest::ReadControlRspMsgToExpectControlType(CTSE_ControlMsgBase* pControlMsgBase,TSEControlTypeEnum eExpectControlType,std::string& strRspMsg,int nTimeOutSec /* = 10 */)
    {
        jgw_timer jt;
        std::string strMsg;
        while(jt.elapsed() <= nTimeOutSec)
        {
            if (pControlMsgBase->ReadControlMsg(strMsg))
            {
                if (eExpectControlType == OnHandleReadControlMsg(pControlMsgBase,strMsg))
                {
                    strRspMsg = strMsg;
                    return true;
                }
            }
            Sleep(200);
        }
        return false;
    }

    TSEControlTypeEnum CTSE_QSPRAdaptTest::OnHandleReadControlMsg(CTSE_ControlMsgBase* pControlMsgBase,const std::string& strMsg)
    {
        TSEControlTypeEnum eControlType;
#ifdef DEBUG_TSE_MS_MC
        if (!CTSE_MSLogJson::GetMSControlType(eControlType,strMsg) /*|| !mcApplicationPipe.CheckProcessIsRun()*/)
#else
        if (!CTSE_MSLogJson::GetMSControlType(eControlType,strMsg) || !mcApplicationPipe.CheckProcessIsRun())
#endif

        {
            return TSEControlUnknownType;
        }

        switch (eControlType)
        {
        case TSEControlRegType:
            pControlMsgBase->WriteControlMsg(CTSE_MSLogJson::GetMSBaseJson(TSEControlRegResultType));
            break;
        case TSEControlXttResultType:
            pControlMsgBase->WriteControlMsg(CTSE_MSLogJson::GetMSBaseJson(TSEControlXttResultType));
            break;
        case TSEControlGlobalVariableResultType:
            pControlMsgBase->WriteControlMsg(CTSE_MSLogJson::GetMSBaseJson(TSEControlGlobalVariableResultType));
            break;
        case TSEControlOperTestResultType:
            pControlMsgBase->WriteControlMsg(CTSE_MSLogJson::GetMSBaseJson(TSEControlOperTestResultType));
            break;
        case TSEControlTestCompleteResultResultType:
            pControlMsgBase->WriteControlMsg(CTSE_MSLogJson::GetMSBaseJson(TSEControlTestCompleteResultResultType));
            break;
        }
        return eControlType;
    }

    void CTSE_QSPRAdaptTest::GetApplicationInfoMsg()
    {
        std::wstring strTemp;
        int readSize = 0;
        readSize = mcApplicationPipe.recv(strTemp);
        if ( readSize > 0 )
        {
            LogD(strTemp.c_str());
        }
    }

    void CTSE_QSPRAdaptTest::GetApplicationProcessErrorMsg()
    {
        std::wstring strTemp;
        int readSize = 0;
        jgw_timer jt;
        while(jt.elapsed() < 3)
        {
            readSize = mcApplicationPipe.recv(strTemp);
            if( -1 == readSize )
            {
                mcApplicationPipe.CloseProcessPipe();
                LogE(L"error: process closed!!!");
                return ;
            }
            else if ( readSize > 0 )
            {
                LogE(strTemp.c_str());
            }
            Sleep(50);
        }
    }

    bool CTSE_QSPRAdaptTest::TSE_Exit()
    {
        if (!mbTestResult)
        {
            std::wstring strReadTemp(mstrRecordTestItemLog.str());
            if (strReadTemp.length() > 2048)
            {
                strReadTemp = strReadTemp.substr(strReadTemp.length() - 2048);
            }
            LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,strReadTemp.c_str());
        }

        for (size_t i = 0;i < mvstrSaveHtmlFilePath.size();i ++)
        {
            std::wstring strFilePath = mvstrSaveHtmlFilePath[i];
            JGW_ReplaceStringW(strFilePath,L"File:///",L"");
            SaveHtmlLogging(mbTestResult,strFilePath);
            if (meShowTestLogType != E_NOT_SHOW_PASS_AND_FAIL_LOG)
            {
                ResolveHtmlFileInfo(strFilePath);
            }
        }
        return true;
    }

    void CTSE_QSPRAdaptTest::ResolveHtmlFileInfo(const std::wstring& strHtmlFilePath)
    {
        if (mbTestResult && meShowTestLogType != E_TP_SHOW_ALL_LOG)
        {
            return ;
        }

        bool bIsParsePassProject = false;
        if (meShowTestLogType == E_TF_SHOW_ALL_LOG || meShowTestLogType == E_TP_SHOW_ALL_LOG)
        {
            bIsParsePassProject = true;
        }

        CCJGW_ResolveQdartHtml resolveQdartHtml;
        resolveQdartHtml.LoadResolveQdartHtml(JGW_W2A(strHtmlFilePath),msHtmlQdartResolveConfig,bIsParsePassProject);
        if (bIsParsePassProject)
        {
            std::vector<std::string> vTemp;
            JGW_ParserStrA(resolveQdartHtml.GetTestResult().c_str(),"\n",vTemp);
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                LogI(JGW_A2W(vTemp[i],CP_UTF8).c_str());
            }
        }
        if (!mbTestResult)
        {
            std::vector<std::string> vTemp;
            JGW_ParserStrA(resolveQdartHtml.GetErrorTestResult().c_str(),"\n",vTemp);
            for (size_t i = 0;i < vTemp.size();i ++)
            {
                LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,JGW_A2W(vTemp[i],CP_UTF8).c_str());
            }
        }
    }

    void CTSE_QSPRAdaptTest::SaveHtmlLogging(bool bTestResult,const std::wstring& strLogFilePath)
    {
        if (mbSaveTestLogging) 
        {
            std::wstring strSN = GetGlobalEnvironment()->GetString(mstrSNEnvironment);
            mMoveTestLogging.MoveTestLogging(GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME).c_str(),strSN.c_str(),strLogFilePath.c_str(),bTestResult);
            //!if (!mbca) return ;
            //! 保存_Log_ConsoleApp文件
            std::wstring strLogConsoleApp(strLogFilePath);
            JGW_ReplaceStringW(strLogConsoleApp,L".html",L"");
            strLogConsoleApp += L"_Log_ConsoleApp.txt";
            if (JGW_FileExistsToFilePath(strLogConsoleApp.c_str()))
            {
                mMoveTestLogging.MoveTestLogging(GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME).c_str(),strSN.c_str(),strLogConsoleApp.c_str(),bTestResult);
            }    
        }
    }

}
