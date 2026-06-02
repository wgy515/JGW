#include "stdafx.h"
#include <JGW_FoundationFunc/JGW_ThreadFunc.h>

namespace JGW
{
    bool JGW_ThreadIsRunning(HANDLE hThread)
    {
        DWORD dwExitCode = STILL_ACTIVE;
        GetExitCodeThread(hThread,&dwExitCode);

        return (dwExitCode == STILL_ACTIVE);
    }

    bool JGW_PostThreadMsg(DWORD dwThreadID,UINT Msg,WPARAM wParam,LPARAM lParam,int nCounts /* = 30 */)
    {
        while( nCounts>1 )
        {
            if(FALSE != ::PostThreadMessage(dwThreadID,Msg,wParam,lParam) )
            {
                return true;
            }

            nCounts --;
            if ( GetLastError() != ERROR_INVALID_THREAD_ID )
            {
                return false;
            }
            Sleep(100);
        }

        return false;
    }
}