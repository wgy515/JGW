#include "StdAfx.h"
#include "CJGW_TSEUIConfig.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_TSEUIConfig::CCJGW_TSEUIConfig(void)
    {
    }


    CCJGW_TSEUIConfig::~CCJGW_TSEUIConfig(void)
    {
    }

    void CCJGW_TSEUIConfig::LoadTSEUIConfig(const std::wstring& strTSETUIConfigPath,S_UI_CONFIG_INFO& msUIConfigInfo)
    {
        CCJGW_ConfigIni configINI;
        configINI.InitIniFilePath(strTSETUIConfigPath.c_str());
        msUIConfigInfo.mstrUISkinXmlFileName = configINI.GetIniKeyValue(L"MAIN",L"UISkinXmlFileName");
        std::wstring strTemp = configINI.GetIniKeyValue(L"MAIN",L"ControlNames");
        JGW_ParserStrW(strTemp.c_str(),L",",msUIConfigInfo.mvControlNames);
    }
}
