#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_SaharaOpenPort : public CTSE_TestBase
    {
    public:
        CTSE_SaharaOpenPort(void);
        ~CTSE_SaharaOpenPort(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! Sahara hello Packet mode, when mode = 0, this indicates a SAHARA_MODE_IMAGE_TX_PENDING mode when mode = 2, this indicates a SAHARA_MODE_MEMORY_DEBUG mode
        int mnMode;
        //! 
        size_t mnTimeOutSec;
        //! 
        std::wstring mstrResourceContextEnv;
        //! 
        std::wstring mstrPortEnv;
    };

}

