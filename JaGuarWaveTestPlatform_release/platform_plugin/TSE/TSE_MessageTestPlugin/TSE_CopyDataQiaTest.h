#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "TSE_QSPRAdaptTest.h"
#include <sstream>
namespace JGW
{
    class CTSE_CopyDataQiaTest : public CTSE_TestBase
    {
    public:
        CTSE_CopyDataQiaTest(void);
        ~CTSE_CopyDataQiaTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Exit();
    private:
        //! 清空WM_COPTDATA消息队列
        void ClearCopyDataMessageQueue();
        //! 处理WM_COPTDATA消息队列
        void OnHandleCopyDataMessage(const std::string& strMessage);
        //! 读取WM_COPTDATA消息队列
        void ReadCopyDataMessageQueue();
        //! 检查WM_COPTDATA消息队列，全局环境变量
        bool CheckCopyDataGlobalEnvironment(const std::wstring& strKey,int nTimeOutSec = 5);

        bool StartQiaApplication();
        bool CheckQiaApplicationIsRunning();
        bool SendCopyDataMessageToQia(int logType,int userDefineID,const std::string& strLog);
        bool GetXttGlobalEnvironment(std::string& strXttGlobalVariable);
    private:
        void ResolveHtmlFileInfo(const std::wstring& strHtmlFilePath);
        void SaveHtmlLogging(bool bTestResult,const std::wstring& strLogFilePath);
    private:
        //! 是否检查WIFI校准首个增益功率 gain-lFreq-chainMask
        bool mbCheckWiFiCalbrationFirstGainPower;

        bool mbSaveRichEditTestLog;
        bool mbTestResult;
        bool mbSaveTestLogging;
        //! bool mbCheckQutsProcessRuning;
        //! 0 : 不显示PASS和FAIL LOG
        //! 1 : TSET FAIL 显示FAIL LOG 但不显示PASS LOG
        //! 2 : TSET FAIL 显示所有LOG
        //! 3 : TSET PASS 显示所有LOG
        EHTML_SHOW_LOG_TYPE meShowTestLogType;
        /*
        #define SW_HIDE             0
        #define SW_SHOWNORMAL       1
        #define SW_NORMAL           1
        #define SW_SHOWMINIMIZED    2
        #define SW_SHOWMAXIMIZED    3
        #define SW_MAXIMIZE         3
        #define SW_SHOWNOACTIVATE   4
        #define SW_SHOW             5
        #define SW_MINIMIZE         6
        #define SW_SHOWMINNOACTIVE  7
        #define SW_SHOWNA           8
        #define SW_RESTORE          9
        #define SW_SHOWDEFAULT      10
        #define SW_FORCEMINIMIZE    11
        #define SW_MAX              11
        */
        char* mszCopyDataBuf;
        int mnShowWindow;
        int mnTestTimeOutSec;
        int mnQiaWindowHandle;
        std::wstring mstrXttTestTreeFilePath;
        std::wstring mstrQiaExecFilePath;
        //std::wstring mstrTSEGlobalVariableKey;
        //std::wstring mstrXttGlobalVariableKey;
        std::wstring mstrSerialNumberEnvironment;
        std::wstring mstrSaveRichEditTestLogFileEnvironment;
        //! 过滤窗口消息
        std::wstring mstrFilterWindowMessage;
        //! 
        std::wstring mstrHtmlResolveRuleFilePath;
        //! 10,5180,1,-4;10,5180,2,-4;10,5180,4,-4;10,5180,8,-4;2,2412,1,-4;2,2412,2,-4;2,2412,4,-4;2,2412,8,-4;
        std::wstring mstrWiFiCalbrationFirstGainPowerEnvironmentCommand;
        S_HTML_QDART_RESOLVE_CONFIG_INFO msHtmlQdartResolveConfig;
        CCJGW_MoveTestLogging mMoveTestLogging; //! 移动测试Log
        std::wostringstream mstreamRichEditTestLog;
        std::vector<std::wstring> mvTSEGlobalVariableKey;
        std::vector<std::wstring> mvXttGlobalVariableKey;
        std::vector<std::wstring> mvstrSaveHtmlFilePath;
        //! 10,5180,1,-4;10,5180,2,-4;10,5180,4,-4;10,5180,8,-4;2,2412,1,-4;2,2412,2,-4;2,2412,4,-4;2,2412,8,-4;
        typedef struct
        {
            int mnGain;
            unsigned int mnFreq;
            unsigned int mnChainMask;
            float mfTestPower;
            float mfPowerLowerLimit;
        }S_WiFiCalbrationFirstGainPower;
        std::vector<S_WiFiCalbrationFirstGainPower> msWiFiCalbrationFirstGainPower;
    };
}
