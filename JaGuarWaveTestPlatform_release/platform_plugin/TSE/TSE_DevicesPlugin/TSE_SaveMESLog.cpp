#include "StdAfx.h"
#include "TSE_SaveMESLog.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
namespace JGW
{
    CTSE_SaveMESLog::CTSE_SaveMESLog(void)
    {
    }


    CTSE_SaveMESLog::~CTSE_SaveMESLog(void)
    {
    }

    const wchar_t* CTSE_SaveMESLog::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_SaveMESLog::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("SrcSaveMESLogEnvionment"),strParamName))
        {
            mstrSrcSaveMESLogEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SaveMESLogEnvionment"),strParamName))
        {
            mstrSaveMESLogEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("RecordingTimerEnvionment"),strParamName))
        {
            mstrRecordingTimerEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SerialNumberEnvionment"),strParamName))
        {
            mstrSerialNumberEnvionment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SaveMESLog::TSE_Run()
    {
        std::wstring strSrcSaveMESLogPath = GetGlobalEnvironment()->GetString(mstrSrcSaveMESLogEnvionment);
        std::wstring strSaveMESLogPath;
        std::wstring strSerialNumber = GetGlobalEnvironment()->GetString(mstrSerialNumberEnvionment);

        bool bTestResult = GetGlobalEnvironment()->GetBool(TSE_TEST_RESULT);
        JGW_FormatWString(strSaveMESLogPath,strSrcSaveMESLogPath.c_str(),strSerialNumber.c_str());
        strSaveMESLogPath += bTestResult ? L"_PASS":L"_FAIL";
        strSaveMESLogPath += L".txt";

        if (!MoveFile(strSrcSaveMESLogPath.c_str(),strSaveMESLogPath.c_str()))
        {
            LogE_F(L"Save MES Log Fail(%s)",JGW_GetSystemErrorString(GetLastError()).c_str());
            return false;
        }

        if (!JGW_FileExistsToFilePath(strSaveMESLogPath.c_str()))
        {
            LogE(L"MES Log Move File Fail");
            return false;
        }

        jgw_timer* pTimer = (jgw_timer*)GetGlobalEnvironment()->GetInt(mstrRecordingTimerEnvionment);
        if (pTimer == NULL)
        {
            LogE(L"Get Record Timer Fail");
            return false;
        }

        CCJGW_ConfigIni configIni;
        if (!configIni.InitIniFilePath(strSaveMESLogPath.c_str()))
        {
            LogE(L"Build MES Log Fail");
            return false;
        }

        configIni.SetIniKeyValue(L"LotInfo",L"TestStartTime",JGW_GetCurrentTimeStampYMDHMS(L"%04d-%02d-%02d %02d:%02d:%02d").c_str());
        configIni.SetIniKeyValue(L"LotInfo",L"TestSN",strSerialNumber.c_str());
        configIni.SetIniKeyValue(L"LotInfo",L"TestTotTime",JGW_GetFormatWString(L"%.2lf",pTimer->elapsed()).c_str());
        configIni.SetIniKeyValue(L"LotInfo",L"TestResult",bTestResult?L"PASS":L"FAIL");
        GetGlobalEnvironment()->PutString(mstrSaveMESLogEnvionment,strSaveMESLogPath);

        return true;
    }
}
