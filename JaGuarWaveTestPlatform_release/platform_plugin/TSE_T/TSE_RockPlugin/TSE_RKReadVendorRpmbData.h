#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_RKDeviceHelp.h"
namespace JGW
{
    class CTSE_RKReadVendorRpmbData : public CTSE_TestBase
    {
    public:
        CTSE_RKReadVendorRpmbData(void);
        ~CTSE_RKReadVendorRpmbData(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        int mnDest;
        //! VENDOR_ID
        unsigned short mnVendorID;
        //! 
        bool mbIsRead;
        //! 
        int mnBinOrText;
        //!
        const ID_MAP2* mpIDMap2;
        //!
        std::wstring mstrSaveReadValueEnvironment;
    };
}

