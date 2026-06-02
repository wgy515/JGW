#pragma once

#include "JGW_FoundationFuncLib_Define.h"

namespace JGW
{
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    extern "C" 
    {
#endif
        //! 刷新任务栏图标
        JGW_FOUNDATIONFUNC_LIB_API void JGW_RefreshTaskbarIcon();
        //! 检测窗口是否为未响应。确定系统是否认为指定的应用程序没有响应。 如果应用程序未等待输入、未在启动处理中且未在 5 秒的内部超时期限内调用 PeekMessage ，则应用程序被视为未响应。Detects whether the window is not responding
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_DetectWindowIsNotResponding(HWND hWindow);
        //! 获取焦点窗口句柄 检索前台窗口的句柄， (用户当前正在使用) 窗口。 系统为创建前台窗口的线程分配的优先级略高于其他线程的优先级 返回值是前台窗口的句柄。 在某些情况下（例如，当窗口丢失激活时），前台窗口可以为 NULL 。
        JGW_FOUNDATIONFUNC_LIB_API HWND JGW_GetForegroundWindow();
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    }
#endif
}
