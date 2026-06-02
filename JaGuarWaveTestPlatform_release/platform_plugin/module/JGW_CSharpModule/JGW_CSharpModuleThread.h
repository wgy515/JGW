#pragma once
#include <JGW_CSharpModule/CJGW_CSharpCommunicationBase.h>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "CJGW_FTM_Json.h"

namespace JGW
{
    class CJGW_CSharpModuleThread : public CCMessageThread
    {
    public:
        CJGW_CSharpModuleThread(void);
        ~CJGW_CSharpModuleThread(void);
    public:
        bool StartThread();
    private:
        virtual void RunThread();
    private:
        void HandleRequest(const std::string& strRecv);
    private:
        void HandleMESCheckSNStatusReq(CCJGW_FTM_Json& ftmJson);
        void HandleMESGetIMEIInfoReq(CCJGW_FTM_Json& ftmJson);
        void HandleMESGetSNInfoReq(CCJGW_FTM_Json& ftmJson);
        void HandleMESNextIMEIStationReq(CCJGW_FTM_Json& ftmJson);
        void HandleMESNextSNStationReq(CCJGW_FTM_Json& ftmJson);
        void HandleMESSaveTestRecordsReq(CCJGW_FTM_Json& ftmJson);
        void HandleTSECreateSuiteProjectReq(CCJGW_FTM_Json& ftmJson);
        void HandleTSEGetEnvironmentValueReq(CCJGW_FTM_Json& ftmJson);
        void HandleTSESetEnviromentValueReq(CCJGW_FTM_Json& ftmJson);
        void HandleTSERunSuiteProjectReq(CCJGW_FTM_Json& ftmJson);
        void HandleUpdateLogReq(CCJGW_FTM_Json& ftmJson);
    private:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        char* mszRecvBuf;
        CJGW_CSharpCommunicationBase* mpCommunicationBase;
    };
}
