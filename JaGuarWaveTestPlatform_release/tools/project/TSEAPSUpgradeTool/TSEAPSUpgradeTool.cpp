// TSEDownload.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageInterface.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
#include <TSE_SequenceManagerPlugin/TSE_LogListener.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include <JGW_CryptPlugin/CJGW_CryptDes.h>
//! TSE_DOWNLOAD_PORT
//! TSE_PROVISION_XML_NAME 
//! TSE_DOWNLOAD_PATH
//! TSE_RAWPROGRAM_XML_NAMES
//! TSE_PATCH_XML_NAMES
//! TSE_RESET_ENABLE
using namespace JGW;

CTSE_SequenceManageInterface* pSequenceManageInterface = NULL;
CTSE_SequencePocoClassLoader<CTSE_SequenceManageInterface> mTestSuitePocoClassLoader;
CTSE_SequenceManageImpl* pSequenceManageImpl = NULL;
bool gbIsEncryptedConfig = false;
const wchar_t g_szStartFlag[] = L"{D261BC0C-45F3-498D-AEE8-5E730691CF35}";
const wchar_t g_szPassFlag[] = L"{A6A28E32-E91C-40D0-A79B-06CC8B441601}";
const wchar_t g_szFailFlag[] = L"{241BC6EF-0CD0-49CE-9B39-EEA67691669C}";
const wchar_t g_szExitFlag[] = L"{65FDA37A-8DFE-4540-9611-255A5C823649}";

std::string mSuiteConfigPath = "C:\\JW\\suite.xml";
bool bErrorStop = true;
bool bAutoRun = false;
//!  verbose log
bool bVerboseLog = true;
//! 自动运行中出现错误停止 默认不停止
bool gbAutoRunErrorStop = false;
int nInterval = 5000;

int gnPassRecord = 0;
int gnFailRecord = 0;
int gnTestCount = 0;

//! TSE_ERROR_STOP=0 TSE_SUITE_CONFIG_FILE_PATH
void PareseParam(int argc, _TCHAR* argv[])
{
    std::wstring strTemp,strKey,strVal;
    int pos = 0;

    for (int i = 1;i < argc;i ++)
    {
        strTemp = argv[i];
        strKey = argv[i];
        strVal = L"";
        pos = strTemp.find('=');
        if (-1 != pos)
        {
            strKey = strTemp.substr(0,pos);
            if (strTemp.length() > pos + 1) strVal = strTemp.substr(pos + 1);
            JGW_EraseLastAndFristChars(strKey,' ');
            JGW_EraseLastAndFristChars(strVal,' ');
        }
        if (bVerboseLog) std::wcout << L"Key: " << strKey << L" Val: " << strVal << std::endl;
        if (0 == strKey.compare(L"TSE_ERROR_STOP")) bErrorStop = (_ttoi(strVal.c_str()) == 1);
        if (0 == strKey.compare(L"TSE_VERBOSE_LOG")) bVerboseLog = (_ttoi(strVal.c_str()) == 1);
        if (0 == strKey.compare(L"TSE_ENCRYPT_CONFIG")) gbIsEncryptedConfig = (_ttoi(strVal.c_str()) == 1);
        if (0 == strKey.compare(L"TSE_SUITE_CONFIG_FILE_PATH")) mSuiteConfigPath = JGW_W2A(strVal);
        if (0 == strKey.compare(L"TSE_SUITE_AUTO_RUN")) bAutoRun = (_ttoi(strVal.c_str()) == 1);
        if (0 == strKey.compare(L"TSE_SUITE_AUTO_RUN_ERROR_STOP")) gbAutoRunErrorStop = (_ttoi(strVal.c_str()) == 1);
        if (0 == strKey.compare(L"TSE_SUITE_AUTO_RUN_INTERVAL")) nInterval = _ttoi(strVal.c_str());
        pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutString(strKey,strVal);
    }

    mSuiteConfigPath = JGW_W2A(JGW_RealativePathToAbsPath(JGW_A2W(mSuiteConfigPath).c_str()));
}

