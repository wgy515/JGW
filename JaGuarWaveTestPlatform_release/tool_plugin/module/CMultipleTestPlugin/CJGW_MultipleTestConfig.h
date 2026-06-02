#pragma once
#include "CMultipleTestPluginDefine.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

namespace JGW
{
    class CCJGW_MultipleTestConfig
    {
    public:
        CCJGW_MultipleTestConfig(void);
        ~CCJGW_MultipleTestConfig(void);
    public:
        bool LoadMultipleTestConfig(const std::wstring& strModulePath,S_MULTIPLE_CONFIG& sMultipleConfig);
        CCJGW_ConfigIni& GetConfigIniInstance(); 
    public:
        void SaveMultipleTestConfig(S_MULTIPLE_CONFIG& sMultipleConfig);
        void SettingTftp32BaseDirectory(const std::wstring& strBaseDirectory);
    private:
        std::wstring mstrTftpd32ConfigIni;
        std::wstring mstrMultipleTestConfigPath;
        CCJGW_ConfigIni mConfigINI;
    };
}

