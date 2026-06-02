#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_CSerialPort.h>

namespace JGW
{
    CCJGW_CSerialPort::CCJGW_CSerialPort(void):mhFileHandle(INVALID_HANDLE_VALUE)
    {
    }


    CCJGW_CSerialPort::~CCJGW_CSerialPort(void)
    {
    }

    bool CCJGW_CSerialPort::OpenPort(std::wstring strComName,DWORD dwFlagsAndAttributes /* = FILE_FLAG_OVERLAPPED */)
    {
        mhFileHandle = CreateFile (strComName.c_str() ,GENERIC_WRITE | GENERIC_READ,0,0, OPEN_EXISTING ,dwFlagsAndAttributes ,0);
        return IsOpenPort();
    }

    void CCJGW_CSerialPort::ClosePort()
    {
        if (NULL == mhFileHandle || INVALID_HANDLE_VALUE  == mhFileHandle) return ;
        CloseHandle(mhFileHandle);
        mhFileHandle = INVALID_HANDLE_VALUE;
    }


    bool CCJGW_CSerialPort::IsOpenPort()
    {
        if (NULL == mhFileHandle || INVALID_HANDLE_VALUE  == mhFileHandle) return false;
        DWORD dwError = 0;
        COMSTAT comStat;
        if (!ClearCommErrorPort(dwError,&comStat)) { ClosePort();return false;}
        return true;
    }

    bool CCJGW_CSerialPort::WritePort(LPVOID lpBuffer,DWORD nNumberOfBytesToWrite,DWORD& nNumberOfBytesWritten,LPOVERLAPPED pOverlapped /* = NULL */)
    {
        return (TRUE == WriteFile(mhFileHandle,lpBuffer,nNumberOfBytesToWrite,&nNumberOfBytesWritten,pOverlapped));
    }

    bool CCJGW_CSerialPort::ReadPort(LPVOID lpBuffer,DWORD nNumberOfBytesToRead,DWORD& nNumberOfBytesRead,LPOVERLAPPED pOverlapped /* = NULL */)
    {
        return (TRUE == ReadFile(mhFileHandle,lpBuffer,nNumberOfBytesToRead,&nNumberOfBytesRead,pOverlapped));
    }

    bool CCJGW_CSerialPort::SetControlCommState(LPDCB lpDCB /* = NULL */)
    {
        return (TRUE == SetCommState (mhFileHandle,lpDCB));
    }

    bool CCJGW_CSerialPort::SetCommPortTimeout(LPCOMMTIMEOUTS pCommTimeOuts)
    {
        return (TRUE == SetCommTimeouts(mhFileHandle,pCommTimeOuts));
    }

    bool CCJGW_CSerialPort::GetControlCommState(LPDCB pDCB)
    {
        return (TRUE == GetCommState (mhFileHandle,pDCB));
    }

    bool CCJGW_CSerialPort::PurgeCommPort(DWORD dwFlags /* = PURGE_RXCLEAR |PURGE_TXCLEAR |PURGE_RXABORT | PURGE_TXABORT */)
    {
        return (TRUE == PurgeComm(mhFileHandle, dwFlags));
    }

    bool CCJGW_CSerialPort::SetCommMaskPort(DWORD dwEvtMask /* = EV_RXCHAR | EV_TXEMPTY */)
    {
        return (TRUE == SetCommMask(mhFileHandle,dwEvtMask));
    }

    bool CCJGW_CSerialPort::WaitCommEventPort(DWORD& dwEvent,LPOVERLAPPED pOverlapped /* = NULL */)
    {
        return (TRUE == WaitCommEvent(mhFileHandle,&dwEvent,pOverlapped));
    }

    bool CCJGW_CSerialPort::SetupCommPort(DWORD dwInQueue /* = 1024 */,DWORD dwOutQueue /* = 1024 */)
    {
        return (TRUE == SetupComm(mhFileHandle,dwInQueue,dwOutQueue));
    }

    bool CCJGW_CSerialPort::GetOverlappedResultPort(LPOVERLAPPED pOverlapped,DWORD& dwWrite,BOOL bWait)
    {
        return (TRUE == GetOverlappedResult(mhFileHandle,pOverlapped,&dwWrite,bWait));
    }
    #define COM_ERROR 31

    bool CCJGW_CSerialPort::ClearCommErrorPort(DWORD &dwErrors,LPCOMSTAT lpStat)
    {
        return (TRUE == ClearCommError(mhFileHandle,&dwErrors,lpStat));
/*        { */
//             switch (dwErrors )
//             {
//             case COM_ERROR :
//                 ClosePort();
//                 return (_T ("端口断开 "));
//             case CE_BREAK :
//                 return (_T ("检测到中断信号 "));
//             case CE_FRAME :
//                 return(_T ("硬件检测到帧错误 "));
//             case  CE_IOE :
//                 return(_T ("通信设备发生输入 /输出错误"));
//             case  CE_MODE :
//                 return (_T ("设置模式错误或是串口错误 "));
//             case CE_OVERRUN :
//                 return (_T ("溢出错误，缓冲区容量不足 "));
//             case CE_RXOVER :
//                 return (_T ("输出溢出错误 "));
//             case CE_RXPARITY :
//                 return (_T ("硬件检查到校验位错误 "));
//             case CE_TXFULL :
//                 PurgeCommPort();
//                 return (_T ("发送缓冲区已满 "));
//             }
//         }
//         return L"unknow";
    }

    DWORD CCJGW_CSerialPort::GetOverlappedResultPortEx(LPOVERLAPPED pOverlapped,DWORD& dwWrite,long millis)
    {
        if (!pOverlapped) return false;

        if (STATUS_PENDING  == pOverlapped->Internal)
        {
            DWORD dwWaitRet = WAIT_TIMEOUT;
            dwWaitRet = WaitForSingleObject((NULL != pOverlapped->hEvent)?pOverlapped->hEvent:mhFileHandle,millis);

            if (WAIT_TIMEOUT == dwWaitRet)
            {
                SetLastError(ERROR_IO_INCOMPLETE);
                return WAIT_TIMEOUT;
            }

            if (WAIT_OBJECT_0  != dwWaitRet) return dwWaitRet;
        }

        dwWrite = pOverlapped->InternalHigh;

        if (SUCCEEDED(pOverlapped->Internal)) return WAIT_OBJECT_0;

        SetLastError(pOverlapped->Internal);

        return WAIT_FAILED;
    }
}

