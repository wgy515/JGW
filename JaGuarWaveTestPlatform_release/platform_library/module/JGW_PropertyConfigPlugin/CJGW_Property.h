#pragma once
#include <string>
#include <unordered_map>
namespace JGW
{
    class CCJGW_Property
    {
    public:
        CCJGW_Property(const std::wstring& strPropertyFilleConfigName,std::unordered_map<std::wstring,std::wstring>& mapPropertyConfig);
        ~CCJGW_Property();
    public:
        bool WritePropertyValueToKey(const std::wstring& strKey,const std::wstring& strValue);

        void SetPropertyAppName(const std::wstring& strPropertyAppName = L"propety");

        void ReLoadPropertyFilleConfig();
    private:
        void LoadPropertyFilleConfig();
    private:
        std::wstring mstrPropertyAppName;
        std::wstring mstrPropertyFilleConfigName;
        std::wstring mstrPropertyFilleConfigPath;
        std::unordered_map<std::wstring,std::wstring>& mmapPropertyConfig;
    };
}