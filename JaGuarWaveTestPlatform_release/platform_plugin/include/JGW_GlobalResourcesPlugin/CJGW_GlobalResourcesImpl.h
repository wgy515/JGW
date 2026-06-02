#pragma once
#include "JGW_GlobalResources_Define.h"


namespace JGW
{
    class CCJGW_GlobalResourcesImpl
    {
    public:
        virtual void* GetGlobalResourcesValue(const std::wstring& strGlobalResourcesName) = 0;

        virtual void SetGlobalResourcesValue(const std::wstring& strGlobalResourcesName,void* pGlobalResourcesValue) = 0;

    };


#ifdef __cplusplus
    extern "C" 
    {
#endif
        JGW_GLOBALRESOURCES_API CCJGW_GlobalResourcesImpl* GetGlobalResourcesImpl();

#ifdef __cplusplus
    };
#endif
}