#include "StdAfx.h"
#include "JGW_CSharpModuleThread.h"
#include <JGW_CSharpModule/JGW_CSharpModuleExport.h>
#include "JGW_PipeCommunication.h"
#include "CJGW_CSharpBaseRequstJson.h"

#include "CJGW_UpdateLogReqJson.h"

namespace JGW
{
    CJGW_CSharpModuleThread::CJGW_CSharpModuleThread(void) : mpCommunicationBase(NULL)
    {
        mszRecvBuf = (char*)calloc(1,CSHARP_MESSGAE_MAX_BUFFER);
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CJGW_CSharpModuleThread, CCMessageThread)
    END_MYTHREAD_MESSAGE_MAP()

    CJGW_CSharpModuleThread::~CJGW_CSharpModuleThread(void)
    {
        if (mszRecvBuf)
        {
            free(mszRecvBuf);
        }
        if (mpCommunicationBase)
        {
            mpCommunicationBase->DisConnect();
            delete mpCommunicationBase;
            mpCommunicationBase = NULL;
        }
    }

    bool CJGW_CSharpModuleThread::StartThread()
    {
        mpCommunicationBase = new CJGW_PipeCommunication();
        if (!mpCommunicationBase->Create(JGW_CSHARP_MODULE_PIPE_NAME,0))
        {
            delete mpCommunicationBase;
            mpCommunicationBase = NULL;
            return false;
        }
        
       return CreateMessageThread(0,0,true);
    }
    //! {"SN":"123456798912","OperateID":"-1","IsSMTMoNo":true,"RequestID":0}
    void CJGW_CSharpModuleThread::RunThread()
    {
        if (!mpCommunicationBase->IsConnect())
        {
            if (!mpCommunicationBase->Connect())
            {
                return ;
            }
        }
        int len = mpCommunicationBase->Read(mszRecvBuf,CSHARP_MESSGAE_MAX_BUFFER);
        if (len > 0)
        {
            std::string strRead(mszRecvBuf,len);
            HandleRequest(strRead);
        }
    }

    void CJGW_CSharpModuleThread::HandleRequest(const std::string& strRecv)
    {
        CCJGW_FTM_Json ftmJson;
        if (!ftmJson.FromJsonToString(strRecv))
        {
            return ;
        }

        CCJGW_CSharpBaseRequstJson baseRequestJson;
        baseRequestJson.FromJosn(ftmJson);
        switch (baseRequestJson.RequestID)
        {
        case E_CSM_UNKNOWN:
            PELOG4WW_F_L(L"UNKNOWN REQUEST ID:%d",baseRequestJson.RequestID);
            break;
        case E_CSM_MES_CHECK_SN_STATUS:
            HandleMESCheckSNStatusReq(ftmJson);
            break;
        case E_CSM_MES_GET_IMEI_INFO:
            HandleMESGetIMEIInfoReq(ftmJson);
            break;
        case E_CSM_MES_GET_SN_INFO:
            HandleMESGetSNInfoReq(ftmJson);
            break;
        case E_CSM_MES_NEXT_IMEI_STATION:
            HandleMESNextIMEIStationReq(ftmJson);
            break;
        case E_CSM_MES_NEXT_SN_STATION:
            HandleMESNextSNStationReq(ftmJson);
            break;
        case E_CSM_MES_SAVE_TEST_RECORDS:
            HandleMESSaveTestRecordsReq(ftmJson);
            break;
        case E_CSM_TSE_CREATE_SUITE_PROJECT:
            HandleTSECreateSuiteProjectReq(ftmJson);
            break;
        case E_CSM_TSE_GET_ENVIRONMENT_VALUE:
            HandleTSEGetEnvironmentValueReq(ftmJson);
            break;
        case E_CSM_TSE_SET_ENVIRONMENT_VALUE:
            HandleTSESetEnviromentValueReq(ftmJson);
            break;
        case E_CSM_TSE_RUN_SUITE_PROJECT:
            HandleTSERunSuiteProjectReq(ftmJson);
            break;
        case E_CSM_UPDATE_TEST_LOG:
            HandleUpdateLogReq(ftmJson);
            break;
        default:
            PELOG4WW_F_L(L"UNKNOWN REQUEST ID:%d",baseRequestJson.RequestID);
            break;
        }
    }

    void CJGW_CSharpModuleThread::HandleMESCheckSNStatusReq(CCJGW_FTM_Json& ftmJson)
    {

    }

    void CJGW_CSharpModuleThread::HandleMESGetIMEIInfoReq(CCJGW_FTM_Json& ftmJson)
    {

    }

    void CJGW_CSharpModuleThread::HandleMESGetSNInfoReq(CCJGW_FTM_Json& ftmJson)
    {

    }

    void CJGW_CSharpModuleThread::HandleMESNextIMEIStationReq(CCJGW_FTM_Json& ftmJson)
    {

    }

    void CJGW_CSharpModuleThread::HandleMESNextSNStationReq(CCJGW_FTM_Json& ftmJson)
    {

    }

    void CJGW_CSharpModuleThread::HandleMESSaveTestRecordsReq(CCJGW_FTM_Json& ftmJson)
    {

    }

    void CJGW_CSharpModuleThread::HandleTSECreateSuiteProjectReq(CCJGW_FTM_Json& ftmJson)
    {

    }

    void CJGW_CSharpModuleThread::HandleTSEGetEnvironmentValueReq(CCJGW_FTM_Json& ftmJson)
    {

    }

    void CJGW_CSharpModuleThread::HandleTSESetEnviromentValueReq(CCJGW_FTM_Json& ftmJson)
    {

    }

    void CJGW_CSharpModuleThread::HandleTSERunSuiteProjectReq(CCJGW_FTM_Json& ftmJson)
    {

    }

    void CJGW_CSharpModuleThread::HandleUpdateLogReq(CCJGW_FTM_Json& ftmJson)
    {
        CCJGW_UpdateLogReqJson updateLogReqJson;
        updateLogReqJson.FromJosn(ftmJson);
        switch (updateLogReqJson.Level)
        {
        case E_CSM_DEBUG_LOG:
            PDLOG4WW_L(updateLogReqJson.strwLog.c_str());
            break;
        case E_CSM_INFO_LOG:
            PDLOG4WW_L(updateLogReqJson.strwLog.c_str());
            break;
        case E_CSM_ERROR_LOG:
            PELOG4WW_L(updateLogReqJson.strwLog.c_str());
            break;
        }
    }
}
