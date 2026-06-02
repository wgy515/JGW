#include "StdAfx.h"
#include <CSTSE_FoundationLib/CSTSE_ServerBase.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "CSTSE_CryptoDefault.h"
#include "CSTSE_InitCommandParamJson.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>

#include "CSTSE_LogListenerEmpty.h"
#include "CSTSE_TestStatusListenerEmpty.h"
#include "CSTSE_TestLogJson.h"
#include "CSTSE_TestStatusChangeJson.h"
#include "CSTSE_TestIndexChangeJson.h"
#include "CSTSE_TSETestJson.h"

namespace JGW
{
    CCSTSE_ServerBase::CCSTSE_ServerBase(void) : mpTestStatusListener(CCSTSE_TestStatusListenerEmpty::GetTestStatusListenerEmpty()),mpLogListener(CCSTSE_LogListenerEmpty::GetLogListenerEmpty()),mpCryptoImp(CCSTSE_CryptoDefault::GetCryptoDefault())
    {

    }

    CCSTSE_ServerBase::~CCSTSE_ServerBase(void)
    {

    }

    void CCSTSE_ServerBase::AddLogListener(CTSE_LogListener* pLogListener)
    {
        mpLogListener = pLogListener;
    }

    void CCSTSE_ServerBase::AddTestStatusListener(CTSE_TestStatusListener* pTestStatusListener)
    {
        mpTestStatusListener = pTestStatusListener;
    }

    bool CCSTSE_ServerBase::IsStartCSTSEClient()
    {
        return mCSTSEProcessPipe.CheckProcessIsRun();
    }

    void CCSTSE_ServerBase::SetCryptoImpClass(CCSTSE_CryptoImp* pCryptoImp)
    {
        mpCryptoImp = pCryptoImp;
    }

    bool CCSTSE_ServerBase::StartCSTSEClient(const std::wstring& strCSTSEClientExec,std::map<std::wstring, std::wstring>& strParam)
    {
        if (!mCSTSEProcessPipe.CheckProcessIsRun())
        {
            if (!mCSTSEProcessPipe.InitProcessPipe(strCSTSEClientExec.c_str(),L"",JGW_GetApplicationFolder()))
            {
                mpLogListener->OnErrorLog(JGW_GetFormatWString(L"Start Process Fail(%s)",strCSTSEClientExec.c_str()).c_str());
                return false;
            }
            Sleep(250);
        }
        //Sleep( 20 *1000);
        CCSTSE_InitCommandParamJson initCommandParamJson;
        for (std::map<std::wstring, std::wstring>::iterator it = strParam.begin();
            it != strParam.end();
            ++ it)
        {
            initCommandParamJson.Params[JGW_W2A(it->first)] = JGW_W2A(it->second);
        }

        if (!SendMsgJsonToClient(initCommandParamJson.GetInitCommandParamJson()))
        {
            mpLogListener->OnErrorLog(L"send msg json fail!!!");
            return false;
        }

        std::string strMsgJson;
        if (!GetMsgJsonToCSTSEAction(ECSTSE_ACTION_RESPONSE_COMMAND_PARAM_INIT_TSE,strMsgJson,60))
        {
            mpLogListener->OnErrorLog(L"recv tse param init command fail!!!");
            return false;
        }

        CCSTSE_ResponseBaseJson rspBaseJson(ECSTSE_ACTION_RESPONSE_COMMAND_PARAM_INIT_TSE);
        if (!rspBaseJson.ParseResponseBaseJson(strMsgJson))
        {
            mpLogListener->OnErrorLog(JGW_GetFormatWString(L"parse tse param init command json fail(%s)",strMsgJson.c_str()).c_str());
            return false;
        }

        if (!rspBaseJson.Result)
        {
            mpLogListener->OnErrorLog(JGW_A2W(rspBaseJson.ErrorMsg).c_str());
            return false;
        }
        return true;
    }

    bool CCSTSE_ServerBase::SendMsgJsonToClient(const std::string& strJson,int time_out_sec /* = 10 */)
    {
        if (!IsStartCSTSEClient())
        {
            return false;
        }
        std::string strWriteJson = mpCryptoImp->Encryption(strJson);
        strWriteJson += "\n";
        return mCSTSEProcessPipe.timed_send(strWriteJson,time_out_sec);
    }

    bool CCSTSE_ServerBase::GetMsgJsonToCSTSEAction(ECSTSE_ACTION CSTSEAction,std::string& strMsgJson,int time_out_sec /* = 60 */)
    {
        CCSTSE_BaseJson baseJson(ECSTSE_ACTION_Unknown);
        std::string strRead;

        jgw_timer jt;
        bool bFind = false;
        while (jt.elapsed() <= time_out_sec && !bFind)
        {
            if (mCSTSEProcessPipe.timed_recv(strRead,1))
            {
                JGW_ReplaceStringA(strRead,"\r","");
                std::vector<std::string> vstrTemp;
                JGW_ParserStrA(strRead.c_str(),"\n",vstrTemp);
                
                for (size_t i = 0;i < vstrTemp.size();i ++)
                {
                    strRead = vstrTemp[i];
                    JGW_EraseLastAndFristTwoChars(strRead);
                    //OutputDebugStringA(strRead.c_str());
                    //OutputDebugStringA("\n");
                    std::string strValue = mpCryptoImp->Decryption(strRead);
                    //OutputDebugStringA(strValue.c_str());
                    //OutputDebugStringA("\n");
                    baseJson.CSTSEAction = ECSTSE_ACTION_Unknown;
                    if (baseJson.ParseBaseJson(strValue))
                    {
                        HandleTSETestLogMsg(strValue,baseJson);
                        if (baseJson.CSTSEAction == CSTSEAction)
                        {
                            strMsgJson = strValue;
                            bFind = true;
                        }
                    }  
                }
            }
            //!Sleep(10);
        }
        return bFind;
    }

