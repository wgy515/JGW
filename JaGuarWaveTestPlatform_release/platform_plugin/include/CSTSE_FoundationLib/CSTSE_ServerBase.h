#pragma once
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
#include <TSE_SequenceManagerPlugin/TSE_LogListener.h>
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include <string>
#include <map>
#include "CSTSE_Crypto.h"
#include <CSTSE_FoundationLib/CSTSE_ResponseBaseJson.h>

namespace JGW
{
    class CCSTSE_ServerBase
    {
    public:
        CCSTSE_ServerBase(void);
        ~CCSTSE_ServerBase(void);
    public:
        void AddLogListener(CTSE_LogListener* pLogListener);
        void AddTestStatusListener(CTSE_TestStatusListener* pTestStatusListener);
        void SetCryptoImpClass(CCSTSE_CryptoImp* pCryptoImp);
    public:
        //! 启动CS TSE客户端
        bool StartCSTSEClient(const std::wstring& strCSTSEClientExec,std::map<std::wstring,std::wstring>& strParam);
        //!
        bool ExecuteConfigViewTest();
        //!
        bool ExecuteAutoTest();
        //! 
        bool ExecuteAllTest();
        //! 当前CS TSE CLIENT EXEC是否已经启动
        bool IsStartCSTSEClient();
    private:
        //! 
        void HandleTSETestLogMsg(const std::string& strJson,CCSTSE_BaseJson& rsp);
        //! 
        bool GetMsgJsonToCSTSEAction(ECSTSE_ACTION CSTSEAction,std::string& strMsgJson,int time_out_sec = 60);
        //! bool CheckSendRecvJson(const std::string& strJson,int action,int time_out_sec = 10);
        //! bool CheckSendRecvJson(const std::string& strJson,int action,std::string& strErrorMsg,int time_out_sec = 10);
        bool SendMsgJsonToClient(const std::string& strJson,int time_out_sec = 10);
    private:
        //! 
        void LogJsonMsgToInvokeLogListener(const std::string& strLogJson);
        //! 
        void TestStatusJsonMsgToInvokeTestStatusListener(const std::string& strStatusJson);
        //! 
        void TestIndexChangeJsonMsgToInvokeTestStatusListener(const std::string& strTestIndexChangeJson);
    private:
        CTSE_LogListener* mpLogListener;
        CTSE_TestStatusListener* mpTestStatusListener;
        CCSTSE_CryptoImp* mpCryptoImp;
        CCJGW_ProcessPipe mCSTSEProcessPipe;
    };
}


