#include "StdAfx.h"
#include "CJGW_TestRecordConfig.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CCJGW_TestRecordConfig::CCJGW_TestRecordConfig(void) : mbSaveTestLog(false),mnTestPassCount(0),mnTestCounts(0)
    {
    }


    CCJGW_TestRecordConfig::~CCJGW_TestRecordConfig(void)
    {
    }

    bool CCJGW_TestRecordConfig::GetIsSaveTestLog()
    {
        return mbSaveTestLog;
    }

    void CCJGW_TestRecordConfig::SetIsSaveTestLog(bool isSave)
    {
        mbSaveTestLog = isSave;
        mConfigIni.SetIniKeyValue(L"MAIN",L"SaveTestLog",mbSaveTestLog);
    }

    bool CCJGW_TestRecordConfig::LoadTestRecordConfig(const wchar_t* ModuleFolder,const wchar_t* strRecordConfigName)
    {
        std::wstring strRecordConfigPath;
        JGW_GetConfigExecSubFolder(strRecordConfigPath,ModuleFolder);
        if (NULL == strRecordConfigName)
        {
            strRecordConfigPath += _T(TEST_RECORD_CONFIG_NAME);
        }
        else
        {
            strRecordConfigPath += strRecordConfigName;
            strRecordConfigPath += L".data";
        }


        if (!mConfigIni.InitIniFilePath(strRecordConfigPath.c_str())) return false;

        mbSaveTestLog = mConfigIni.GetIniKeyBoolValue(L"MAIN",L"SaveTestLog",true);
        mnTestCounts = mConfigIni.GetIniKeyIntValue(L"MAIN",L"Counts");
        mnTestPassCount = mConfigIni.GetIniKeyIntValue(L"MAIN",L"Pass");

        return true;
    }

    size_t CCJGW_TestRecordConfig::GetTestRecordCount()
    {
        return mnTestCounts;
    }

    size_t CCJGW_TestRecordConfig::GetTestRecordPassCount()
    {
        return mnTestPassCount;
    }

    void CCJGW_TestRecordConfig::ClearTestRecord()
    {
        mnTestPassCount = 0;
        mnTestCounts = 0;
        mConfigIni.SetIniKeyValue(L"MAIN",L"Counts",0);
        mConfigIni.SetIniKeyValue(L"MAIN",L"Pass",0);
    }

    void CCJGW_TestRecordConfig::UpdateTestRecordToTestFail()
    {
        mnTestCounts++;
        mConfigIni.SetIniKeyValue(L"MAIN",L"Counts",mnTestCounts);
    }

    void CCJGW_TestRecordConfig::UpdateTestRecordToTestPass()
    {
        mnTestCounts ++;
        mnTestPassCount ++;
        mConfigIni.SetIniKeyValue(L"MAIN",L"Counts",mnTestCounts);
        mConfigIni.SetIniKeyValue(L"MAIN",L"Pass",mnTestPassCount);
    }
}
