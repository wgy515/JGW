#pragma once
#include <JGW_GlobalResourcesPlugin/CJGW_GlobalResourcesImpl.h>
#include <map>

namespace JGW
{
    class CCJGW_GlobalResources : public CCJGW_GlobalResourcesImpl
    {
    public:
        CCJGW_GlobalResources(void);
        ~CCJGW_GlobalResources(void);
    private:
        void* GetGlobalResourcesValue(const std::wstring& strGlobalResourcesName);
        void SetGlobalResourcesValue(const std::wstring& strGlobalResourcesName,void* pGlobalResourcesValue);
    private:
        std::map<std::wstring,void*> mmapGlobalResources;
    };
}