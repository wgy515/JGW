#pragma once
#include <unordered_map>

namespace JGW
{
    class CCJGW_PropertyConfig
    {
    public:
        CCJGW_PropertyConfig(void);
        ~CCJGW_PropertyConfig(void);

    private:
        std::unordered_map<std::wstring,std::wstring> mmapPropertyConfig;
    };
}