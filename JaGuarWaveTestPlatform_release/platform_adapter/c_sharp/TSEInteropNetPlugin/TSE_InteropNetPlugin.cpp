// TSEInteropNetPlugin.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "TSE_SequenceTestInteropManager.h"
#include "TSE_TestLog.h"
#include "TSE_TestStatusLog.h"
#include "TSE_SequenceResourceManage.h"

JGW::CTSE_SequenceTestInteropManager* gpInteropManager = NULL;
TSETestLogFunc gobalLogCallBack = NULL;
TSETestStatusChangeFunc gobalTestStatusCallBack = NULL;
JGW::CTSE_TestLog gTestLog;
JGW::CTSE_TestStatusLog gTestStatusLog;
JGW::CTSE_SequenceResourceManage gSequenceResourceManage;
//int gErrorRetryTestCount = 3;

void SetTSETestLogCallBack(TSETestLogFunc testLogCallBack)
{
    gobalLogCallBack = testLogCallBack;
}

void SetTSETestStatusCallBack(TSETestStatusChangeFunc testStatusCallBack)
{
    gobalTestStatusCallBack = testStatusCallBack;
}

bool InitInteropManager()
{
    ReleaseInteropManager();
    if (gobalLogCallBack) gobalLogCallBack(0,0,L"InitInteropManager");
    gpInteropManager = new JGW::CTSE_SequenceTestInteropManager(gSequenceResourceManage);
    if (gpInteropManager)
    {
        gSequenceResourceManage.SetOnLogListener(&gTestLog);
        gSequenceResourceManage.SetOnTestStatusListener(&gTestStatusLog);
    }
    return (NULL != gpInteropManager);
}

void ReleaseInteropManager()
{
    if (gpInteropManager)
    {
        if (gobalLogCallBack) gobalLogCallBack(0,0,L"ReleaseInteropManager");
        gSequenceResourceManage.RemoveOnLogListener(&gTestLog);
        gSequenceResourceManage.RemoveOnTestStatusListener(&gTestStatusLog);
        delete gpInteropManager;
        gpInteropManager = NULL;
    }
}

//void UpdateErrorRetryTestCount(int errorRetryTestCount)
//{
//    if (errorRetryTestCount < 1) errorRetryTestCount = 1;
//    gErrorRetryTestCount = errorRetryTestCount;
//}

bool LoadTSEModuleToModuleName(const wchar_t* modulePath)
{
    if (gpInteropManager)
    {
        return gpInteropManager->LoadTSEModuleToModuleName(modulePath);
    }
    return false;
}

bool UpdateSuiteXmlConfigFilePath(const wchar_t* suiteXmlConfigFilePath)
{
    return false;
}

void* GetSequenceTestInstanceToRealName(const wchar_t* realName,const wchar_t* modulePath)
{
    JGW::CTSE_TestBase* ptrTestBase = NULL;
    if (gpInteropManager)
    {
        ptrTestBase = gpInteropManager->GetTestSuitePtrClassToModuleName(realName,modulePath);
    }
    return ptrTestBase;
}

void UnLoadTSEModule()
{
    if (gpInteropManager) gpInteropManager->UnLoadTSEModule();
}

void UpdateSequenceTestConfigParamToInstance(void* pSeqTestInstance,const wchar_t* paramName,const wchar_t* paramVal)
{
    if (gpInteropManager && NULL != pSeqTestInstance)
    {
        JGW::CTSE_TestBase* ptrTestBase = dynamic_cast<JGW::CTSE_TestBase*>((JGW::CTSE_TestBase*)pSeqTestInstance);
        gpInteropManager->UpdateTestSuiteConfigParam(ptrTestBase,paramName,paramVal);
    }
}

bool ExecuteSequenceTestToInstance(void* pSeqTestInstance,bool isReversedResult)
{
    if (gpInteropManager && NULL != pSeqTestInstance)
    {
        JGW::CTSE_TestBase* ptrTestBase = dynamic_cast<JGW::CTSE_TestBase*>((JGW::CTSE_TestBase*)pSeqTestInstance);
        if (ptrTestBase) 
        {
            //for (int i = 0;i < gErrorRetryTestCount; i ++)
            //{
                if (gpInteropManager->ExecTestSuite(ptrTestBase,isReversedResult)) return true;
            //}
        }    
    }
    return false;
}

void ReleaseSequenceTestInstance(void* pSeqTestInstance)
{
    if (gpInteropManager)
    {
        gpInteropManager->ReleaseTSEInstance(pSeqTestInstance);
    }
//     JGW::CTSE_TestBase* ptrTestBase = dynamic_cast<JGW::CTSE_TestBase*>((JGW::CTSE_TestBase*)pSeqTestInstance);
//     if (ptrTestBase) delete ptrTestBase;
}

void UpdateTSEGlobalVariable(const wchar_t* environmentName,const wchar_t* environmentValue)
{
    if (gpInteropManager)
    {
        gpInteropManager->UpdateTSEGlobalEnvironment(environmentName,environmentValue);
    }
}

const wchar_t* GetTSEGlobalVariableValue(const wchar_t* environmentName)
{
    if (gpInteropManager)
    {
        return gpInteropManager->GetTSEGlobalEnvironment(environmentName);
    }
    return L"";
}

void ClearTSEGlobalVariable()
{
    if (gpInteropManager)
    {
        gpInteropManager->ClearGlobalEnvironment();
    }
}