#include "StdAfx.h"
#include "json/TSEControlCmdJson.h"
#include "TSETestThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CTSETestThread::CTSETestThread(CTSETestInfoForward& tseTestInfoForward,bool& bStopTest): mTSETestInfoForward(tseTestInfoForward),mbStopTest(bStopTest),mpSequenceManageImpl(NULL),mpSequenceManageInterface(NULL),mbErrorStop(true)
    {
    }

    CTSETestThread::~CTSETestThread(void)
    {
        if (mpSequenceManageImpl) mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSETestFinalize();
        mTestSuitePocoClassLoader.UnloadTestSuiteModule();
    }

    void CTSETestThread::HandleTestControlCmds(std::vector<std::string>& vTestControlCmds)
    {
        for (std::vector<std::string>::iterator it = vTestControlCmds.begin();it != vTestControlCmds.end();
            ++ it )
        {
            CCJGW_FTM_Json ftmJson;
            if (!ftmJson.FromJsonToString(it[0]))
            {
                continue;
            }
            CTSEControlCmdJson controlCmdJson;
            controlCmdJson.FromJosn(ftmJson);
            TSETestControlEnum testControlEnum = controlCmdJson.GetTestControlCmdIndex();
            if (ControlCmd_AJE != controlCmdJson.ActionEnumID)
            {
                continue;
            }
            switch (testControlEnum)
            {
            case Init_TCE:
                InitTSE(controlCmdJson.Params);
                break;
            case Start_TCE:
                ExecuteTSE(controlCmdJson.Params);
                break;
            }
        }
        vTestControlCmds.clear();
    }

    bool CTSETestThread::InitTSE(std::map<std::string, std::string>& mapParams)
    {
        std::string strSuiteConfigPath = mapParams["TSE_SUITE_CONFIG_FILE_PATH"];
        std::string strDllPath = JGW_W2A_W(JGW_GetApplicationFolder());
        strDllPath += "TSE_SequenceManagerPlugin.dll";

        if (!mTestSuitePocoClassLoader.LoadTestSuiteModule(strDllPath))
        {
            mTSETestInfoForward.AddTestLogQueue(Error_TSE,JGW_GetFormatString("LoadTestSuiteModule Module Fail(%s,Error:%s)",strDllPath.c_str(),mTestSuitePocoClassLoader.GetLog().c_str()));
            return false;
        }

        mpSequenceManageInterface = mTestSuitePocoClassLoader.CreateTestSuitePtrClass(std::string("CTSE_SequenceManageExport"));
        if (!mpSequenceManageInterface)
        {
            mTSETestInfoForward.AddTestLogQueue(Error_TSE,mTestSuitePocoClassLoader.GetLog());
            return false;
        }

        mpSequenceManageImpl= mpSequenceManageInterface->GetSequenceManageImpl();
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(MULTIUNIT_TSE_TEST,0);
        mpSequenceManageImpl->SetOnLogListener(&mTSETestInfoForward);
        mpSequenceManageImpl->SetOnTestStatusListener(&mTSETestInfoForward);
        if (!mpSequenceManageImpl->GetSequenceTestImpl()->LoadTSESuiteConfig(strSuiteConfigPath.c_str()))
        {
            mTSETestInfoForward.AddTestLogQueue(Error_TSE,"LoadSequenceTest Config Fail");
            return false;
        }
        UpdateTSETestParam(mapParams);
        mpSequenceManageImpl->GetSequenceTestImpl()->InitTSETestConfig();
        return true;
    }

    bool CTSETestThread::UpdateTSETestParam(std::map<std::string, std::string>& mapParams)
    {
        for (std::map<std::string, std::string>::iterator it = mapParams.begin();
            it != mapParams.end();
            ++it)
        {
            mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutAnsiString(JGW_A2W(it->first),it->second);
        }

        return true;
    }

    bool CTSETestThread::ExecuteTSE(std::map<std::string,std::string>& mapParams)
    {
        UpdateTSETestParam(mapParams);
        if (mpSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAllSuiteTest(mbErrorStop))
        {
            mTSETestInfoForward.SendDataToTcpServer(CTSEServerJsonCommand::GetTestControlCmdJson(gstrTestGuids[PASS_TCE + 1]));
            return true;
        }
        else
        {
            mTSETestInfoForward.SendDataToTcpServer(CTSEServerJsonCommand::GetTestControlCmdJson(gstrTestGuids[Fail_TCE + 1]));
            return false;
        }
    }
}

