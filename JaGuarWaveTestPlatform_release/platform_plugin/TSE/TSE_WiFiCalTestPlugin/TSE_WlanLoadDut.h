#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>

namespace JGW
{
    class CTSE_WlanLoadDut : public CTSE_TestBase
    {
    public:
        CTSE_WlanLoadDut(void);
        ~CTSE_WlanLoadDut(void);
    private:     
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        NVMEM_OPTION meiNVMemOption;
        //! Device ID: 0x3c:QC98xx; 0x3d:AR6004; 0x3e:AR6174
        size_t mnSSID;
        std::string mstrDevDLLName;
        std::string mstrRefDesign;
        std::string mstrEepFName;
    };
}

