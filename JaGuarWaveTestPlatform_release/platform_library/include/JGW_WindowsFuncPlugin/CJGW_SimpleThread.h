#pragma once
#include "JGW_WindowsFuncPlugin_Define.h"

namespace JGW
{
    typedef void (*RunThreadCallback)(void *lpParam);

    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_SimpleThread
    {
    public:
        CCJGW_SimpleThread(void);
        ~CCJGW_SimpleThread(void);
    public:
        //! 创建消息线程 CREATE_SUSPENDED TIMEOUT 1S
        bool CreateMessageThread(RunThreadCallback lpRunThreadCallback,void* pThreadContext,bool bWaitThreadRun = false);

        void TerminateMessageThread();
    public:
        //! 判断当前线程是否正在运行
        bool IsThreadRun();
    private:
        static unsigned _stdcall ThreadMessageFunc(void *lpParam);
    public:
        HANDLE m_hThread;
        unsigned int m_dThreadID;
    private:
        bool m_bThreadRuning;
        void* mpThreadContext;
        RunThreadCallback mpRunThreadCallback;
    };

}

