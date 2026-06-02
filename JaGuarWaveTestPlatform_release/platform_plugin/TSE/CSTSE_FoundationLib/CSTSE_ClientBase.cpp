#include "StdAfx.h"
#include <CSTSE_FoundationLib/CSTSE_ClientBase.h>
#include "CSTSE_CryptoDefault.h"
#include <iostream>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include "CSTSE_InitCommandParamJson.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include "CSTSE_RspEnverionmentJson.h"
#include "CSTSE_EnverionmentJson.h"
#include "CSTSE_TSETestJson.h"
#include "CSTSE_TestStatusChangeJson.h"
#include "CSTSE_TestIndexChangeJson.h"
#include "CSTSE_TestLogJson.h"

namespace JGW
{
    CCSTSE_ClientBase::CCSTSE_ClientBase(void) : mpCryptoImp(CCSTSE_CryptoDefault::GetCryptoDefault()),mpSequenceManageImpl(NULL),mptrTestSuiteManageInterface(NULL),mbErrorStop(true)
    {
    }


    CCSTSE_ClientBase::~CCSTSE_ClientBase(void)
    {
        if (mpCryptoImp)
        {
            delete mpCryptoImp;
            mpCryptoImp = NULL;
        }
        UninitializeTSE();
    }

    void CCSTSE_ClientBase::SetCryptoImpClass(CCSTSE_CryptoImp* pCryptoImp)
    {
        mpCryptoImp = pCryptoImp;
    }

    bool CCSTSE_ClientBase::InitCSTSEClient(const std::string& strJson)
    {
        CCSTSE_InitCommandParamJson initCommandParamJson;
        CCSTSE_ResponseBaseJson rsp(ECSTSE_ACTION_RESPONSE_COMMAND_PARAM_INIT_TSE);

        if (!initCommandParamJson.ParseInitCommandParamJson(strJson))
        {
            JGW_FormatString(rsp.ErrorMsg,"Parse Json(%s) Fail",strJson.c_str());
            SendServerPipe(rsp.GetResponseBaseJson());
            return rsp.Result;
        }
        //! init tse
        std::string strSuiteConfigPath =  initCommandParamJson.Params[TSE_SUITE_FILE_PATH_A];
        if (!JGW_PathFileExistsA(strSuiteConfigPath.c_str()))
        {
            JGW_FormatString(rsp.ErrorMsg,"Suite Xml(%s) File Not Exists Fail",strSuiteConfigPath.c_str());
            SendServerPipe(rsp.GetResponseBaseJson());
            return rsp.Result;
        }
        //! error stop
        mbErrorStop = true;
        if (initCommandParamJson.Params.end() != initCommandParamJson.Params.find(TSE_ERROR_STOP_A))
        {
            mbErrorStop = 1 == atoi(initCommandParamJson.Params[TSE_ERROR_STOP_A].c_str());
        }
        HANDLE hMainWnd = (HANDLE)atoi(initCommandParamJson.Params[TSE_GLOBAL_MAIN_HWND_A].c_str());
        if (!InitializeTSE(strSuiteConfigPath,initCommandParamJson.Params,hMainWnd,mbErrorStop))
        {
            SendServerPipe(rsp.GetResponseBaseJson());
            return rsp.Result;
        }

        rsp.Result = true;
        SendServerPipe(rsp.GetResponseBaseJson());
        return rsp.Result;
    }

