#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_SimpleThread.h>
#include <process.h>
namespace JGW
{
    CCJGW_SimpleThread::CCJGW_SimpleThread(void) : m_bThreadRuning(false),mpThreadContext(NULL),mpRunThreadCallback(NULL),m_hThread(NULL),m_dThreadID(0)
    {
    }


    CCJGW_SimpleThread::~CCJGW_SimpleThread(void)
    {
    }

    bool CCJGW_SimpleThread::CreateMessageThread(RunThreadCallback lpRunThreadCallback,void* pThreadContext,bool bWaitThreadRun /* = false */)
    {
        mpRunThreadCallback = lpRunThreadCallback;
        mpThreadContext = pThreadContext;
        TerminateMessageThread();

        m_hThread = (HANDLE)_beginthreadex(NULL,0,ThreadMessageFunc,this,0,&m_dThreadID);
        if (NULL ==  m_hThread)
        {
            return false;
        }

        if ( bWaitThreadRun ) 
        {
            for (int i = 0;i < 100;i ++)
            {
                if (m_bThreadRuning) return true;
                Sleep( 10 );
            }
        }

        return true;
    }

    unsigned CCJGW_SimpleThread::ThreadMessageFunc(void *lpParam)
    {
        CCJGW_SimpleThread *lpThreadTemp = (CCJGW_SimpleThread*)lpParam;
        if (NULL != lpThreadTemp && NULL != lpThreadTemp->mpRunThreadCallback)
        {
            lpThreadTemp->m_bThreadRuning = true;
            lpThreadTemp->mpRunThreadCallback(lpThreadTemp->mpThreadContext);
            lpThreadTemp->m_bThreadRuning = false;
        }
    
        return 0;
    }

    void CCJGW_SimpleThread::TerminateMessageThread()
    {
        if (IsThreadRun())
        {
            m_bThreadRuning = false;
            TerminateThread( m_hThread,-1 );
        }
        if( m_hThread ) CloseHandle( m_hThread );
        m_hThread = NULL;
    }

    bool CCJGW_SimpleThread::IsThreadRun()
    {
        if(!m_bThreadRuning || !m_hThread ) return false;
        DWORD dwExitCode = STILL_ACTIVE;
        GetExitCodeThread(m_hThread,&dwExitCode);

        return (dwExitCode == STILL_ACTIVE);
    }
}

