#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <process.h>
#include <comdef.h>

namespace JGW
{
    CCMessageThread::CCMessageThread(void):m_hThread(NULL)
        ,m_dThreadID(-1)
        ,m_bThreadStatus(E_MESSAGE_THREAD_EXIT)
        ,m_nThreadSleepTimeInterval(20)
    {
    }


    CCMessageThread::~CCMessageThread(void)
    {
        ExitThread();
        TerminateMessageThread();
    }

    void CCMessageThread::TerminateMessageThread()
    {
        if (IsThreadRun())
        {
            SetThreadStatus( E_MESSAGE_THREAD_EXIT );
            TerminateThread( m_hThread,-1 );
        }
        if( m_hThread ) CloseHandle( m_hThread );
        m_hThread = NULL;
    }

    void CCMessageThread::SetThreadStatus( E_MESSAGE_THREAD_STATUS eThreadStatus )
    {
        if ( E_MESSAGE_THREAD_EXIT == m_bThreadStatus && E_MESSAGE_THREAD_INIT != eThreadStatus ) return;
        m_bThreadStatus = eThreadStatus;
    }

    bool CCMessageThread::CreateMessageThread( unsigned stacksize /* = 0 */,unsigned createflag /* = 0 */,bool bWaitThreadRun /* = false */ )
    {
        SetThreadStatus( E_MESSAGE_THREAD_INIT );
        m_hThread = (HANDLE)_beginthreadex(NULL,stacksize,ThreadMessageFunc,this,createflag,&m_dThreadID);
        if (NULL ==  m_hThread)
        {
            SetThreadStatus( E_MESSAGE_THREAD_EXIT );
            return false;
        }

        if ( bWaitThreadRun ) 
        {
            for (int i = 0;i < 100;i ++)
            {
                if (E_MESSAGE_THREAD_RUNING == m_bThreadStatus) return true;
                Sleep( 10 );
            }
        }

        return true;
    }

    unsigned CCMessageThread::ThreadMessageFunc(void *lpParam)
    {
        if( !lpParam ) return -1;
        CCMessageThread *lpThreadTemp = (CCMessageThread*)lpParam;
        lpThreadTemp->DispatchMessageFunc();

        return 0;
    }

    void CCMessageThread::DispatchMessageFunc()
    {
        MSG msg = {0};
        const AFX_THREAD_MSGMAP* lpWndMessageMap = GetMessageMap();
        const S_THREAD_MESSAGE* lpWndMessage = NULL;

        SetThreadStatus( E_MESSAGE_THREAD_RUNING );
        if ( !lpWndMessageMap /*|| !iCount*/)return;

        while ( E_MESSAGE_THREAD_RUNING == m_bThreadStatus )
        {
            RunThread();
            if (FALSE != PeekMessage(&msg,NULL,WM_USER,WM_USER + 10000,PM_REMOVE))
            {			
                lpWndMessageMap = GetMessageMap();
                while ( lpWndMessageMap )
                {
                    lpWndMessage = lpWndMessageMap->lpEntries;

                    for ( int i = 0; NULL != lpWndMessage[i].mssageID;i ++ )
                    {
                        if ( NULL == lpWndMessage[i].myFunc ) continue;
                        if (msg.message == lpWndMessage[i].mssageID)
                        {
                            try
                            {
                                (this->*(lpWndMessage[i].myFunc))(msg.wParam,msg.lParam);
                            }
                            catch (_com_error& e)                                   
                            {                                                     
                                ::MessageBox(FindWindow(NULL,NULL),e.Description(),L"Error",MB_ICONERROR);
                            } 
                            catch (...)
                            {
                                ::MessageBox(FindWindow(NULL,NULL),L"unknown exception",L"Exception",MB_ICONERROR);
                            }
                            break;
                        }
                    }
                    lpWndMessageMap =	/*(pfnGetBaseMapFunc)*/(*(lpWndMessageMap->pfnGetBaseMap))();
                }
            }

            Sleep(GetThreadSleepTimeInterval());
        }
        _endthreadex(0);
        SetThreadStatus( E_MESSAGE_THREAD_EXIT );
        CloseHandle( m_hThread );
        m_hThread = NULL;
    }

    void CCMessageThread::ExitThread(bool bWaitExit /* = false */,DWORD dwMilliseconds /* = INFINITE */)
    {
        if ( !IsThreadRun() ) return ;
        SetThreadStatus( E_MESSAGE_THREAD_STOP );
        if ( bWaitExit ) WaitForSingleObject(m_hThread,dwMilliseconds);
        else Sleep(100);
    }

    bool CCMessageThread::PostThreadMessage(UINT Msg,WPARAM wParam /* = NULL */,LPARAM lParam /* = NULL */,int nCounts /* = 30 */)
    {
        while( nCounts > 1 && E_MESSAGE_THREAD_RUNING == m_bThreadStatus )
        {
            if(FALSE != ::PostThreadMessage(m_dThreadID,Msg,wParam,lParam) )
            {
                return true;
            }

            nCounts --;
            if (GetLastError() != ERROR_INVALID_THREAD_ID)
            {
                return false;
            }
            Sleep(100);
        }

        return false;
    }

    const AFX_THREAD_MSGMAP* CCMessageThread::GetThisMessageMap()
    {
        return NULL;
    }

    const  AFX_THREAD_MSGMAP* CCMessageThread::GetMessageMap() const
    {
        return NULL;
    }

    bool CCMessageThread::IsThreadRun()
    {
        if( E_MESSAGE_THREAD_EXIT == m_bThreadStatus || !m_hThread ) return false;
        DWORD dwExitCode = STILL_ACTIVE;
        GetExitCodeThread(m_hThread,&dwExitCode);

        return (dwExitCode == STILL_ACTIVE);
    }

    int CCMessageThread::GetThreadSleepTimeInterval()
    {
        return m_nThreadSleepTimeInterval;
    }

    void CCMessageThread::ResumeMessageThread()
    {
        if( IsThreadRun() ) ResumeThread(m_hThread);
    }

    void CCMessageThread::SuspendMessageThread()
    {
        if( IsThreadRun() ) SuspendThread(m_hThread);
    }

    void CCMessageThread::SetMessageThreadPriority(int nPriority /* = THREAD_PRIORITY_NORMAL */)
    {
        if( IsThreadRun() ) SetThreadPriority(m_hThread,nPriority);
    }

    void CCMessageThread::GetMessageThreadPriority(int& nPriority)
    {
        nPriority = -1;
        if( IsThreadRun() ) nPriority = GetThreadPriority(m_hThread);
    }
}
