#include "StdAfx.h"
#include "TSE_CopyDataQiaTest.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <sstream>
#include "CJGW_HtmlQdartResolveConfig.h"
#include "CJGW_ResolveQdartHtml.h"
#include "CJGW_CopyDataTestLogJson.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <sstream>
#include <JGW_TestLoggingPlugin/CJGW_SaveTestLog.h>


#define COPY_DATA_BUF_LENGTH 1024 * 2
namespace JGW
{
    /// <summary>
    /// PLUGIN LOG TYPE
    /// </summary>
    enum CopyDataLogTypePluginDomain
    {
        Debug_COPYDATA = 0,
        Info_COPYDATA = 1,
        Warn_COPYDATA = 2,
        Error_COPYDATA = 3,
        UserDefine_COPYDATA = 4,
        QIAWindowHanlde_COPYDATA = 5,
        CheckQIAIsRunning_COPYDATA = 6,
        StartQIAXttTree_COPYDATA = 7,
        StopQIAXttTree_COPYDATA = 8,
        QIAXttTreeTestComplete_COPYDATA = 9,
        QIATestResultMsg_COPYDATA = 10,
        QIATestProjectName_COPYDATA = 11
    };
    ///// <summary>
    ///// USER DEFINE ID TYPE
    ///// </summary>
    //enum CopyDataUserDefineIDType
    //{
    //    E_COPYDATA_USER_DEFINE_HTMLFILEPATH = 0,// html log 文件路径
    //    E_COPYDATA_TSE_SHOW_CONTENT_INFO = 1,// 测试list内容修改
    //    E_COPYDATA_TSE_SHOW_TEST_INFO_TO_RICHEDIT = 2,
    //    E_COPYDATA_TSE_SHOW_IDEL_BUSY_LIST_TEST_STATUS = 3,
    //    E_COPYDATA_TSE_SHOW_FAIL_LIST_TEST_STATUS = 4,
    //    E_COPYDATA_TSE_SHOW_PASS_LIST_TEST_STATUS = 5
    //};
    /*
    // 摘要:
    //     指定为 System.Diagnostics.Debug、System.Diagnostics.Trace 和 System.Diagnostics.TraceSwitch
    //     类输出的消息。
    public enum TraceLevel
    {
    // 摘要:
    //     不输出跟踪和调试消息。
    Off = 0,
    //
    // 摘要:
    //     输出错误处理消息。
    Error = 1,
    //
    // 摘要:
    //     输出警告和错误处理消息。
    Warning = 2,
    //
    // 摘要:
    //     输出信息性消息、警告和错误处理消息。
    Info = 3,
    //
    // 摘要:
    //     输出所有调试和跟踪消息。
    Verbose = 4,
    } Info,Debug,Error,Temp
    */
    CTSE_CopyDataQiaTest::CTSE_CopyDataQiaTest(void) : mnShowWindow(SW_HIDE),mbTestResult(false),mbSaveTestLogging(false),meShowTestLogType(E_TF_SHOW_FAIL_NOT_PASS_LOG),mnQiaWindowHandle(0),mbSaveRichEditTestLog(false),mbCheckWiFiCalbrationFirstGainPower(false)
    {
        mszCopyDataBuf = (char*)malloc(COPY_DATA_BUF_LENGTH);
    }


    CTSE_CopyDataQiaTest::~CTSE_CopyDataQiaTest(void)
    {
        free(mszCopyDataBuf);
        mszCopyDataBuf = NULL;
    }

