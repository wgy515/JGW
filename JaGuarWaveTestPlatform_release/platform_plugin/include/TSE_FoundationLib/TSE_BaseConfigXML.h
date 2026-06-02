#pragma once
#include <tinyxml/tinyxml.h>
#include <vector>

namespace JGW
{
    typedef struct
    {
		std::wstring mstrRateValue;//! 倍率
        std::wstring mstrLabelName; //! 配置名称
        //! std::wstring mstrEditValue; //! 默认配置属性
        std::wstring mstrValueEnvironment; //! 属性名称
    }S_TSE_BASE_ITEM,*LPS_TSE_BASE_ITEM;

    class CTSE_BaseConfigXML
    {
    public:
        CTSE_BaseConfigXML(std::vector<S_TSE_BASE_ITEM>& vTSEBaseItems);
        ~CTSE_BaseConfigXML(void);
    public:
        void SaveLanSuiteConfig(const std::string& strSuiteConfigPath);
    private:
        std::vector<S_TSE_BASE_ITEM>& mvTSEBaseItems;
    };
}