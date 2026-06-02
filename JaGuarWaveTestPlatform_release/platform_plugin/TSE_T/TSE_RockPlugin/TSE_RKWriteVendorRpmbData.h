#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_RKDeviceHelp.h"

namespace JGW
{
    class CTSE_RKWriteVendorRpmbData : public CTSE_TestBase
    {
    public:
        CTSE_RKWriteVendorRpmbData(void);
        ~CTSE_RKWriteVendorRpmbData(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbIsWrite;
        //! VENDOR_ID
        unsigned short mnVendorID;
        //! /* type 0:str 1:decimal 2:hex*/
        int mnBinOrText;
        //! vendor(0) or rpmb(1)
        int mnDest;
        //! 
        const ID_MAP2* mpIDMap2;
        //!
        std::wstring mstrWriteValueCommand;
    };
}