    bool CCSTSE_ClientBase::UpdateGetEnverionment(const std::string& strJson)
    {
        CCSTSE_RspEnverionmentJson rsp;
        if (!mpSequenceManageImpl)
        {
            rsp.ErrorMsg = "Not Initialized TSE";
            SendServerPipe(rsp.GetResponseBaseJson());
            return rsp.Result;
        }

        CCSTSE_EnverionmentJson enverionmentJson;
        if (!enverionmentJson.ParseEnverionmentJson(strJson))
        {
            JGW_FormatString(rsp.ErrorMsg,"Parse Json(%s) Fail",strJson.c_str());
            SendServerPipe(rsp.GetResponseBaseJson());
            return rsp.Result;
        }

        if (enverionmentJson.IsUpdate)
        {
            for (std::map<std::string,std::string>::iterator it = enverionmentJson.EnvironmentVariables.begin();
                it != enverionmentJson.EnvironmentVariables.end();
                ++ it)
            {
                mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutAnsiString(JGW_A2W(it->first),it->second);
            }
        }
        else
        {
            for (std::map<std::string,std::string>::iterator it = enverionmentJson.EnvironmentVariables.begin();
                it != enverionmentJson.EnvironmentVariables.end();
                ++ it)
            {
                rsp.EnvironmentVariables[it->first] =  mpSequenceManageImpl->GetGlobalEnvironmentImpl()->GetAnsiString(JGW_A2W(it->first));
            }
        }
        rsp.Result = true;
        SendServerPipe(rsp.GetResponseBaseJson());
        return rsp.Result;
    }

    ECSTSE_ACTION CCSTSE_ClientBase::RecvCSTSEMsg(std::string& strRead,int time_out_sec /* = 5 */)
    {
        CCSTSE_BaseJson baseJson(ECSTSE_ACTION_Unknown);
        strRead = "";
        if (!RecvServerPipe(strRead,time_out_sec))
        {
            return ECSTSE_ACTION_Unknown;
        }

        if (!baseJson.ParseBaseJson(strRead))
        {
            return ECSTSE_ACTION_Unknown;
        }
        return baseJson.CSTSEAction;
    }

    bool CCSTSE_ClientBase::SendServerPipe(const std::string& strBuf)
    {
        std::string strWrite = mpCryptoImp->Encryption(strBuf);
        std::cout << strWrite << std::endl;
        return true;
    }

    bool CCSTSE_ClientBase::RecvServerPipe(std::string& strRead,int time_out_sec /* = 5 */)
    {
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);

