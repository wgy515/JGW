#pragma once

namespace JGW
{
    class CCJGW_TSEUIConfig
    {
    public:
        CCJGW_TSEUIConfig(void);
        ~CCJGW_TSEUIConfig(void);
    public:
        void LoadTSEUIConfig(const std::wstring& strTSETUIConfigPath,S_UI_CONFIG_INFO& msUIConfigInfo);
    };
}

