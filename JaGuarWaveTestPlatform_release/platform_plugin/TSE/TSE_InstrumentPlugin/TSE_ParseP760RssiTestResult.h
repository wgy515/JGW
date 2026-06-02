#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
namespace JGW
{
    class CTSE_ParseP760RssiTestResult : public CTSE_TestBase
    {
    public:
        CTSE_ParseP760RssiTestResult(void);
        ~CTSE_ParseP760RssiTestResult(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool PareseMTKRxRssiResult(const std::wstring& strRxTestResult,short& rssi);
        std::wstring FormatTestResultCommand(const std::wstring& strShowTestResultCommand);
    private:
        bool mbIsRecordMesLog;
        std::wstring mstrSaveTestResultEnvironment;
        std::wstring mstrRssiTestResultEnvironment;
        std::wstring mstrShowTestResultForamt;
        std::wstring mstrRssiRangeParamName;
        std::map<std::wstring,std::wstring> mmapParams;
        std::wstring mstrMESLogPathEnvionment;
        std::wstring mstrMESLogAppName;
        std::wstring mstrMESLogKeyName;
        CCJGW_ConfigIni mConfigIni;
    };
}

