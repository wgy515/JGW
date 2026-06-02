#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_FoundationFunc/queue_buffer.hpp>
#include <queue>

namespace JGW
{
    //! 委托线程
    typedef void (CWindowWnd::*UIDelegateThreadFunc)(WPARAM& wParam,LPARAM& lParam);
    //! 线程委托函数
    typedef struct  
    {
        CWindowWnd* mpCWindowWndClassPtr; //! 窗口类指针
        UIDelegateThreadFunc muiDelegateThreadFunc;//! 异步回调函数
        DWORD       mdwMessageID; //! 异步函数执行完成后发送给主窗口消息ID
        WPARAM      mwParam; //! 传参
        LPARAM      mlParam; //! 传参
    }S_UIDelegateThread;

    class CCJGW_UIDelegateThread : public CCMessageThread
    {
    public:
        CCJGW_UIDelegateThread(void);
        ~CCJGW_UIDelegateThread(void);
        //! 开始异步调用
        void BeginInvoke(CWindowWnd* mpCWindowWndClassPtr,UIDelegateThreadFunc muiDelegateThreadFunc,DWORD mdwMessageID,WPARAM wParam = NULL,LPARAM lParam = NULL);
    private:
         virtual void RunThread();
         DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        queue_buffer<S_UIDelegateThread> msUIDelegateThreadFuncQueue;
    };
}