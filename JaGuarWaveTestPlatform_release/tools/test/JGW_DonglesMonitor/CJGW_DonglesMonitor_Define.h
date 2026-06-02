#pragma once

namespace JGW
{
    typedef struct
    {
        bool mbPerasoSocImpSSH;
        bool mbOpenDonglesPort;
        HWND mhMainWnd;

        size_t mnSSHPort;
        std::string mstrHost;
        std::string mstrSourceAddr;
        std::string mstrUserName;
        std::string mstrUserPassword;
        std::string mstrBinPath;

        std::wstring mstrSN;
        std::wstring mstrMac;
        std::wstring mstrFW;
        std::wstring mstrUsbSpeed;      
    }S_DONGLES_MONITOR_PARAM,*LPS_DONGLES_MONITOR_PARAM;


}