    //bool CCSTSE_ServerBase::CheckSendRecvJson(const std::string& strJson,int action,std::string& strErrorMsg,int time_out_sec /* = 10 */)
    //{
    //    std::string strWriteJson = mpCryptoImp->Encryption(strJson);
    //    strWriteJson += "\n";

    //    if (!IsStartCSTSEClient())
    //    {
    //        return false;
    //    }

    //    if (!mCSTSEProcessPipe.timed_send(strWriteJson,3))
    //    {
    //        return false;
    //    }

    //    jgw_timer jt;
    //    CCSTSE_BaseJson baseJson(ECSTSE_ACTION_Unknown);
    //    std::string strRead;

    //    while (jt.elapsed() <= time_out_sec)
    //    {
    //        if (mCSTSEProcessPipe.timed_recv(strRead,3))
    //        {
    //            JGW_ReplaceStringA(strRead,"\r","");
    //            std::vector<std::string> vstrTemp;
    //            bool bFind = false;
    //            JGW_ParserStrA(strRead.c_str(),"\n",vstrTemp);
    //            for (size_t i = 0;i < vstrTemp.size();i ++)
    //            {
    //                strRead = vstrTemp[i];
    //                JGW_EraseLastAndFristTwoChars(strRead);
    //                OutputDebugStringA(strRead.c_str());
    //                OutputDebugStringA("\n");
    //                std::string strValue = mpCryptoImp->Decryption(strRead);
    //                OutputDebugStringA(strValue.c_str());
    //                OutputDebugStringA("\n");
    //                if (baseJson.ParseBaseJson(strValue))
    //                {
    //                    HandleTSETestLogMsg(strValue,baseJson);
    //                    if (baseJson.CSTSEAction == action)
    //                    {
    //                        bFind = true;
    //                    }
    //                }                    
    //            }
    //            if (bFind)
    //            {
    //                break;
    //            }
    //        }

    //        Sleep(200);
    //    }
    //    strErrorMsg = responseBaseJson.ErrorMsg;
    //    return responseBaseJson.Result;
    //}

    //bool CCSTSE_ServerBase::CheckSendRecvJson(const std::string& strJson,int action,int time_out_sec /* = 10 */)
    //{
    //    std::string strReadJson;
    //    return CheckSendRecvJson(strJson,action,strReadJson,time_out_sec);
    //}

    void CCSTSE_ServerBase::HandleTSETestLogMsg(const std::string& strJson,CCSTSE_BaseJson& rsp)
    {
        switch (rsp.CSTSEAction)
        {
        case ECSTSE_ACTION_TEST_LOG:
            LogJsonMsgToInvokeLogListener(strJson);
            break;
        case ECSTSE_ACTION_TEST_INDEX_CHANGE:
            TestIndexChangeJsonMsgToInvokeTestStatusListener(strJson);
            break;
        case ECSTSE_ACTION_TEST_STATUS_CHANGE:
            TestStatusJsonMsgToInvokeTestStatusListener(strJson);
            break;
        }
    }

    void CCSTSE_ServerBase::LogJsonMsgToInvokeLogListener(const std::string& strLogJson)
    {
        CCSTSE_TestLogJson testJson;
        if (mpLogListener && testJson.ParseTestLogJson(strLogJson))
        {
            switch (testJson.LogType)
            {
            case ECSTSE_TSE_TEST_LOG_TYPE_DEBUG:
                mpLogListener->OnDebugLog(JGW_A2W(testJson.Log).c_str());
                break;
            case ECSTSE_TSE_TEST_LOG_TYPE_ERROR:
                mpLogListener->OnErrorLog(JGW_A2W(testJson.Log).c_str());
                break;
            case ECSTSE_TSE_TEST_LOG_TYPE_INFO:
                mpLogListener->OnInfoLog(JGW_A2W(testJson.Log).c_str());
                break;
            case ECSTSE_TSE_TEST_LOG_TYPE_USERDEFINE:
                mpLogListener->OnUserDefinedLog(testJson.UserDefineType,JGW_A2W(testJson.Log).c_str());
                break;
            }
        }
    }

    void CCSTSE_ServerBase::TestStatusJsonMsgToInvokeTestStatusListener(const std::string& strStatusJson)
    {
        CCSTSE_TestStatusChangeJson testStatusChangeJson;
        if (mpTestStatusListener && testStatusChangeJson.ParseTestStatusChangeJson(strStatusJson))
        {
            mpTestStatusListener->OnTestStatusChange(JGW_A2W(testStatusChangeJson.TestName).c_str(),testStatusChangeJson.TestStatus);
        }
    }

    void CCSTSE_ServerBase::TestIndexChangeJsonMsgToInvokeTestStatusListener(const std::string& strTestIndexChangeJson)
    {
        CCSTSE_TestIndexChangeJson testIndexChangeJson;
        if (mpTestStatusListener && testIndexChangeJson.ParseTestIndexChangeJson(strTestIndexChangeJson))
        {
            mpTestStatusListener->OnTestIndexChange(testIndexChangeJson.Index,testIndexChangeJson.SubIndex);
        }
    }

    bool CCSTSE_ServerBase::ExecuteAllTest()
    {
        return false;
    }

    bool CCSTSE_ServerBase::ExecuteAutoTest()
    {
        return false;
    }

    bool CCSTSE_ServerBase::ExecuteConfigViewTest()
    {
        CCSTSE_TSETestJson tesTestJson;
        tesTestJson.SuiteTestType = ECSTSE_TSET_TYPE_ConfigView;
        SendMsgJsonToClient(tesTestJson.GetTSETestJson());
        return true;
    }
}

