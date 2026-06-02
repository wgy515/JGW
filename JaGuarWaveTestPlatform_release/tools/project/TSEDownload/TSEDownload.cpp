// TSEDownload.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <Windows.h>
#include <iostream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageInterface.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
#include <TSE_SequenceManagerPlugin/TSE_LogListener.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
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

const wchar_t g_szStartFlag[] = L"{D261BC0C-45F3-498D-AEE8-5E730691CF35}";
const wchar_t g_szPassFlag[] = L"{A6A28E32-E91C-40D0-A79B-06CC8B441601}";
const wchar_t g_szFailFlag[] = L"{241BC6EF-0CD0-49CE-9B39-EEA67691669C}";
const wchar_t g_szExitFlag[] = L"{65FDA37A-8DFE-4540-9611-255A5C823649}";

bool bDownloadAfterResetEnable = false;
bool bErrorStop = true;
int nTSEDownloadPort = 6;
std::string mSuiteConfigPath = "C:\\JW\\suite.xml";
std::wstring mSaharaFirehoseImage = L"prog_ufs_firehose_8996_ddr.elf";
std::wstring mProvisionXmlName = L"provision_samsung.xml";
std::wstring mstrDownloadPath = L"c:\\JW\\cache\\TSEDownload\\out-bin\\";
std::wstring mstrRawProgramXmlNames = L"rawprogram_unsparse0.xml,rawprogram_unsparse4.xml,rawprogram1.xml,rawprogram2.xml,rawprogram3.xml,rawprogram5.xml";
std::wstring mstrPathXmlNames = L"patch0.xml,patch1.xml,patch2.xml,patch3.xml,patch4.xml,patch5.xml";

enum E_TSE_DOWNLOAD_PARAM 
{
    E_NULL_PARAM = 0,
    E_TSS_SUITE_CONFIG_PATH = 1,
    E_TSE_DOWNLOAD_PORT_PARAM = 2,
    E_TSE_DOWNLOAD_PATH_PARAM,
    E_TSE_SAHARA_FIREHOSE_IMAGE_PARAM,
    E_TSE_DOWNLOAD_PROVISION_XML_NAME_PARAM, 
    E_TSE_RAWPROGRAM_XML_NAMES_PARAM,
    E_TSE_PATCH_XML_NAMES_PARAM,
    E_TSE_RESET_ENABLE_PARAM,
    E_TSE_ERROR_STOP
};

