#pragma once
#include "TSETestInfoForward.h"
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageInterface.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
#include <TSE_SequenceManagerPlugin/TSE_LogListener.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
namespace JGW
{
    class CTSETestThread
    {
    public:
        CTSETestThread(CTSETestInfoForward& tseTestInfoForward,bool& bStopTest);
        ~CTSETestThread(void);
    public:
        void HandleTestControlCmds(std::vector<std::string>& vTestControlCmds);
    private:
        bool InitTSE(std::map<std::string,std::string>& mapParams);
        bool ExecuteTSE(std::map<std::string,std::string>& mapParams);
        bool UpdateTSETestParam(std::map<std::string, std::string>& mapParams);
    private:
        bool& mbStopTest;
        bool mbErrorStop;
        CTSETestInfoForward& mTSETestInfoForward;
        CTSE_SequenceManageImpl* mpSequenceManageImpl;
        CTSE_SequenceManageInterface* mpSequenceManageInterface;
        CTSE_SequencePocoClassLoader<CTSE_SequenceManageInterface> mTestSuitePocoClassLoader;
    };
}


