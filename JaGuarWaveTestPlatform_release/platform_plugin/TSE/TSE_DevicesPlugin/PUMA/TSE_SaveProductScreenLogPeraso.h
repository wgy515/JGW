#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_TestLoggingPlugin/CJGW_MoveTestLogging.h>
namespace JGW
{
    class CTSE_SaveProductScreenLogPeraso : public CTSE_TestBase
    {
    public:
        CTSE_SaveProductScreenLogPeraso(void);
        ~CTSE_SaveProductScreenLogPeraso(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Init();
    private:
        bool GetSerialNumber();
    private:
        char* mszTestResultBuf;
        std::wstring mstrSerialNumber;
        std::string mstrSerialNumberPos;
        std::wstring mstrLogFolder;
        std::wstring mstrProductScreenToolReport;
        std::wstring mstrProductScreenToolVerboseFilePath;
        std::string mstrTestResultFilePath;
        std::wstring mstrSNEnvironment;
        CCJGW_MoveTestLogging mMoveTestLoggin;
    };
}