    const wchar_t* CTSE_CopyDataQiaTest::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CopyDataQiaTest\",\
                \"TestName\": \"QIA Test\",\
                \"ShowWindow\": \"0:HIDE,1:NORMAL,2:MINIIZED,3:MAXIMIZED,4:ACTIVATE,5:SHOW,6:MINIMIZE,7:MINNOACTIVE,8:SHOWNA,9:RESTORE,10:SHOWDEFAULT,11:FORCEMINIMIZE default:0\",\
                \"QIAApplicationPath\": \"qia application path environment\",\
                \"XttTestTreeFilePath\": \"xtt file path environment\",\
                \"TSEGlobalVariableKey\": \"tse global variable key\",\
                \"XttGlobalVariableKey\": \"xtt global variable key\",\
                \"FilterWindowMessage\": \"filter window message : Info,Debug,Error,Temp\",\
                \"TestTimeOutSec\": \"test time out second\",\
                \"SNEnvironment\": \"sn environment\",\
                \"SaveTestLogging\": \"保存测试记录\",\
                \"ShowTestLogType\": \"0:不显示PASS和FAIL LOG,1:TSET FAIL 显示FAIL LOG 但不显示PASS LOG,2:TSET FAIL 显示所有LOG,3:TSET PASS 显示所有LOG\",\
                \"HtmlResolveRuleFilePath\": \"qdart html resolve rule file path\"\
                }";
    }

    bool CTSE_CopyDataQiaTest::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ShowWindow"))
        {
            mnShowWindow = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"QIAApplicationPath"))
        {
            mstrQiaExecFilePath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"XttTestTreeFilePath"))
        {
            mstrXttTestTreeFilePath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TSEGlobalVariableKey"))
        {
            mvTSEGlobalVariableKey.clear();
            JGW_ParserStrW(strParamValue,L",",mvTSEGlobalVariableKey);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"XttGlobalVariableKey"))
        {
            mvXttGlobalVariableKey.clear();
            JGW_ParserStrW(strParamValue,L",",mvXttGlobalVariableKey);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestTimeOutSec"))
        {
            mnTestTimeOutSec = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SNEnvironment"))
        {
            mstrSerialNumberEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SaveTestLogging"),strParamName))
        {
            mbSaveTestLogging = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("FilterWindowMessage"),strParamName))
        {
            mstrFilterWindowMessage = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("ShowTestLogType"),strParamName))
        {
            meShowTestLogType = (EHTML_SHOW_LOG_TYPE)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SaveRichEditTestLog"),strParamName))
        {
            mbSaveRichEditTestLog = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SaveRichEditTestLogFileEnvironment"),strParamName))
        {
            mstrSaveRichEditTestLogFileEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("CheckWiFiCalbrationFirstGainPower"),strParamName))
        {
            mbCheckWiFiCalbrationFirstGainPower = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("WiFiCalbrationFirstGainPowerEnvironmentCommand"),strParamName))
        {
            std::wstring strValue(strParamValue);
            GetEnvironmentCommandString(strValue);
            std::vector<std::wstring> vstrTemp,vstrPowerGain;
            msWiFiCalbrationFirstGainPower.clear();
            JGW_ParserStrW(strValue.c_str(),L";",vstrTemp);
            for (std::vector<std::wstring>::iterator it = vstrTemp.begin();it != vstrTemp.end();++it)
            {
                S_WiFiCalbrationFirstGainPower sWiFiCalbrationFirstGainPower;
                vstrPowerGain.clear();
                JGW_ParserStrW(it->c_str(),L",",vstrPowerGain);
                //! gain-lFreq-chainMask-MinPower
                if (4 == vstrPowerGain.size())
                {
                    sWiFiCalbrationFirstGainPower.mnGain = _ttoi(vstrPowerGain[0].c_str());
                    sWiFiCalbrationFirstGainPower.mnFreq = _ttoi(vstrPowerGain[1].c_str());
                    sWiFiCalbrationFirstGainPower.mnChainMask = _ttoi(vstrPowerGain[2].c_str());
                    sWiFiCalbrationFirstGainPower.mfPowerLowerLimit = static_cast<float>( _ttof(vstrPowerGain[3].c_str()));
                    sWiFiCalbrationFirstGainPower.mfTestPower = -99999;
                    msWiFiCalbrationFirstGainPower.push_back(sWiFiCalbrationFirstGainPower);
                }
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("HtmlResolveRuleFilePath"),strParamName))
        {
            mstrHtmlResolveRuleFilePath = JGW_RealativePathToAbsPath(strParamValue);
            CCJGW_HtmlQdartResolveConfig htmlQdartResolveConfig;
            htmlQdartResolveConfig.LoadHtmlQdartResolveConfig(mstrHtmlResolveRuleFilePath,msHtmlQdartResolveConfig);  
        }
        else
        {
            return false;
        }
        return true;
    }
