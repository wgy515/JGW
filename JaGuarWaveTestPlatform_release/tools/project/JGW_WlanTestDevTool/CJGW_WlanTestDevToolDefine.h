#pragma once
#include <string>

namespace JGW
{
    typedef struct
    {
        int mnTemperatureIndex;
        HWND mhMainWindow;
        std::wstring mstrWiFiTestLogPath;
        std::wstring mstrSaveTemplatePath;
        std::wstring mstrExcelParseIniFilePath;
        std::wstring mstrExcelTemplateFilePath;
    } S_WIFI_TEST_TEMPLATE_PARAM,*LPS_WIFI_TEST_TEMPLATE_PARAM;
}