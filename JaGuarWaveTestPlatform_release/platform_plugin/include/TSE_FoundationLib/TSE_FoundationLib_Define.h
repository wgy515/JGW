#pragma once

#include <TSE_SequenceManagerPlugin/TSE_GlobalEnvironmentImpl.h>
#include <TSE_SequenceManagerPlugin/TSE_LogServicesImpl.h>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusServiceImpl.h>
#include <Windows.h>

#ifdef _DEBUG
#define LogD GetLogServices()->DebugLog
#define LogD_F GetLogServices()->DebugLogFormat
#else
#define LogD //! GetLogServices()->DebugLog
#define LogD_F //! GetLogServices()->DebugLogFormat
#endif
 

#define LogI GetLogServices()->InfoLog
#define LogI_F GetLogServices()->InfoLogFormat
#define LogE GetLogServices()->ErrorLog
#define LogE_F GetLogServices()->ErrorLogFormat
#define LogU GetLogServices()->UserDefinedLog
#define LogU_F GetLogServices()->UserDefinedLogFormat
/*#define LogU_F GetLogServices()->ErrorLogFormat*/
#define TestStatus GetTestStatusService()->TestStatusChange

namespace JGW
{
    extern CTSE_GlobalEnvironmentImpl* GetGlobalEnvironment();

    extern CTSE_LogServicesImpl* GetLogServices();

    extern CTSE_TestStatusServiceImpl* GetTestStatusService();

    extern void GetEnvironmentCommandString(std::wstring& strCommand);

    extern HWND GetGlobalMainHwnd();
}