#if 1
    void CTSE_CopyDataQiaTest::ClearCopyDataMessageQueue()
    {
        std::string strMessage = GetGlobalEnvironment()->GetAnsiCopyDataMessageQueue();
        while (!strMessage.empty())
        {
            strMessage = GetGlobalEnvironment()->GetAnsiCopyDataMessageQueue();
        }
    }

    bool CTSE_CopyDataQiaTest::StartQiaApplication()
    {
        GetGlobalEnvironment()->PutInt(TSE_QIA_WINDOW_HANLDE,0);
        int nMainWindowHandle = GetGlobalEnvironment()->GetInt(TSE_GLOBAL_MAIN_HWND);
        if ( !IsWindow((HWND)nMainWindowHandle) )
        {
            LogE_F(_T("当前窗口句柄不存在(HWND:%d)\n"),nMainWindowHandle);
            return false;
        }

        std::wstring strArg;
        std::wstring strWorkDir = JGW_GetPathOfFile(mstrQiaExecFilePath.c_str());
        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};

        JGW_FormatWString(strArg,L" \"%s\" %d 0 \"%s\"",mstrXttTestTreeFilePath.c_str(),nMainWindowHandle,mstrFilterWindowMessage.c_str());
        si.cb = sizeof(STARTUPINFO);
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;;
        si.wShowWindow = mnShowWindow;

        if (!CreateProcess (
            mstrQiaExecFilePath.c_str(),
            const_cast<wchar_t*>(strArg.c_str()),
            NULL, NULL,
            TRUE, 0,
            NULL,strWorkDir.c_str(),
            &si, &pi))
        {
            LogE_F(L"CTSE_CopyDataQiaTest::InitProcessPipe CreateProcess faile(%s)",JGW_GetSystemErrorString(GetLastError()).c_str());
            return false;
        }
        return true;
    }

    bool CTSE_CopyDataQiaTest::SendCopyDataMessageToQia(int logType,int userDefineID,const std::string& strLog)
    {
        int nQiaWindowHandle = GetGlobalEnvironment()->GetInt(TSE_QIA_WINDOW_HANLDE);
        if (NULL == nQiaWindowHandle)
        {
            return false;
        }
        memset(mszCopyDataBuf,0x00,COPY_DATA_BUF_LENGTH);
        COPYDATASTRUCT CopyData = {0};
        CopyData.dwData = 1;
#if 0
        sprintf_s(mszCopyDataBuf,COPY_DATA_BUF_LENGTH,"{\"LogType\":%d,\"UserDefineID\":%d,\"Message\":\"%s\"}",logType, userDefineID,strLog.c_str());
#else
        boost::property_tree::ptree jsonTree;
        jsonTree.put("LogType",logType);
        jsonTree.put("UserDefineID",userDefineID);
        jsonTree.put("Message",strLog);
        std::stringstream msg_ss;
        write_json(msg_ss,jsonTree);
        sprintf_s(mszCopyDataBuf,COPY_DATA_BUF_LENGTH,"%s",msg_ss.str().c_str());
#endif
        CopyData.cbData = strlen(mszCopyDataBuf)/* + 1*/;  
        mszCopyDataBuf[CopyData.cbData/* - 1*/] = '\0';
        CopyData.lpData = mszCopyDataBuf;

        SendMessage((HWND)nQiaWindowHandle, WM_COPYDATA, NULL, (LPARAM)&CopyData); 
        return true;
    }

    void CTSE_CopyDataQiaTest::ReadCopyDataMessageQueue()
    {
        std::string strMessage = GetGlobalEnvironment()->GetAnsiCopyDataMessageQueue();
        while (!strMessage.empty())
        {
            OnHandleCopyDataMessage(strMessage);
            strMessage = GetGlobalEnvironment()->GetAnsiCopyDataMessageQueue();
        }
    }

    void CTSE_CopyDataQiaTest::OnHandleCopyDataMessage(const std::string& strMessage)
    {
        CCJGW_FTM_Json ftmJson;
        if (ftmJson.FromJsonToString(strMessage))
        {
            CCJGW_CopyDataTestLogJson testLog;
            testLog.FromJosn(ftmJson);
            switch ((CopyDataLogTypePluginDomain)testLog.LogType)
            {
            case Debug_COPYDATA:
            case Info_COPYDATA:
                LogI(testLog.Message.c_str());
                break;
            case Warn_COPYDATA:
            case Error_COPYDATA:
                LogE(testLog.Message.c_str());
                //OnAppendRichEditTest(WPARAM(testLog.Message.c_str()),NULL);
                //PELOG4WW(testLog.Message.c_str());
                break;
            case QIAWindowHanlde_COPYDATA:
                //((CCTSEThroughputTestThread*)mpTestThread)->UpdateQIAWindowHandle(_ttoi(testLog.Message.c_str()));
                GetGlobalEnvironment()->PutInt(TSE_QIA_WINDOW_HANLDE,_ttoi(testLog.Message.c_str()));
                break;
            case CheckQIAIsRunning_COPYDATA:
                //((CCTSEThroughputTestThread*)mpTestThread)->UpdateCheckQIAIsRunningFlag(1);
                GetGlobalEnvironment()->PutInt(TSE_QIA_IS_RUNNING_FLAG,1);
                break;
            case QIAXttTreeTestComplete_COPYDATA:
                GetGlobalEnvironment()->PutBool(TSE_QIA_TEST_RESULT,1 == _ttoi(testLog.Message.c_str()));
                GetGlobalEnvironment()->PutInt(TSE_QIA_TEST_DONE,1);
                //((CCTSEThroughputTestThread*)mpTestThread)->UpdateQiaTestRessult(1 == _ttoi(testLog.Message.c_str()));
                break;
            case QIATestProjectName_COPYDATA:
                LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,testLog.Message.c_str());
                if (mbSaveRichEditTestLog || mbCheckWiFiCalbrationFirstGainPower) 
                    mstreamRichEditTestLog << testLog.Message << std::endl;
                break;
            case QIATestResultMsg_COPYDATA:
                LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,testLog.Message.c_str());
                if (mbSaveRichEditTestLog || mbCheckWiFiCalbrationFirstGainPower) 
                    mstreamRichEditTestLog << testLog.Message << std::endl;
                break;
            case UserDefine_COPYDATA:
                LogU(testLog.UserDefineID,testLog.Message.c_str());
                switch ((E_USER_DEFINE_LOG_TYPE)testLog.UserDefineID)
                {
                case E_USER_DEFINE_HTMLFILEPATH:
                    //LogU(E_USER_DEFINE_HTMLFILEPATH,testLog.Message.c_str());
                    LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,testLog.Message.c_str());
                    if (mbSaveRichEditTestLog || mbCheckWiFiCalbrationFirstGainPower) 
                        mstreamRichEditTestLog << testLog.Message << std::endl;
                    mvstrSaveHtmlFilePath.push_back(testLog.Message);
                    //PHLOG4WW(testLog.Message.c_str());
                    //OnAppendRichEditTest(WPARAM(testLog.Message.c_str()),NULL);
                    //((CCTSEThroughputTestThread*)mpTestThread)->AddHtmlTestLogging(testLog.Message);
                    break;
                case E_TSE_SHOW_TEST_INFO_TO_RICHEDIT:
                    if (mbCheckWiFiCalbrationFirstGainPower) 
                        mstreamRichEditTestLog << testLog.Message << std::endl;
                //case E_TSE_SHOW_CONTENT_INFO:
                //    LogU(E_TSE_SHOW_CONTENT_INFO,testLog.Message.c_str());
                //    //OnTSEShowTestContent(WPARAM(testLog.Message.c_str()),NULL);
                //    break;
                //case E_TSE_SHOW_TEST_INFO_TO_RICHEDIT:
                //    //LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,testLog.Message.c_str());
                //    //OnAppendRichEditTest(WPARAM(testLog.Message.c_str()),NULL);
                //    break;
                //case E_COPYDATA_TSE_SHOW_IDEL_BUSY_LIST_TEST_STATUS:
                //    //OnTestListMsg(WPARAM(testLog.Message.c_str()),BALCK);
                //    //OnTestListMsg(WPARAM(testLog.Message.c_str()),BULE);
                //    break;
                //case E_COPYDATA_TSE_SHOW_FAIL_LIST_TEST_STATUS:
                //    //OnTestListMsg(WPARAM(testLog.Message.c_str()),RED);
                //    break;
                //case E_COPYDATA_TSE_SHOW_PASS_LIST_TEST_STATUS:
                //    //OnTestListMsg(WPARAM(testLog.Message.c_str()),GREEN);
                //    break;
                }
                
                break;
            }
        }
    }

    bool CTSE_CopyDataQiaTest::CheckCopyDataGlobalEnvironment(const std::wstring& strKey,int nTimeOutSec /* = 5 */)
    {
        int value = GetGlobalEnvironment()->GetInt(strKey);
        for (int index = 0;index <= nTimeOutSec * 5 && 0 == value;index++)
        {
            Sleep(200);
            ReadCopyDataMessageQueue();
            value = GetGlobalEnvironment()->GetInt(strKey);
        }
        return 0 != value;
    }

    //! 1、QIA启动参数包括 XTT路径以及以及窗口句柄
    //！2、QIA启动后将自身窗口句柄信息回传到指定环境变量中
    //! 3、检测QIA窗口通信是否正常
    bool CTSE_CopyDataQiaTest::CheckQiaApplicationIsRunning()
    {
        std::wstring strQiaFileName = JGW_GetFileNameToFilePath(mstrQiaExecFilePath.c_str());
        if (!JGW_FileExistsToFilePath(mstrQiaExecFilePath.c_str()))
        {
            LogE_F(L"The file path(%s) does not exist",mstrQiaExecFilePath.c_str());
            return false;
        }
        //int nMainWindowHandle = GetGlobalEnvironment()->GetInt(TSE_GLOBAL_MAIN_HWND);
        mnQiaWindowHandle = GetGlobalEnvironment()->GetInt(TSE_QIA_WINDOW_HANLDE);
        if (NULL == mnQiaWindowHandle || !IsWindow((HWND)mnQiaWindowHandle))
        {
            if (!StartQiaApplication())
            {
                LogE(L"QIA Process startup failure");
                return false;
            }

            if (!CheckCopyDataGlobalEnvironment(TSE_QIA_WINDOW_HANLDE,10))
            {
                LogE("QIA Start the timeout");
                return false;
            }
            mnQiaWindowHandle = GetGlobalEnvironment()->GetInt(TSE_QIA_WINDOW_HANLDE);
        }
        // 检查QIA是否正在运行
        GetGlobalEnvironment()->PutInt(TSE_QIA_IS_RUNNING_FLAG,0);
        SendCopyDataMessageToQia(CheckQIAIsRunning_COPYDATA,0,"");
        //! 
        if (!CheckCopyDataGlobalEnvironment(TSE_QIA_IS_RUNNING_FLAG,30))
        {
            LogE(L"QIA Check Is running failure");
            return false;
        }
        return true;
    }

    bool CTSE_CopyDataQiaTest::TSE_Run()
    {
        mvstrSaveHtmlFilePath.clear();
        mbTestResult = false;
        mstreamRichEditTestLog.str(L"");
        mstreamRichEditTestLog.clear();
        ClearCopyDataMessageQueue();

        if (!CheckQiaApplicationIsRunning())
        {
            std::wstring strQiaFileName = JGW_GetFileNameToFilePath(mstrQiaExecFilePath.c_str());
            JGW_KillProcessToName(strQiaFileName);
            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%s : %s",L"kill process",strQiaFileName.c_str());
            return false;
        }

        std::string strXttGlobalVariable;
        if (!GetXttGlobalEnvironment(strXttGlobalVariable))
        {
            return false;
        }

        GetGlobalEnvironment()->PutInt(TSE_QIA_TEST_DONE,0);
        GetGlobalEnvironment()->PutInt(TSE_QIA_TEST_RESULT,0);
        if (!SendCopyDataMessageToQia(StartQIAXttTree_COPYDATA,0,strXttGlobalVariable))
        {
            LogE("start qia xtt tree fail");
            return false;
        }
        int testDone = GetGlobalEnvironment()->GetInt(TSE_QIA_TEST_DONE);
        for (int index = 0;index < mnTestTimeOutSec * 5;index++)
        {
            if (testDone == 1)
            {
                mbTestResult = GetGlobalEnvironment()->GetBool(TSE_QIA_TEST_RESULT);
                return mbTestResult;
            }
            Sleep(200);
            ReadCopyDataMessageQueue();
            testDone = GetGlobalEnvironment()->GetInt(TSE_QIA_TEST_DONE);
        }

        std::wstring strQiaFileName = JGW_GetFileNameToFilePath(mstrQiaExecFilePath.c_str());
        JGW_KillProcessToName(strQiaFileName);
        LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%s : %s",L"kill process",strQiaFileName.c_str());

        return false;
    }

    bool CTSE_CopyDataQiaTest::GetXttGlobalEnvironment(std::string& strXttGlobalVariable)
    {
        if (mvXttGlobalVariableKey.size() != mvTSEGlobalVariableKey.size())
        {
            LogE(L"GlobalVariable Param Fail");
            return false;
        }

        std::wostringstream strGlobalVariables;
        for (size_t i = 0;i < mvTSEGlobalVariableKey.size();i ++)
        {
            //strGlobalVariables += JGW_GetFormatWString();
            strGlobalVariables << mvXttGlobalVariableKey[i] << L"=" << GetGlobalEnvironment()->GetString(mvTSEGlobalVariableKey[i]);
            if (i != mvTSEGlobalVariableKey.size() - 1)
            {
                strGlobalVariables << L"&&&&&&&&";
            }
        }
        strXttGlobalVariable = JGW_W2A(strGlobalVariables.str(),CP_UTF8);
        return true;
    }

