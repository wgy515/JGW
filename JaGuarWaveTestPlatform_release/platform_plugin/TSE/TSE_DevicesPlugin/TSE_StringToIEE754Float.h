#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_StringToIEE754Float : public CTSE_TestBase
    {
    public:
        CTSE_StringToIEE754Float(void);
        ~CTSE_StringToIEE754Float(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! 是否十六进制字符串
        bool mbIsHexString;
        std::wstring mstrIEE754StringEnvrionment;
        std::wstring mstrSaveFloatEnvironment;
    };
}