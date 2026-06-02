#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_IPQ807xWlanSetLocalMac : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xWlanSetLocalMac(void);
        ~CTSE_IPQ807xWlanSetLocalMac(void);
    private:
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    private:
        std::wstring mstrMacAddr1Environment;
        std::wstring mstrMacAddr2Environment;
        std::wstring mstrMacAddr3Environment;
        std::wstring mstrMacAddr4Environment;
        std::wstring mstrMacAddr5Environment;
        std::wstring mstrMacAddr6Environment;
        std::wstring mstrMacAddr7Environment;
        std::wstring mstrMacAddr8Environment;
        std::wstring mstrMacAddr9Environment;
    };
}