class CTestLog : public CTSE_LogListener,public CTSE_TestStatusListener
{
public:
    void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
    {
        std::wstring strTemp;
        switch (eTestStatus)
        {
        case E_INIT_TEST_STATUS:
            break;
        case E_RUN_TEST_STATUS:
            break;
        case E_RUNING_TEST_STATUS:
            JGW_FormatWString(strTemp,L"%s => Runing",strTestName);
            std::wcout << strTemp << std::endl;
            break;
        case E_PASS_TEST_STATUS:
            JGW_FormatWString(strTemp,L"%s => PASS",strTestName);
            std::wcout << strTemp << std::endl;
            break;
        case E_FAIL_TEST_STATUS:
            JGW_FormatWString(strTemp,L"%s => FAIL",strTestName);
            std::wcout << strTemp << std::endl;
            break;
        }
        
    }

    void OnInfoLog(const wchar_t* strInfoLog)
    {
        if (!bVerboseLog) return ;
        std::wstring strTemp(strInfoLog);
        JGW_EraseLastAndFristTwoCharsW(strTemp);
        if (!strTemp.empty())
        {
            std::wcout << strTemp.c_str() << std::endl;
        }
    }

    void OnErrorLog(const wchar_t* strErrorLog)
    {
        if (!bVerboseLog) return ;
        std::wstring strTemp(strErrorLog);
        JGW_EraseLastAndFristTwoCharsW(strTemp);
        if (!strTemp.empty())
        {
            std::wcout << "ERROR:" << strTemp.c_str() << std::endl;
        }
    }

    void OnDebugLog(const wchar_t* strDebugLog)
    {
        /*std::wstring strTemp(strDebugLog);
        JGW_EraseLastAndFristTwoCharsW(strTemp);
        if (!strTemp.empty())
        {
        std::wcout << strTemp.c_str() << std::endl;
        }*/
    }
};

CTestLog testLog;

void deinit()
{
    std::wcout << L"Exit Download Application"<< std::endl;
    if (pSequenceManageImpl) pSequenceManageImpl->GetSequenceTestImpl()->ExecTSETestFinalize();
    mTestSuitePocoClassLoader.UnloadTestSuiteModule();
}

std::string GetEncryptedTempFilePath(const std::string& strInputFilePath)
{
    if (!gbIsEncryptedConfig) return strInputFilePath;
    std::string strOutputFilePath;
    JGW_FormatString(strOutputFilePath,"%s\\%s_%u_%u.tmp",JGW_W2A_W(JGW_GetApplicationFolder()).c_str(),JGW_GetCurrentTimeStampYMDHMSA().c_str(),GetCurrentProcessId(),GetCurrentThreadId());
    if (!CCJGW_CryptDes::DecryptDESFile(strInputFilePath,strOutputFilePath))
    {
        return "";
    }
    return strOutputFilePath;
}

bool InitTSE(int argc, _TCHAR* argv[])
{
    std::wcout.imbue(std::locale(""));
    std::cout.imbue(std::locale(""));

    std::wstring strDllPath = JGW_GetApplicationFolder();
    strDllPath += L"TSE_SequenceManagerPlugin.dll";

    if (!mTestSuitePocoClassLoader.LoadTestSuiteModule(strDllPath))
    {
        std::wcout << L"LoadTestSuiteModule Module Fail";
        std::wcout << strDllPath << std::endl;
        return false;
    }

    pSequenceManageInterface = mTestSuitePocoClassLoader.CreateTestSuitePtrClass(std::string("CTSE_SequenceManageExport"));
    if (!pSequenceManageInterface)
    {
        std::wcout << L"CreateTestSuitePtrClass Fail" << std::endl;
        return false;
    }

    pSequenceManageImpl= pSequenceManageInterface->GetSequenceManageImpl();
    pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(MULTIUNIT_TSE_TEST,0);
    pSequenceManageImpl->SetOnLogListener(&testLog);
    pSequenceManageImpl->SetOnTestStatusListener(&testLog);
    PareseParam(argc,argv);
    std::string strSuiteConfig (mSuiteConfigPath);
    strSuiteConfig = GetEncryptedTempFilePath(mSuiteConfigPath);
    if (!pSequenceManageImpl->GetSequenceTestImpl()->LoadTSESuiteConfig(strSuiteConfig.c_str()))
    {
         if (gbIsEncryptedConfig) JGW_DeleteFile(JGW_A2W(strSuiteConfig));
        std::wcout << L"LoadSequenceTest Config Fail" << std::endl;
        return false;
    }
    if (gbIsEncryptedConfig) JGW_DeleteFile(JGW_A2W(strSuiteConfig));
    PareseParam(argc,argv);
    pSequenceManageImpl->GetSequenceTestImpl()->InitTSETestConfig();
    return true;
}



