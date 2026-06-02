#pragma once
#include "CSTSE_Crypto.h"
#include <string>
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageInterface.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
#include <TSE_SequenceManagerPlugin/TSE_LogListener.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include <TSE_SequenceTestLib/TSE_TestSequenceExec.h>
#include "CSTSE_ActionDefine.h"
#define DEFAULT_CSTSE_MSG_TIMEOUT_SEC 5

namespace JGW
{
    class CCSTSE_ClientBase : public CTSE_LogListener,public CTSE_TestStatusListener
    {
    public:
        CCSTSE_ClientBase(void);
        virtual ~CCSTSE_ClientBase(void);
    public:
        void SetCryptoImpClass(CCSTSE_CryptoImp* pCryptoImp);
    public:
        //! 初始化CS TSE
        bool InitCSTSEClient(const std::string& strJson);
        //! 更新获取全局环境变量
        bool UpdateGetEnverionment(const std::string& strJson);
        //! 
        bool TSETest(const std::string& strJson);
        //! 处理CS TSE MSG
        //! bool HandleCSTSEMsg();
        //! 
        ECSTSE_ACTION RecvCSTSEMsg(std::string& strRead,int time_out_sec = DEFAULT_CSTSE_MSG_TIMEOUT_SEC);
    private:
        //! send control command
        bool SendServerPipe(const std::string& strBuf);
        //! read 
        bool RecvServerPipe(std::string& strRead,int time_out_sec = DEFAULT_CSTSE_MSG_TIMEOUT_SEC);
    private:
        //! CTSE_LogListener
        void OnDebugLog(const wchar_t* strDebugLog);

        void OnInfoLog(const wchar_t* strInfoLog);

        void OnErrorLog(const wchar_t* strErrorLog);

        void OnUserDefinedLog(int id,const wchar_t* strCustomLog);
        //! CTSE_TestStatusListener
        void OnTestIndexChange(size_t index,int subIndex);
        //! 
        void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
    private:
        void SendPipeLog(ECSTSE_TSE_TEST_LOG_TYPE logType,const std::string& strLog,E_USER_DEFINE_LOG_TYPE id = E_UNKNOW_USER_DEFINE);
    private:
        bool InitializeTSE(const std::string& strSuiteConfigPath,const std::map<std::string,std::string>& params,HANDLE hMainHandle,bool bErrorStop = true);
        void UninitializeTSE();
    private:
        bool mbErrorStop;
        std::string mstrErrorMsg;
        CCSTSE_CryptoImp* mpCryptoImp;
        CTSE_SequenceManageImpl* mpSequenceManageImpl;
        CTSE_SequenceManageInterface* mptrTestSuiteManageInterface;
        CTSE_SequencePocoClassLoader<CTSE_SequenceManageInterface> mTestSuitePocoClassLoader;
    };
}