#include "StdAfx.h"
#include "TSETestInfoForward.h"
#include <Windows.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "json/TSEControlCmdJson.h"

namespace JGW
{
    CTSETestInfoForward::CTSETestInfoForward(bool& bStopTSETest) : mbStopTSETest(bStopTSETest),mpIWriteInfoServerInterface(NULL)
    {
    }


    CTSETestInfoForward::~CTSETestInfoForward(void)
    {
    }

    void CTSETestInfoForward::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
    {
        std::string strCommand = JGW_W2A(strTestName);
        if (strCommand.length() < TCP_CONTENT_MAX_BUF_LEN)
        {
            mTSETestMsgQueue.PutAsyncMessage(CTSEServerJsonCommand::GetTestStatusChangeJsonCommand(eTestStatus,strCommand));
        }
        else
        {
            mTSETestMsgQueue.PutAsyncMessage(CTSEServerJsonCommand::GetTestStatusChangeJsonCommand(eTestStatus,strCommand.substr(0,TCP_CONTENT_MAX_BUF_LEN)));
        }
    }

    void CTSETestInfoForward::OnTestIndexChange(size_t index,int subIndex /* = -1 */)
    {
        mTSETestMsgQueue.PutAsyncMessage(CTSEServerJsonCommand::GetTestIndexChangeJsonCommand(index,subIndex));
    }

    void CTSETestInfoForward::OnDebugLog(const wchar_t* strDebugLog)
    {
        AddTestLogQueue(Debug_TSE,JGW_W2A(strDebugLog));
    }

    void CTSETestInfoForward::OnInfoLog(const wchar_t* strInfoLog)
    {
        AddTestLogQueue(Info_TSE,JGW_W2A(strInfoLog));
    }

    void CTSETestInfoForward::OnErrorLog(const wchar_t* strErrorLog)
    {
        AddTestLogQueue(Error_TSE,JGW_W2A(strErrorLog));
    }

    void CTSETestInfoForward::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
    {
        AddTestLogQueue(UserDefine_TSE,JGW_W2A(strCustomLog),(E_USER_DEFINE_LOG_TYPE)id);
    }

    void CTSETestInfoForward::AddTestLogQueue(TSELogTypeEnum tseLogTypeEnum,const std::string& strLog,E_USER_DEFINE_LOG_TYPE userDefineID /* = E_UNKNOW_USER_DEFINE */)
    {
        if (strLog.length() < TCP_CONTENT_MAX_BUF_LEN)
        {
            mTSETestMsgQueue.PutAsyncMessage(CTSEServerJsonCommand::GetTestLogJsonCommand(tseLogTypeEnum,strLog,userDefineID));
        }
        else
        {
            int len = strLog.length();
            int index = 0;
            while (len > 0)
            {
                int count = len > TCP_CONTENT_MAX_BUF_LEN?TCP_CONTENT_MAX_BUF_LEN:len; 
                mTSETestMsgQueue.PutAsyncMessage(CTSEServerJsonCommand::GetTestLogJsonCommand(tseLogTypeEnum,strLog.substr(index,count),userDefineID));
                index += count;
                len -= TCP_CONTENT_MAX_BUF_LEN;
            }
        }
    }

    void CTSETestInfoForward::SendDataToTcpServer(const std::string& strData)
    {
        mTSETestMsgQueue.PutAsyncMessage(strData);
    }

    void CTSETestInfoForward::OnForwardRecvControlCmdInfo(const std::string& strData)
    {
        mTSERecvTestMsgQueue.PutAsyncMessage(strData);
    }

    bool CTSETestInfoForward::GetRecvControlCmdInfos(std::vector<std::string>& vTestControlCmds)
    {
        if (mvTestControlCmds.empty())
        {
            return false;
        }

        JGW::CCJGW_CriticalSectionAutoLock autoLock(mCriticalSectionLock);
        for (size_t i = 0;i < mvTestControlCmds.size();i ++)
        {
            vTestControlCmds.push_back(mvTestControlCmds[i]);
        }
        mvTestControlCmds.clear();
        return true;
    }

    void CTSETestInfoForward::SetWriteInfoServerInterfaceInstance(IWriteInfoServerInterface* iwriteInfoServerInterface)
    {
        mpIWriteInfoServerInterface = iwriteInfoServerInterface;
    }

    void CTSETestInfoForward::OnForWardTSETestInfo()
    {
        std::queue<std::string>* pTestMsgQueue = NULL;
        std::queue<std::string>* pRecvMsgQueue = NULL;
        while (true)
        {
            if (NULL == pRecvMsgQueue || pRecvMsgQueue->empty())
            {
                pRecvMsgQueue = mTSERecvTestMsgQueue.GetExecuteQueue();
                if (!pRecvMsgQueue->empty()) HandleRecvControlCmdInfo(pRecvMsgQueue);
            }

            if (NULL == pTestMsgQueue || pTestMsgQueue->empty())
            {
                pTestMsgQueue = mTSETestMsgQueue.GetExecuteQueue();
                if (!pTestMsgQueue->empty()) mpIWriteInfoServerInterface->WriteInfoToTcpServer(pTestMsgQueue);
            }

            Sleep(500);
        }
    }

    void CTSETestInfoForward::HandleRecvControlCmdInfo(std::queue<std::string>* pRecvMsgQueue)
    {
        while (!pRecvMsgQueue->empty())
        {
            std::string strData = pRecvMsgQueue->front();
            pRecvMsgQueue->pop();

            CCJGW_FTM_Json ftmJson;
            if (!ftmJson.FromJsonToString(strData))
            {
                continue;
            }

            CTSEControlCmdJson controlCmdJson;
            controlCmdJson.FromJosn(ftmJson);
            TSETestControlEnum testControlEnum = controlCmdJson.GetTestControlCmdIndex();
            if (ControlCmd_AJE == controlCmdJson.ActionEnumID)
            {
                if (testControlEnum == Start_TCE || testControlEnum == Init_TCE)
                {
                    CCJGW_CriticalSectionAutoLock autoLock(mCriticalSectionLock);
                    mvTestControlCmds.push_back(strData);
                }
                else if (testControlEnum == Stop_TCE)
                {
                    mbStopTSETest = true;
                }
                else if (testControlEnum == Exit_TCE)
                {
                    exit(0);
                }
            } 
        }
    }
}

