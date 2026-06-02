#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_BuildDateTimeStamp : public CTSE_TestBase
    {
    public:
        CTSE_BuildDateTimeStamp(void);
        ~CTSE_BuildDateTimeStamp(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrDateTimeEnvironment;
        std::wstring mstrTimeStampEnvironment;
        std::wstring mstrTimeStampYMDHMSFormmat;
        std::wstring mstrTimeStampYMDHMSEnvironment;
    };

}

