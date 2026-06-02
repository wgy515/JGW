#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_WriteStringNVX20 : public CTSE_TestBase
    {
    public:
        CTSE_WriteStringNVX20(void);
        ~CTSE_WriteStringNVX20(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! 数据长度
        size_t mnDataLen;
        //! 分隔符
        std::wstring mstrSeparator;
        std::wstring mstrWriteStringNVCommand;
        std::wstring mstrSaveWriteStringNVEnvrionment;
    };
}

