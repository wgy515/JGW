#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_ReadStringNVX20 : public CTSE_TestBase
    {
    public:
        CTSE_ReadStringNVX20(void);
        ~CTSE_ReadStringNVX20(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! 数据长度
        size_t mnDataLen;
        //! 分隔符
        std::wstring mstrSeparator;
        //! NV原始数据以逗号分隔
        std::wstring mstrNVRawDataEnvrionment;
        //!
        std::wstring mstrSaveDataEnvrionment;
    };
}