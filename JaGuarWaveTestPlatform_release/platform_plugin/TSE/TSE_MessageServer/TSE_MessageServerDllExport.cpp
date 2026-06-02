#include "StdAfx.h"
#include <TSE_MessageServer/TSE_MessageServerDllExport.h>
#include "TSE_MsgServer.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <vector>
#define TSE_MS_CONFIG_FILE_NAME L"tse_ms.config"

JGW::CTSE_MsgServer* gpMsgServer = NULL;
std::vector<std::string> gvStrID;
JGW::CCJGW_CriticalSectionLock gCriticalSectionLock;
typedef struct
{
    int mnLogPort;
    //int mnControlPort;
    std::string mstrLogHost;
    //std::string mstrControlHost;
}S_TSE_MS_CONFIG,*LPS_TSE_MS_CONFIG;

void LoadTSEMSConfig(S_TSE_MS_CONFIG& sTSEMSConfig)
{
    std::wstring strConfigFilePath = JGW::JGW_GetExecConfigFilePath(TSE_MS_CONFIG_FILE_NAME);

    JGW::CCJGW_ConfigIni configInfi;
    configInfi.InitIniFilePath(strConfigFilePath.c_str());
    sTSEMSConfig.mnLogPort = configInfi.GetIniKeyIntValue(L"TSE_MS",L"port",3003);
    sTSEMSConfig.mstrLogHost = configInfi.GetIniKeyValueA(L"TSE_MS",L"host",L"127.0.0.1");
}

void UpdateTSEMSConfigSocketPort(int port)
{
    std::wstring strConfigFilePath = JGW::JGW_GetExecConfigFilePath(TSE_MS_CONFIG_FILE_NAME);

    JGW::CCJGW_ConfigIni configInfi;
    configInfi.InitIniFilePath(strConfigFilePath.c_str());
    configInfi.SetIniKeyValue(L"TSE_MS",L"port",port);
}


void StartTSEMessageServer()
{
    if (NULL == gpMsgServer)
    {
        JGW::JGW_KillProcessToName(L"QSPR.exe");
        JGW::JGW_KillProcessToName(L"JaGuarWaveQualcommHelpTool.exe");        
        //JGW::JGW_KillProcessToName(L"cmd.exe");
        S_TSE_MS_CONFIG sTSEMSConfig;
        LoadTSEMSConfig(sTSEMSConfig);
        gpMsgServer = new JGW::CTSE_MsgServer(sTSEMSConfig.mstrLogHost,sTSEMSConfig.mnLogPort);
    }
}

void StopTSEMessageServer()
{
    if (NULL != gpMsgServer)
    {
        //! Ê×ÏÈ¹Ø±ÕMSG SERVER
        gpMsgServer->Stop();
        delete gpMsgServer;
        gpMsgServer = NULL;  
    }
}

void* GetTSELogBaseToID(const char* strID)
{
    if (NULL != gpMsgServer)
    {
        std::shared_ptr<JGW::CTSE_LogMsg> ptrLogMsgBase = gpMsgServer->GetMsgManage()->GetLogMsgClassToID(strID);
        if (!ptrLogMsgBase)
        {
            return NULL;
        }
        return ptrLogMsgBase.get();
    }
    return NULL;
}

void* GetTSEControlBaseToID(const char* strID)
{
    if (NULL != gpMsgServer)
    {
        std::shared_ptr<JGW::CTSE_ControlMsg> ptrLogMsgBase = gpMsgServer->GetMsgManage()->GetControlMsgClassToID(strID);
        if (!ptrLogMsgBase)
        {
            return NULL;
        }
        return ptrLogMsgBase.get();
    }
    return NULL;
}

const char* GetTSEMSID()
{
    JGW::CCJGW_CriticalSectionAutoLock autoLock(gCriticalSectionLock);
    std::string strTemp;
    JGW::JGW_FormatString(strTemp,"TSEMS_%d",gvStrID.size() + 1);
    gvStrID.push_back(strTemp);
    return gvStrID[gvStrID.size() - 1].c_str();
}