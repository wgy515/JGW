#include "StdAfx.h"
#include "TSE_BuildMESLog.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

namespace JGW
{
    CTSE_BuildMESLog::CTSE_BuildMESLog(void)
    {
		TCHAR szComputerName[260] = {0};
		DWORD dwSize = 260;
		GetComputerNameW(szComputerName,&dwSize);
		mstrComputeName = szComputerName;
    }


    CTSE_BuildMESLog::~CTSE_BuildMESLog(void)
    {
    }

    const wchar_t* CTSE_BuildMESLog::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_BuildMESLog::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("SrcTempleteFilePath"),strParamName))
        {
            mstrSrcTempleteFilePath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SaveMESLogEnvionment"),strParamName))
        {
            mstrSaveMESLogEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("RecordingTimerEnvionment"),strParamName))
        {
            mstrRecordingTimerEnvionment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_BuildMESLog::TSE_Run()
    {
        std::wstring strMESLogFilePath;
        std::wstring strOperateID = JGW_A2W(GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrOperationId);
        std::wstring strTaskName = JGW_A2W(GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrTaskNumber);
        std::wstring strUserName = JGW_A2W(GetMESDataBaseAdo()->GetMESServiceSocketConnect()->msMesConnectInfo.mstrUsername);

        mTimer.restart();
        GetGlobalEnvironment()->PutInt(mstrRecordingTimerEnvionment,(int)&mTimer);
        JGW_FormatWString(strMESLogFilePath,L"%sTestLog\\CPK\\%s\\%s\\%s_%%s_%s",JGW_GetApplicationFolder(),strTaskName.c_str(),JGW_GetTimeFolder().c_str(),strOperateID.c_str(),JGW_GetCurrentTimeStampYMDHMS().c_str());
        JGW_CreateDirectory(strMESLogFilePath.c_str(),false);
        CopyFile(mstrSrcTempleteFilePath.c_str(),strMESLogFilePath.c_str(),FALSE);

        CCJGW_ConfigIni configIni;
        if (!configIni.InitIniFilePath(strMESLogFilePath.c_str()))
        {
            LogE(L"Build MES Log Fail");
            return false;
        }

        configIni.SetIniKeyValue(L"Test_Station",L"OrderID",strTaskName.c_str());
        configIni.SetIniKeyValue(L"Test_Station",L"OperationID",strOperateID.c_str());
        configIni.SetIniKeyValue(L"Test_Station",L"UserName",strUserName.c_str());
		configIni.SetIniKeyValue(L"Test_Station",L"WorkUnitID",mstrComputeName.c_str());
        GetGlobalEnvironment()->PutString(mstrSaveMESLogEnvionment,strMESLogFilePath);
        return true;
    }
}
