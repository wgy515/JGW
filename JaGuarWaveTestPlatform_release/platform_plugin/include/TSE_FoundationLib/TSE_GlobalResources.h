#pragma once
#include "TSE_TestBase.h"

namespace JGW
{
    class CTSE_GlobalResources : public CTSE_TestBase
    {
    public:
        CTSE_GlobalResources();
//     public:
//         CTSE_GlobalEnvironmentImpl* GetGlobalEnvironment();
//         CTSE_LogServicesImpl* GetLogServices();
//         CTSE_TestStatusServiceImpl* GetTestStatusService();
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strName,const wchar_t* strValue);
    public:
        static CTSE_GlobalEnvironmentImpl* mpGlobalEnvironmentImpl;
        static CTSE_LogServicesImpl* mpLogServicesImpl;
        static CTSE_TestStatusServiceImpl* mpTestStatusServiceImpl;
    };
}