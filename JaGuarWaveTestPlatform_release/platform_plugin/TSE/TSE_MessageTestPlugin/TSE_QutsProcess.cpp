#include "StdAfx.h"
#include "TSE_QutsProcess.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CTSE_QutsProcess::CTSE_QutsProcess(void) : mbCheckQutsProcessRuning(false)
    {
    }


    CTSE_QutsProcess::~CTSE_QutsProcess(void)
    {
    }

    const wchar_t* CTSE_QutsProcess::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_QutsProcess\",\
                \"TestName\": \"Start Quts Process\",\
                \"ShowWindow\": \"0:HIDE,1:NORMAL,2:MINIIZED,3:MAXIMIZED,4:ACTIVATE,5:SHOW,6:MINIMIZE,7:MINNOACTIVE,8:SHOWNA,9:RESTORE,10:SHOWDEFAULT,11:FORCEMINIMIZE default:0\",\
                \"QutsApplicationPath\": \"quts application path\",\
                \"QutsApplicationName\": \"quts application name\"\
                }";
    }

    void CTSE_QutsProcess::StartQutsProcess()
    {
        std::wstring strTFTP32ExecFilePath = JGW_RealativePathToAbsPath(L"JGW_Quts.exe");
        if (JGW_PathFileExists(strTFTP32ExecFilePath.c_str()))
        {
            JGW_CreateProcess(strTFTP32ExecFilePath.c_str(),JGW_GetPathOfFile(strTFTP32ExecFilePath.c_str()).c_str());
        }
        //mbCheckTFPT32ProcessRuning = true;
    }

    bool CTSE_QutsProcess::TSE_Run()
    {
        if (JGW_GetProcessID2Name(L"JGW_Quts.exe") != -1)
        {
            return true;
        }

        return false;
    }
}
