#pragma once
#include "TSE_FTM_TestBase.h"
#include <map>

namespace JGW
{
    class CTSE_FTM_CheckVersion : public CTSE_FTM_TestBase
    {
    public:
        CTSE_FTM_CheckVersion(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run(void);
    private:
        bool CheckAttribute(CCJGW_FTM_Json& ftmJson,const char* strAttrName,const std::string& strCompareValue,const char* strDefalutName = NULL);
    private:
        std::string mstrClientVersion;
        std::string mstrBuildNumber;
        std::string mstrModemVersion;
        std::string mstrGSMRFCalFlag;
        std::string mstrWCDMARFCalFlag;
        std::string mstrLTERFCalFlag;
        std::string mstrGSMVFSFlag;
        std::string mstrWCDMAVFSFlag;
        std::string mstrLTEVFSFlag;
        std::map<std::string,std::string> mmapUserDefineCompare;
    };
}