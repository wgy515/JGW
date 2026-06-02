#pragma once
#include <unordered_map>
#include "CJGW_PropertyMemoryConfig.h"
#include "CJGW_Property.h"

namespace JGW
{
    class CCJGW_PropertyFileConfig : public CCJGW_PropertyMemoryConfig
    {
    public:
        CCJGW_PropertyFileConfig(const std::wstring& strPropertyFilleConfigName);
        virtual ~CCJGW_PropertyFileConfig(void);
    public:
        virtual void SetPropertyAppName(const std::wstring& strPropertyAppName = L"propety");
    private:
        virtual void PutString(const std::wstring& itemName,const std::wstring& itemValue);
        virtual void ReloadPropertyFile();
    private:
        std::wstring mstrPropertyFilleConfigName;
        CCJGW_Property mProperty;
    };
}