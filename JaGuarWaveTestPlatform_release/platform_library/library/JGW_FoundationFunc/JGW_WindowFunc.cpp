#include "stdafx.h"
#include <JGW_FoundationFunc/JGW_WindowFunc.h>

namespace JGW
{
    //刷新任务栏图标
    void JGW_RefreshTaskbarIcon()
    {
        //任务栏窗口
        HWND hShellTrayWnd = ::FindWindow(L"Shell_TrayWnd",NULL);
        //任务栏右边托盘图标+时间区
        HWND hTrayNotifyWnd = ::FindWindowEx(hShellTrayWnd,0,L"TrayNotifyWnd",NULL);
        //不同系统可能有可能没有这层
        HWND hSysPager = ::FindWindowEx(hTrayNotifyWnd,0,L"SysPager",NULL);
        //托盘图标窗口
        HWND hToolbarWindow32;
        if (hSysPager)
        {
            hToolbarWindow32 = ::FindWindowEx(hSysPager,0,L"ToolbarWindow32",NULL);
        }
        else
        {
            hToolbarWindow32 = ::FindWindowEx(hTrayNotifyWnd,0,L"ToolbarWindow32",NULL);
        }
        if (hToolbarWindow32)
        {
            RECT r;
            ::GetWindowRect(hToolbarWindow32,&r);
            int width = r.right - r.left;
            int height = r.bottom - r.top;
            //从任务栏中间从左到右 MOUSEMOVE一遍，所有图标状态会被更新
            for (int x = 1; x<width; x++)
            {
                ::SendMessage(hToolbarWindow32,WM_MOUSEMOVE,0,MAKELPARAM(x,height/2));
            }
        }
    }

    bool JGW_DetectWindowIsNotResponding(HWND hWindow)
    {
        return TRUE == IsHungAppWindow(hWindow);
#if 0
        // 判断一个窗口是否已经停止响应了（超时0.5秒）  
        DWORD_PTR dwResult = 0;  
        LRESULT lr = ::SendMessageTimeout(g_hWin, WM_NULL, 0, 0, SMTO_ABORTIFHUNG | SMTO_BLOCK, 500, &dwResult);  
        if (lr)  
        {  
            // 还可以响应  
        }  
        else  
        {  
            // 已经停止响应了（俗话说的窗口挂死了）  
        }
#endif
    }

    HWND JGW_GetForegroundWindow()
    {
        return GetForegroundWindow();
    }
}