#else
   

    bool CTSE_CopyDataQiaTest::TSE_Run()
    {
        mbTestResult = false;
        GetGlobalEnvironment()->PutString(TSE_QIA_HTML_TEST_FILE,L"");
        if (!CheckQiaApplicationIsRunning())
        {
            std::wstring strQiaFileName = JGW_GetFileNameToFilePath(mstrQiaExecFilePath.c_str());
            JGW_KillProcessToName(strQiaFileName);
            return false;
        }

        std::string strXttGlobalVariable;
        if (!GetXttGlobalEnvironment(strXttGlobalVariable))
        {
            return false;
        }
        GetGlobalEnvironment()->PutInt(TSE_QIA_TEST_DONE,0);
        GetGlobalEnvironment()->PutInt(TSE_QIA_TEST_RESULT,0);
        if (!SendCopyDataMessageToQia(StartQIAXttTree_COPYDATA,0,strXttGlobalVariable))
        {
            LogE("start qia xtt tree fail");
            return false;
        }
        int testDone = GetGlobalEnvironment()->GetInt(TSE_QIA_TEST_DONE);
        for (int index = 0;index < mnTestTimeOutSec * 5;index++)
        {
            if (testDone == 1)
            {
                mbTestResult = GetGlobalEnvironment()->GetBool(TSE_QIA_TEST_RESULT);
                return mbTestResult;
            }
            Sleep(200);
            testDone = GetGlobalEnvironment()->GetInt(TSE_QIA_TEST_DONE);
        }
        std::wstring strQiaFileName = JGW_GetFileNameToFilePath(mstrQiaExecFilePath.c_str());
        JGW_KillProcessToName(strQiaFileName);
        return false;
    }

    bool CTSE_CopyDataQiaTest::StartQiaApplication()
    {
        GetGlobalEnvironment()->PutInt(TSE_QIA_WINDOW_HANLDE,0);
        int nMainWindowHandle = GetGlobalEnvironment()->GetInt(TSE_GLOBAL_MAIN_HWND);
        if ( !IsWindow((HWND)nMainWindowHandle) )
        {
            LogE_F(_T("当前窗口句柄不存在(HWND:%d)\n"),nMainWindowHandle);
            return false;
        }
        std::wstring strArg;
        std::wstring strWorkDir = JGW_GetPathOfFile(mstrQiaExecFilePath.c_str());
        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};

        JGW_FormatWString(strArg,L" \"%s\" %d",mstrXttTestTreeFilePath.c_str(),nMainWindowHandle);
        si.cb = sizeof(STARTUPINFO);
        si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;;
        si.wShowWindow = mnShowWindow;

        if (!CreateProcess (
            mstrQiaExecFilePath.c_str(),
            const_cast<wchar_t*>(strArg.c_str()),
            NULL, NULL,
            TRUE, 0,
            NULL,strWorkDir.c_str(),
            &si, &pi))
        {
            LogE_F(L"CTSE_CopyDataQiaTest::InitProcessPipe CreateProcess faile(%s)",JGW_GetSystemErrorString(GetLastError()).c_str());
            return false;
        }
        return true;
    }

    bool CTSE_CopyDataQiaTest::SendCopyDataMessageToQia(int logType,int userDefineID,const std::string& strLog)
    {
        int nQiaWindowHandle = GetGlobalEnvironment()->GetInt(TSE_QIA_WINDOW_HANLDE);
        if (NULL == nQiaWindowHandle)
        {
            return false;
        }
        memset(mszCopyDataBuf,0x00,COPY_DATA_BUF_LENGTH);
        COPYDATASTRUCT CopyData = {0};
        CopyData.dwData = 1;

        sprintf_s(mszCopyDataBuf,COPY_DATA_BUF_LENGTH,"{\"LogType\":%d,\"UserDefineID\":%d,\"Message\":\"%s\"}",logType, userDefineID,strLog.c_str());
        CopyData.cbData = strlen(mszCopyDataBuf)/* + 1*/;  
        mszCopyDataBuf[CopyData.cbData/* - 1*/] = '\0';
        CopyData.lpData = mszCopyDataBuf;
        GetGlobalEnvironment()->PutInt(TSE_QIA_IS_RUNNING_FLAG,0);
        SendMessage((HWND)nQiaWindowHandle, WM_COPYDATA, NULL, (LPARAM)&CopyData); 
        return true;
    }

    //! 1、QIA启动参数包括 XTT路径以及以及窗口句柄
    //！2、QIA启动后将自身窗口句柄信息回传到指定环境变量中
    //! 3、检测QIA窗口通信是否正常
    bool CTSE_CopyDataQiaTest::CheckQiaApplicationIsRunning()
    {
        std::wstring strQiaFileName = JGW_GetFileNameToFilePath(mstrQiaExecFilePath.c_str());
        if (!JGW_FileExistsToFilePath(mstrQiaExecFilePath.c_str()))
        {
            LogE_F(L"The file path(%s) does not exist",mstrQiaExecFilePath.c_str());
            return false;
        }

        int nQiaWindowHandle = GetGlobalEnvironment()->GetInt(TSE_QIA_WINDOW_HANLDE);
        if (NULL == nQiaWindowHandle || !IsWindow((HWND)nQiaWindowHandle))
        {
            if (!StartQiaApplication())
            {
                LogE(L"QIA Process startup failure");
                return false;
            }
            nQiaWindowHandle = GetGlobalEnvironment()->GetInt(TSE_QIA_WINDOW_HANLDE);
            for (int index = 0;index < 10 * 5;index++)
            {
                if (0 != nQiaWindowHandle)
                {
                    break;
                }
                Sleep(200);
                nQiaWindowHandle = GetGlobalEnvironment()->GetInt(TSE_QIA_WINDOW_HANLDE);
            }
            if (0 == nQiaWindowHandle)
            {
                return false;
            }
        }
        // 发送信息检测
        //char szSendBuf[100] = {0};  
        //COPYDATASTRUCT CopyData = {0};
        //CopyData.dwData = 0;  
        //CopyData.cbData = strlen(szSendBuf);  
        //szSendBuf[CopyData.cbData - 1] = '\0';
        //sprintf(szSendBuf,"{\"LogType\":%d,\"UserDefineID\":%d,\"Message\":\"\"}",CheckQIAIsRunning_COPYDATA, 0);
        //CopyData.lpData = szSendBuf;
        GetGlobalEnvironment()->PutInt(TSE_QIA_IS_RUNNING_FLAG,0);
        SendCopyDataMessageToQia(CheckQIAIsRunning_COPYDATA,0,"");
        //SendMessage((HWND)nQiaWindowHandle, WM_COPYDATA, NULL, (LPARAM)&CopyData); 
        int flag = GetGlobalEnvironment()->GetInt(TSE_QIA_IS_RUNNING_FLAG);
        for (int index = 0;index < 10 * 5;index++)
        {
            if (0 != flag)
            {
                return true;
            }
            Sleep(200);
            flag = GetGlobalEnvironment()->GetInt(TSE_QIA_IS_RUNNING_FLAG);
        }
        LogE(L"QIA Check Is running failure");
        return false;
    }