        jgw_timer jt;
        DWORD bytesAvailable = 0; 
        while (jt.elapsed() <= time_out_sec)
        {
            PeekNamedPipe(hStdin,NULL,0,NULL,&bytesAvailable,NULL);
            if(bytesAvailable > 0)
            {
                std::string strTemp = "";
                getline(std::cin,strTemp);
                strRead = mpCryptoImp->Decryption(strTemp);
                //! std::cin.clear();
                return true;
            }
            Sleep(200);
        }
        return false;
    }

    void CCSTSE_ClientBase::OnDebugLog(const wchar_t* strDebugLog)
    {
        SendPipeLog(ECSTSE_TSE_TEST_LOG_TYPE_DEBUG,JGW_W2A(strDebugLog));
    }

    void CCSTSE_ClientBase::OnErrorLog(const wchar_t* strErrorLog)
    {
        SendPipeLog(ECSTSE_TSE_TEST_LOG_TYPE_ERROR,JGW_W2A(strErrorLog));
    }

    void CCSTSE_ClientBase::OnInfoLog(const wchar_t* strInfoLog)
    {
        SendPipeLog(ECSTSE_TSE_TEST_LOG_TYPE_INFO,JGW_W2A(strInfoLog));
    }

    void CCSTSE_ClientBase::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
    {
        SendPipeLog(ECSTSE_TSE_TEST_LOG_TYPE_USERDEFINE,JGW_W2A(strCustomLog),(E_USER_DEFINE_LOG_TYPE)id);
    }

    void CCSTSE_ClientBase::OnTestIndexChange(size_t index,int subIndex)
    {
        CCSTSE_TestIndexChangeJson testIndexChangeJson;
        testIndexChangeJson.Index = index;
        testIndexChangeJson.SubIndex = subIndex;
        SendServerPipe(testIndexChangeJson.GetTestIndexChangeJson());
    }

    void CCSTSE_ClientBase::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
    {
        CCSTSE_TestStatusChangeJson testStatusChangeJson;
        testStatusChangeJson.TestName = JGW_W2A(strTestName);
        testStatusChangeJson.TestStatus = eTestStatus;
        SendServerPipe(testStatusChangeJson.GetTestStatusChangeJson());
    }

    void CCSTSE_ClientBase::SendPipeLog(ECSTSE_TSE_TEST_LOG_TYPE logType,const std::string& strLog,E_USER_DEFINE_LOG_TYPE id /* = E_UNKNOW_USER_DEFINE */)
    {
        CCSTSE_TestLogJson testLogJson;
        testLogJson.LogType = logType;
        testLogJson.Log = strLog;
        testLogJson.UserDefineType = id;
        SendServerPipe(testLogJson.GetTestLogJson());
    }


    bool CCSTSE_ClientBase::InitializeTSE(const std::string& strSuiteConfigPath,const std::map<std::string, std::string>& params,HANDLE hMainHandle,bool bErrorStop /* = true */)
    {
        std::string strDllPath = JGW_W2A_W(JGW_GetApplicationFolder());
        strDllPath += "TSE_SequenceManagerPlugin.dll";

        if (!mTestSuitePocoClassLoader.LoadTestSuiteModule(strDllPath))
        {
            JGW_FormatString(mstrErrorMsg,"LoadTestSuiteModule Module Fail(%s)",strDllPath.c_str());
            return false;
        }

        mptrTestSuiteManageInterface = mTestSuitePocoClassLoader.CreateTestSuitePtrClass(std::string("CTSE_SequenceManageExport"));
        if (!mptrTestSuiteManageInterface)
        {
            mstrErrorMsg = "CreateTestSuitePtrClass Fail";
            return false;
        }

        mpSequenceManageImpl= mptrTestSuiteManageInterface->GetSequenceManageImpl();
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(MULTIUNIT_TSE_TEST,0);
        for (auto it = params.begin();
            it != params.end();
            ++it)
        {
            mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutAnsiString(JGW_A2W(it->first),it->second);
        }
        mpSequenceManageImpl->SetOnLogListener(this);
        mpSequenceManageImpl->SetOnTestStatusListener(this);
        if (!mpSequenceManageImpl->GetSequenceTestImpl()->LoadTSESuiteConfig(strSuiteConfigPath.c_str()))
        {
            mstrErrorMsg = "LoadSequenceTest Config Fail";
            return false;
        }
        mpSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(TSE_GLOBAL_MAIN_HWND,(int)hMainHandle);
        mpSequenceManageImpl->GetSequenceTestImpl()->InitTSETestConfig();
        return true;
    }

    void CCSTSE_ClientBase::UninitializeTSE()
    {
        if (mpSequenceManageImpl)
        {
            if (mptrTestSuiteManageInterface) mptrTestSuiteManageInterface->ReleaseSequenceManageImpl(mpSequenceManageImpl);
            mpSequenceManageImpl = NULL;
            //CoUninitialize();
        }
    }

    bool CCSTSE_ClientBase::TSETest(const std::string& strJson)
    {
        if (!mpSequenceManageImpl)
        {
            OnErrorLog(L"Not Initialized TSE");
            return false;
        }

        CCSTSE_TSETestJson tseTestJson;
        if (!tseTestJson.ParseTSETestJson(strJson))
        {
            JGW_FormatString(mstrErrorMsg,"Parse Json(%s) Fail",strJson.c_str());
            OnErrorLog(JGW_A2W(mstrErrorMsg).c_str());
            return false;
        }
        CTSE_SequenceTestImpl* seq = mpSequenceManageImpl->GetSequenceTestImpl();
        bool result = false;
        switch (tseTestJson.SuiteTestType)
        {
        case ECSTSE_TSET_TYPE_AUTO:
            result = seq->ExecTSEAutoTest();
            break;
        case ECSTSE_TSET_TYPE_ConfigView:
            result = seq->ExecTSEConfigTest();
            break;
        case ECSTSE_TSET_TYPE_NORMAL:
            if (-1 == tseTestJson.SuiteIndex)
            {
                result = seq->ExecTSEAllSuiteTest(mbErrorStop);
            }
            else
            {
                result = seq->ExecTSETestToIndex(tseTestJson.SuiteIndex);
            }
            break;
        }
        //! ªÿ¿°
        CCSTSE_ResponseBaseJson rsp(ECSTSE_ACTION_RESPONSE_TSE_TEST);
        rsp.Result = result;
        SendServerPipe(rsp.GetResponseBaseJson());

        return result;
    }
}

