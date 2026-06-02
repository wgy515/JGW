#pragma once
#include "JGW_WindowsFuncPlugin_Define.h"
#include <Windows.h>

#ifdef UNICODE
#define MY_CLIP_FORMAT	CF_UNICODETEXT
#else
#define MY_CLIP_FORMAT	CF_TEXT
#endif

namespace JGW
{
    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_Clipboard
    {
    public:
        CCJGW_Clipboard(void);
        ~CCJGW_Clipboard(void);

    public:
        //! 
        static std::wstring GetClipBoardText(HWND hWnd,std::wstring& strClipBoardText);
        //! 
        static bool SetClipBoardText(const wchar_t* text,HWND hWnd);
    };
}