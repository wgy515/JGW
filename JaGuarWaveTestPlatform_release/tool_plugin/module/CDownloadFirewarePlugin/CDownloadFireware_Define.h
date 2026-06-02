#pragma once
#define DOWNLOAD_FIREWARE_TIME_OUT_SEC 120
#define DOWNLOAD_FIREWARE_TOOL_NAME L"prs_download_flash.exe"
namespace JGW
{
    typedef struct 
    {
        HWND mhMainWnd;
        std::wstring mstrFirewareFolder;
        std::wstring mstrFirewareToolFilePath;
        std::wstring mstrCMDFilePath;
    }S_DOWNLOAD_FIREWARE_PARAM,*LPS_DOWNLOAD_FIREWARE_PARAM;
}