void PareseParam(int argc, _TCHAR* argv[])
{
    if (argc > E_TSS_SUITE_CONFIG_PATH)
    {
        mSuiteConfigPath = JGW_W2A_W(argv[E_TSS_SUITE_CONFIG_PATH]);
        std::wcout << L"Suite Config Path:" << argv[E_TSS_SUITE_CONFIG_PATH] << std::endl;
    }

    if (argc > E_TSE_DOWNLOAD_PORT_PARAM)
    {
        nTSEDownloadPort = _ttoi(argv[E_TSE_DOWNLOAD_PORT_PARAM]);
        std::wcout << L"Download Port:" << argv[E_TSE_DOWNLOAD_PORT_PARAM] << std::endl;
    }
    pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(TSE_DOWNLOAD_PORT_PARAM,nTSEDownloadPort);

    if (argc > E_TSE_DOWNLOAD_PATH_PARAM)
    {
        mstrDownloadPath = argv[E_TSE_DOWNLOAD_PATH_PARAM];
        std::wcout << L"Download Path:" << argv[E_TSE_DOWNLOAD_PATH_PARAM] << std::endl;
    }
    pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutString(TSE_DOWNLOAD_PATH_PARAM,mstrDownloadPath);

    if (argc > E_TSE_SAHARA_FIREHOSE_IMAGE_PARAM)
    {
        mSaharaFirehoseImage = argv[E_TSE_SAHARA_FIREHOSE_IMAGE_PARAM];
        std::wcout << L"Sahara Firehose Image:" << argv[E_TSE_SAHARA_FIREHOSE_IMAGE_PARAM] << std::endl;
    }
    pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutString(TSE_SAHARA_FIREHOSE_IMAGE_PARAM,mSaharaFirehoseImage);

    if (argc > E_TSE_DOWNLOAD_PROVISION_XML_NAME_PARAM)
    {
        mProvisionXmlName = argv[E_TSE_DOWNLOAD_PROVISION_XML_NAME_PARAM];
        std::wcout << L"Provision Xml Name:" << argv[E_TSE_DOWNLOAD_PROVISION_XML_NAME_PARAM] << std::endl; 
    }
    pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutString(TSE_DOWNLOAD_PROVISION_XML_NAME_PARAM,mProvisionXmlName);

    if (argc > E_TSE_RAWPROGRAM_XML_NAMES_PARAM)
    {
        mstrRawProgramXmlNames = argv[E_TSE_RAWPROGRAM_XML_NAMES_PARAM];
        std::wcout << L"RawProgram Xml Name:" << argv[E_TSE_RAWPROGRAM_XML_NAMES_PARAM] << std::endl;
        
    }
    pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutString(TSE_RAWPROGRAM_XML_NAMES_PARAM,mstrRawProgramXmlNames);

    if (argc > E_TSE_PATCH_XML_NAMES_PARAM)
    {
        mstrPathXmlNames = argv[E_TSE_PATCH_XML_NAMES_PARAM];
        std::wcout << L"Patch Xml Name:" << argv[E_TSE_PATCH_XML_NAMES_PARAM] << std::endl;
    }
    pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutString(TSE_PATCH_XML_NAMES_PARAM,mstrPathXmlNames);

    if (argc > E_TSE_RESET_ENABLE_PARAM)
    {
        bDownloadAfterResetEnable = (1 == _ttoi(argv[E_TSE_RESET_ENABLE_PARAM]));
        std::wcout << L"DownloadAfterReset:" << argv[E_TSE_RESET_ENABLE_PARAM] << std::endl; 
    }
    pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutBool(TSE_RESET_ENABLE_PARAM,bDownloadAfterResetEnable);

    if (argc > E_TSE_ERROR_STOP)
    {
        bErrorStop = (1 == _ttoi(argv[E_TSE_ERROR_STOP]));
        std::wcout << L"Error Stop Flag:" << argv[E_TSE_ERROR_STOP] << std::endl;
    }
}

class CTestLog : public CTSE_LogListener,public CTSE_TestStatusListener
{
public:
    void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
    {
        std::wcout << strTestName <<"  "<< gTestStatus[eTestStatus] << std::endl;
    }

    void OnInfoLog(const wchar_t* strInfoLog)
    {
        std::wcout << strInfoLog;
    }

    void OnErrorLog(const wchar_t* strErrorLog)
    {
        std::wcout << "ERROR:" << strErrorLog;
    }

    void OnDebugLog(const wchar_t* strDebugLog)
    {
        std::wcout << strDebugLog;
    }
};

CTestLog testLog;

void deinit()
{
    std::wcout << L"Exit Download Application"<< std::endl;
    mTestSuitePocoClassLoader.UnloadTestSuiteModule();
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
    PareseParam(argc,argv);
    pSequenceManageImpl->SetOnLogListener(&testLog);
    pSequenceManageImpl->SetOnTestStatusListener(&testLog);
    if (!pSequenceManageImpl->GetSequenceTestImpl()->LoadTSESuiteConfig(mSuiteConfigPath.c_str()))
    {
        std::wcout << L"LoadSequenceTest Config Fail" << std::endl;
        return false;
    }
    pSequenceManageImpl->GetSequenceTestImpl()->InitTSETestConfig();
    return true;
}

void ExecuteTSE()
{
    std::wstring strTemp;
    while (1)
    {
        if (pSequenceManageImpl->GetSequenceTestImpl()->ExecTSEAllSuiteTest(bErrorStop))
        {
            std::wcout<<g_szPassFlag<<std::endl;
        }
        else
        {
            std::wcout<<g_szFailFlag<<std::endl;
        }

        while(1)
        {
            std::wcin >> strTemp;
            if (std::wstring::npos != strTemp.find(g_szStartFlag) )
            {
                const wchar_t* lpmsg = wstrcasewstr(strTemp.c_str(),L"Port:");
                if ( NULL != lpmsg )
                {
                    nTSEDownloadPort = _ttoi(lpmsg + _tcslen(L"Port:"))/* * 1024.00f*/;
                    std::wcout << L"Download Port:" << nTSEDownloadPort << std::endl;
                    pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(TSE_DOWNLOAD_PORT_PARAM,nTSEDownloadPort);
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