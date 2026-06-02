#pragma once

namespace JGW
{
    typedef struct
    {
        bool mbOpenDonglesPort;
        HWND mhMainWnd;
        std::wstring mstrSN;
        std::wstring mstrMac;
        std::wstring mstrFW;
        std::wstring mstrUsbSpeed;      
    }S_DONGLES_MONITOR_PARAM,*LPS_DONGLES_MONITOR_PARAM;
}