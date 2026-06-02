#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include <TSE_MSFoundationLib/CTSE_LogMsgBase.h>
#include <TSE_MSFoundationLib/CTSE_ControlMsgBase.h>
#include <TSE_MSFoundationLib/CTSE_MSFoundationDefine.h>
#include <JGW_TestLoggingPlugin/CJGW_MoveTestLogging.h>
#include "HtmlQdartResolveDefine.h"
#include <sstream>
namespace JGW
{
    enum QSPREngineType : int
    {
        QSPRGUIType = 0,
        QIAType
    };

    enum EHTML_SHOW_LOG_TYPE
    {
        //! 0 : 不显示PASS和FAIL LOG
        E_NOT_SHOW_PASS_AND_FAIL_LOG = 0,
        //! 1 : TSET FAIL 显示FAIL LOG 但不显示PASS LOG
        E_TF_SHOW_FAIL_NOT_PASS_LOG = 1,
        //! 2 : TSET FAIL 显示所有LOG
        E_TF_SHOW_ALL_LOG = 2,
        //! 3 : TSET PASS 显示所有LOG
        E_TP_SHOW_ALL_LOG = 3
    };
    enum QSPREngineMsgType
    {
        /// <summary>
        /// 全局环境变量
        /// </summary>
        GlobalVariableMsgType = 0,
        /// <summary>
        /// 测试Log
        /// </summary>
        DebugMsgType,
        /// <summary>
        /// 单个测试项目状态
        /// </summary>
        TestItemStatusMsgType,
        /// <summary>
        /// QSPR进程状态
        /// </summary>
        ProcessMsgType,
        /// <summary>
        /// 测试消息接收
        /// </summary>
        TestMessageReceivedType,
        /// <summary>
        /// 测试HTML文件
        /// </summary>
        TestResultFileType
    };
    //! 1、QSPR XTT路径
    //! 2、SN或者MAC等相关参数 采用KEY-VALUE属性匹配 修改XTT ENV等相关参数 (TSE_SN,TSE_24MAC,TSE_5MAC)(sn,mac_2G,mac)
    //! 3、
    class CTSE_QSPRAdaptTest : public CTSE_TestBase
    {
    public:
        CTSE_QSPRAdaptTest(void);
        ~CTSE_QSPRAdaptTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        //! Unknown Attribute
        virtual bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        //! 
        virtual bool TSE_Run(void);
        //!
        virtual bool TSE_Exit(void);
    private:
        //! 启动TSE MC进程
        bool StartApplicationProcess();
        //! 检测TSE MC是否已注册
        void* GetControlMsgBase();
        //! 接收TSE MC注册信息，并将收到信息发送给TSE MC
        bool HandleWithControlRegMsg(CTSE_ControlMsgBase* pControlMsgBase);
        //! 发送XTT JSON结果给TSE MC，并接收TSE MC回馈得结果，然后将收到信息OK结果回馈给TSEMC
        bool HandleWithControlXttMsg(CTSE_ControlMsgBase* pControlMsgBase);
        //! 发送全局变量 -> TSE MC,并接收TSE MC回馈结果，然后将收到信息OK结果回馈给TSE MC
        bool HandleWithControlGlobalVariableMsg(CTSE_ControlMsgBase* pControlMsgBase);
        //! 发送开始测试信息 -> TSE MC,并接收TSE MC反馈是否已经开始测试结果，然后将收到反馈得消息回馈给TSE MC
        bool HandleWithControlOperTestMsg(CTSE_ControlMsgBase* pControlMsgBase);
        //! 接收LOG信息以及接收测试结果MSG,并且将已经接收到测试结果信息反馈给TSE MC
        bool HandleWithLogAndControlMsg(CTSE_ControlMsgBase* pControlMsgBase);
        //!
        void GetApplicationInfoMsg();
        //! 
        void GetApplicationProcessErrorMsg();

        TSEControlTypeEnum OnHandleReadControlMsg(CTSE_ControlMsgBase* pControlMsgBase,const std::string& strMsg);
        //! void OnHandleLogMsg(const std::string& strMsg);
        //! 
        bool ReadControlRspMsgToExpectControlType(CTSE_ControlMsgBase* pControlMsgBase,TSEControlTypeEnum eExpectControlType,std::string& strRspMsg,int nTimeOutSec = 10);
        //!
        void SaveHtmlLogging(bool bTestResult,const std::wstring& strLogFilePath);
    private:
        void ResolveHtmlFileInfo(const std::wstring& strHtmlFilePath); 
    private:
        //! 是否保存测试log
        bool mbSaveTestLogging;
        //! 是否是首次启动应用程序
        bool mbFristStartApp;
        //! 校准测试结果文件
        bool mbTestResult;
        //! 
        QSPREngineType meQSPREngineType;
        //! 默认接收超时时间
        int mnDefaultRecvTimeOutSec;
        //! 测试超时
        int mnTestTimeOutSec;
        //! 0 : 不显示PASS和FAIL LOG
        //! 1 : TSET FAIL 显示FAIL LOG 但不显示PASS LOG
        //! 2 : TSET FAIL 显示所有LOG
        //! 3 : TSET PASS 显示所有LOG
        EHTML_SHOW_LOG_TYPE meShowTestLogType;

        std::string mstrTSEMSID;
        std::wstring mstrSNEnvironment;
        std::wstring mstrApplicationPathEnvironment;
        std::wstring mstrXttPathEnvironment;
        std::wstring mstrTSEGlobalVariableKey;
        std::wstring mstrXttGlobalVariableKey;
        std::wstring mstrHtmlResolveRuleFilePath;
        S_HTML_QDART_RESOLVE_CONFIG_INFO msHtmlQdartResolveConfig;
        CCJGW_MoveTestLogging mMoveTestLogging; //! 移动测试Log
        CCJGW_ProcessPipe mcApplicationPipe;
        std::vector<std::wstring> mvstrSaveHtmlFilePath;
        std::wostringstream mstrRecordTestItemLog;
    };
}