#endif
    bool CTSE_CopyDataQiaTest::TSE_Exit()
    {
        if (mbSaveRichEditTestLog)
        {
            std::wstring strTestLogFilePath,strSaveDownloadLogFolder;
            std::wstring strSN = GetGlobalEnvironment()->GetString(mstrSerialNumberEnvironment);
            JGW_FormatWString(strSaveDownloadLogFolder,L"%s\\%s",JGW_GetApplicationFolder(),GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME).c_str());
            CCJGW_SaveTestLog::SaveTestLogGetFilePath(strSaveDownloadLogFolder,strSN,true,mstreamRichEditTestLog.str(),strTestLogFilePath);
            GetGlobalEnvironment()->PutString(mstrSaveRichEditTestLogFileEnvironment,strTestLogFilePath);
        }

        if (mbCheckWiFiCalbrationFirstGainPower && mbTestResult)
        {
            //! 手动加载配置
            //{
            //    mstreamRichEditTestLog.imbue(std::locale(""));
            //    std::ifstream inputFile;
            //    inputFile.imbue(std::locale(""));
            //    inputFile.open("D:\\ftp\\DHW2412459\\2024-11-23\\TestLog_TEST-DL-E4-01_172.17.109.153_1732339701\\wifi_calibration\\wifi_calibration_1.xml\\2024-11-23\\P_JR100PR2445007_6279656.log");

            //    const int LINE_LENGTH = 1024 * 4; 
            //    char str[LINE_LENGTH] = {0}; 

            //    while (inputFile.getline(str,LINE_LENGTH,'\r'))
            //    {
            //        mstreamRichEditTestLog << JGW_A2W(str,CP_UTF8);
            //    }
            //}
            std::wstring strTestLog(mstreamRichEditTestLog.str()),strFindValue;
            mstreamRichEditTestLog.str(L"");
            mstreamRichEditTestLog.clear();

            for (std::vector<S_WiFiCalbrationFirstGainPower>::iterator it = msWiFiCalbrationFirstGainPower.begin();it != msWiFiCalbrationFirstGainPower.end();++it)
            {
                JGW_FormatWString(strFindValue,L"gain: %d   lFreq: %d    chainMask: %d",it->mnGain,it->mnFreq,it->mnChainMask);
                size_t pos = strTestLog.find(strFindValue);
                const size_t mes_power_len = strlen("MeasPwr:1.63   ") + 5;
                if (std::wstring::npos != pos && pos >= mes_power_len)
                {
                    std::wstring strValue = strTestLog.substr(pos - mes_power_len,mes_power_len);
                    pos = strValue.find(L"MeasPwr:");
                    if (pos != std::wstring::npos)
                    {
                        const wchar_t* p = wcsstr(strValue.c_str(),L"MeasPwr:");
                        it->mfTestPower = static_cast<float>(_ttof(p + wcslen(L"MeasPwr:")));
                        if (it->mfTestPower > it->mfPowerLowerLimit)
                        {
                            LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Chain%u Gain : %d,Freq : %u Power(%.2f)",it->mnChainMask,it->mnGain,it->mnFreq,it->mfTestPower);
                            continue;
                        }
                    }
                }
                LogE_F(L"Chain%d Gain : %d,Freq : %d Power(%.2f) exception ERROR",it->mnChainMask,it->mnGain,it->mnFreq,it->mfTestPower);
                it->mfTestPower = -99999;
                return false;
            }
        }
        //if (!mbTestResult)
        //{
        //    std::wstring strReadTemp(mstrRecordTestItemLog.str());
        //    if (strReadTemp.length() > 2048)
        //    {
        //        strReadTemp = strReadTemp.substr(strReadTemp.length() - 2048);
        //    }
        //    LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,strReadTemp.c_str());
        //}
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

    void CTSE_CopyDataQiaTest::ResolveHtmlFileInfo(const std::wstring& strHtmlFilePath)
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

    void CTSE_CopyDataQiaTest::SaveHtmlLogging(bool bTestResult,const std::wstring& strLogFilePath)
    {
        if (mbSaveTestLogging) 
        {
            std::wstring strSN = GetGlobalEnvironment()->GetString(mstrSerialNumberEnvironment);
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

    //void CTSE_CopyDataQiaTest::StartTFTP32Process()
    //{
    //    DWORD processID = JGW_GetProcessID2Name(L"tftpd32.exe");
    //    if (processID != -1)
    //    {
    //        mbCheckTFPT32ProcessRuning = true;
    //        return ;
    //    }

    //    std::wstring strTFTP32ExecFilePath = JGW_RealativePathToAbsPath(L"Tftpd32\\tftpd32.exe");
    //    if (JGW_PathFileExists(strTFTP32ExecFilePath.c_str()))
    //    {
    //        JGW_CreateProcess(strTFTP32ExecFilePath.c_str(),JGW_GetPathOfFile(strTFTP32ExecFilePath.c_str()).c_str());
    //    }
    //    mbCheckTFPT32ProcessRuning = true;
    //}

}
