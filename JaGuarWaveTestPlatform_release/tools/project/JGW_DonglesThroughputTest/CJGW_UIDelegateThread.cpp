#include "StdAfx.h"
#include "CJGW_UIDelegateThread.h"

namespace JGW
{
    CCJGW_UIDelegateThread::CCJGW_UIDelegateThread(void)
    {
    }


    CCJGW_UIDelegateThread::~CCJGW_UIDelegateThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_UIDelegateThread,CCMessageThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_UIDelegateThread::BeginInvoke(CWindowWnd* mpCWindowWndClassPtr,UIDelegateThreadFunc muiDelegateThreadFunc,DWORD mdwMessageID,WPARAM wParam /* = NULL */,LPARAM lParam /* = NULL */)
    {
        S_UIDelegateThread sUIDelegateThread = {mpCWindowWndClassPtr,muiDelegateThreadFunc,mdwMessageID,wParam,lParam};
        msUIDelegateThreadFuncQueue.put(sUIDelegateThread);
    }

    void CCJGW_UIDelegateThread::RunThread()
    {
        if (msUIDelegateThreadFuncQueue.is_empty()) return;
        S_UIDelegateThread sUIDelegateThread = msUIDelegateThreadFuncQueue.get();
        (sUIDelegateThread.mpCWindowWndClassPtr->*(sUIDelegateThread.muiDelegateThreadFunc))(sUIDelegateThread.mwParam,sUIDelegateThread.mlParam);
        PostMessage(sUIDelegateThread.mpCWindowWndClassPtr->GetHWND(),sUIDelegateThread.mdwMessageID,sUIDelegateThread.mwParam,sUIDelegateThread.mlParam);
    }
}