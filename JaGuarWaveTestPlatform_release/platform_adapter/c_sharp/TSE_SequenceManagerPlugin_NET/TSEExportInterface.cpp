// TSE_SequenceManagerPlugin_NET.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "TSEExportInterface.h"
#include <string>
#include "CTSE_ManagerSource.h"

LogCallBackDelegate gLogCallBack = NULL;
//! JGW::CTSE_GlobalEnvironment gGlobalEnvironment;
//! JGW::CTSE_LogNet gLogNet;
//! JGW::CTSE_SequenceTestNet gSequenceTestNet(gLogNet,gGlobalEnvironment);
//! S_MES_INFO gsMESInfo = {0};
//#if 1
//JGW::S_HTML_QDART_RESOLVE_CONFIG_INFO* gpsHtmlQdartResolveConfigInfo = NULL;
//#endif
JGW::CCTSE_ManagerSource* GetManagerSource()
{
    static JGW::CCTSE_ManagerSource* pManagerSource = NULL;
    if (NULL == pManagerSource)
    {
        pManagerSource = new JGW::CCTSE_ManagerSource();
    }
    return pManagerSource;
}

bool LoadTSEModule(const wchar_t* moduleName)
{
    return GetManagerSource()->LoadTSEModule(moduleName);
}

void* CreateTSEInstance(const wchar_t* realName, const wchar_t* moduleName)
{
    return GetManagerSource()->CreateTSEInstance(realName,moduleName);
}

bool UpdateConfigParamToTSEInstance(void* instance, const wchar_t* paramName, const wchar_t* paramValue)
{
    return GetManagerSource()->UpdateConfigParamToTSEInstance((JGW::CTSE_TestBase*)instance,paramName,paramValue);
}

bool InitTestToTSEInstance(void* instance)
{
    return GetManagerSource()->InitTestToTSEInstance((JGW::CTSE_TestBase*)instance);
}

bool ExecuteTSEInstance(void* instance)
{
    return GetManagerSource()->ExecuteTSEInstance((JGW::CTSE_TestBase*)instance);
}

bool ExitTestToTSEInstance(void* instance)
{
    return GetManagerSource()->ExitTestToTSEInstance((JGW::CTSE_TestBase*)instance);
}

int GetGlobalEnvironment(const wchar_t* key,wchar_t* val)
{
    return GetManagerSource()->GetGlobalEnvironment(key,val);
}

void SetGlobalEnvironment(const wchar_t* key, const wchar_t* val)
{
    //if (gLogCallBack) gLogCallBack(0,0,L"SetGlobalEnvironment");
    GetManagerSource()->SetGlobalEnvironment(key,val);
}

void RegistLogCallBackFunc(LogCallBackDelegate logCallBack)
{
    gLogCallBack = logCallBack;
    //!if (gLogCallBack) gLogCallBack(0,0,L"RegistLogCallBackFunc");
}

void UnLoadTSEModule(const wchar_t* moduleName)
{
    GetManagerSource()->UnLoadTSEModule(moduleName);
}

bool LoadTSEMESConfig(const wchar_t* mesConfigPath)
{
    return GetManagerSource()->LoadTSEMESConfig(mesConfigPath);
}

void UpdateTSEMESOperation(const wchar_t* operationID)
{
	GetManagerSource()->UpdateTSEMESOperation(operationID);
}

void UpdateTSEMESUserPassword(const wchar_t* userName,const wchar_t* password)
{
	GetManagerSource()->UpdateTSEMESUserPassword(userName,password);
}

bool LoadHtmlResolveRuleFilePath(const wchar_t* htmlResolveRuleFilePath)
{
    return GetManagerSource()->LoadHtmlResolveRuleFilePath(htmlResolveRuleFilePath);
}

const wchar_t* ResolveHtmlFileInfo(const wchar_t* htmlFilePath,int showTestLogType,bool saveTestLogging,const wchar_t* sn,bool testResult)
{
    return GetManagerSource()->ResolveHtmlFileInfo(htmlFilePath,showTestLogType,saveTestLogging,sn,testResult);
}