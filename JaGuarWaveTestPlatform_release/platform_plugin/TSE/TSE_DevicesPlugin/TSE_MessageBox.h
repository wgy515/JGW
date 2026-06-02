#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    //! Notice 提示对话框
    class CTSE_MessageBox : public CTSE_TestBase
    {
    public:
        CTSE_MessageBox(void);
        ~CTSE_MessageBox(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
    
        bool TSE_Run();
    private:
        DWORD mdwFlags;
        std::wstring mstrCaption;
        std::wstring mstrMsg;
    };
}