void ChangeTestParam(const wchar_t* strParam)
{
    std::vector<std::wstring> vstrTemp;
    std::wstring strTemp,strKey,strVal;
    int pos = 0;
    JGW_ParserStrW(strParam,L"&&",vstrTemp);

    for (size_t i = 0;i < vstrTemp.size();i ++)
    {
        strTemp = vstrTemp[i];
        strKey = vstrTemp[i];
        strVal = L"";
        pos = strTemp.find('=');
        if (-1 != pos)
        {
            strKey = strTemp.substr(0,pos);
            if (strTemp.length() > pos + 1) strVal = strTemp.substr(pos + 1);
            JGW_EraseLastAndFristChars(strKey,' ');
            JGW_EraseLastAndFristChars(strVal,' ');
        }
        if (bVerboseLog) std::wcout << L"Key: " << strKey << L" Val: " << strVal << std::endl;
        if (0 == strKey.compare(L"TSE_ERROR_STOP")) bErrorStop = (_ttoi(strVal.c_str()) == 1);
        if (0 == strKey.compare(L"TSE_SUITE_CONFIG_FILE_PATH")) mSuiteConfigPath = JGW_W2A(strVal);
        if (0 == strKey.compare(L"TSE_SUITE_AUTO_RUN")) bAutoRun = (_ttoi(strVal.c_str()) == 1);
        if (0 == strKey.compare(L"TSE_SUITE_AUTO_RUN_ERROR_STOP")) gbAutoRunErrorStop = (_ttoi(strVal.c_str()) == 1);
        if (0 == strKey.compare(L"TSE_SUITE_AUTO_RUN_INTERVAL")) nInterval = _ttoi(strVal.c_str());
        pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutString(strKey,strVal);
    }
}

void ExecuteTSE()
{
    std::wstring strTemp;
    while (1)
    {
        if (pSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAllSuiteTest(bErrorStop))
        {
            /*if (bVerboseLog) */std::wcout<<g_szPassFlag<<std::endl;
            gnPassRecord ++;
            gnTestCount ++;
            std::wcout << L"Pass : " << gnPassRecord << std::endl;
            std::wcout << L"Fail : " << gnFailRecord << std::endl;
            std::wcout << L"Counts : " << gnTestCount << std::endl;
        }
        else
        {
            /*if (bVerboseLog) */std::wcout<<g_szFailFlag<<std::endl;
            gnFailRecord ++;
            gnTestCount ++;
            std::wcout << L"Pass : " << gnPassRecord << std::endl;
            std::wcout << L"Fail : " << gnFailRecord << std::endl;
            std::wcout << L"Counts : " << gnTestCount << std::endl;

            if (bAutoRun && gbAutoRunErrorStop) std::wcin >> strTemp;
        }

        while(1)
        {
            if (!bAutoRun)
            {
                std::wcin >> strTemp;
            }
            else
            {
                strTemp = g_szStartFlag;
                Sleep(nInterval);
            }
            
            if (std::wstring::npos != strTemp.find(g_szStartFlag) )
            {
                const wchar_t* lpmsg = wstrcasewstr(strTemp.c_str(),L"Param:");
                if ( NULL != lpmsg )
                {
                    ChangeTestParam(lpmsg + _tcslen(L"Param:"));
                    /*nTSEDownloadPort = _ttoi(lpmsg + _tcslen(L"Port:"));
                    std::wcout << L"Download Port:" << nTSEDownloadPort << std::endl;
                    pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(TSE_DOWNLOAD_PORT_PARAM,nTSEDownloadPort);*/
                }
                break;
            }
            else if ( std::wstring::npos != strTemp.find( g_szExitFlag ) )
            {
                return;
            }
            Sleep(200);
        }
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    atexit(deinit);
    if (!InitTSE(argc,argv)) return -1;
    ExecuteTSE();
    return 0;
}