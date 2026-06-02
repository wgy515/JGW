#pragma once
#include "ITSEControlCmdForward.h"
#include "IWriteInfoServerInterface.h"
#include "TSETestMsgQueue.h"
#include "json/TSEServerJsonCommand.h"
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
#include <TSE_SequenceManagerPlugin/TSE_LogListener.h>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>


namespace JGW
{
    class CTSETestInfoForward : public CTSE_LogListener,public CTSE_TestStatusListener,public ITSEControlCmdForward
    {
    public:
        CTSETestInfoForward(bool& bStopTSETest);
        ~CTSETestInfoForward(void);
    public:
        //! 
        void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
        //! 
        void OnTestIndexChange(size_t index,int subIndex = -1);
        //! 调试信息 -- 主要用来调试使用 不会在界面上显示
        void OnDebugLog(const wchar_t* strDebugLog);
        //! 错误信息 -- 显示错误的相关内容
        void OnErrorLog(const wchar_t* strErrorLog);
        //! 测试信息 -- 显示测试相关信息  在界面上显示
        void OnInfoLog(const wchar_t* strInfoLog);
        //! 自定义LOG信息
        void OnUserDefinedLog(int id,const wchar_t* strCustomLog);
        //! 接收到控制命令
        void OnForwardRecvControlCmdInfo(const std::string& strData);
    public:
        //! 
        bool GetRecvControlCmdInfos(std::vector<std::string>& vTestControlCmds);
        //! 
        void SetWriteInfoServerInterfaceInstance(IWriteInfoServerInterface* pWriteInfoServerInterface);
        //!
        void SendDataToTcpServer(const std::string& strData);
    public:
        void AddTestLogQueue(TSELogTypeEnum tseLogTypeEnum,const std::string& strLog,E_USER_DEFINE_LOG_TYPE userDefineID = E_UNKNOW_USER_DEFINE);
        //! 转发TSE Test Info
        void OnForWardTSETestInfo();
    private:
        void HandleRecvControlCmdInfo(std::queue<std::string>* pRecvMsgQueue);
    private:
        bool& mbStopTSETest;
        std::vector<std::string> mvTestControlCmds;
        CCJGW_CriticalSectionLock mCriticalSectionLock;
        IWriteInfoServerInterface* mpIWriteInfoServerInterface;
        CTSETestMsgQueue mTSETestMsgQueue;
        CTSETestMsgQueue mTSERecvTestMsgQueue;
    };


}

