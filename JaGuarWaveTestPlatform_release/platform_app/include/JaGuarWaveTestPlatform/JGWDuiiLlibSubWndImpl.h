#pragma once
#include <Windows.h>

namespace JGW
{
    typedef struct 
    {
        LPVOID		mpMainParentContainer;//! 父容器
        LPVOID		mpMainPaintManager;//! PaintManager
        LPVOID		mpPluginInfo;
        LPVOID		mpViewResolverClassLoader;
        LPVOID		mpMapRegMsg;
        LPVOID		mpWParam;
    }S_SUB_WND_IMPL_PARAM,*LPS_SUB_WND_IMPL_PARAM;

    class CJGWDuiiLlibSubWndImpl/*:public UiLib::INotifyUI*/
    {
    public:
        virtual ~CJGWDuiiLlibSubWndImpl(){}
        //! 创建DuiLib子窗体
        virtual bool CreateSubWnd( LPS_SUB_WND_IMPL_PARAM psSubWndImplParam ) = 0;
        //! 响应事件消息
        virtual void OnMsgNotify(LPVOID pNotifyMsg) = 0;
        //! 关闭子窗体
        virtual void CloseSubWndPlugin() = 0;
        //! 主窗体事件
        virtual void OnParentCustomMessage( const UINT& uMsg,const WPARAM& wParam,const LPARAM& lParam ) {}
    };
}