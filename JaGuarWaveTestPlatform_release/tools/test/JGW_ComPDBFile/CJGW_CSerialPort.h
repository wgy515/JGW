#pragma once
#include <Windows.h>
#include <string>
namespace JGW
{
    class  CCJGW_CSerialPort
    {
    public:
        CCJGW_CSerialPort(void);
        ~CCJGW_CSerialPort(void);
        //! 打开端口
        bool OpenPort(std::wstring strComName,DWORD dwFlagsAndAttributes = FILE_FLAG_OVERLAPPED);
        //! 关闭端口
        void ClosePort();
        //! 是否打开端口
        bool IsOpenPort(void);
        //! 写端口操作
        bool WritePort(LPVOID lpBuffer,DWORD nNumberOfBytesToWrite,DWORD& nNumberOfBytesWritten,LPOVERLAPPED pOverlapped = NULL);
        //! 读端口操作
        bool ReadPort(LPVOID lpBuffer,DWORD nNumberOfBytesToRead,DWORD& nNumberOfBytesRead,LPOVERLAPPED pOverlapped = NULL);
        //! 
        bool SetControlCommState (LPDCB lpDCB = NULL);
        //! 
        bool SetCommPortTimeout(LPCOMMTIMEOUTS pCommTimeOuts);
        //!
        bool GetControlCommState(LPDCB pDCB);
        //! 清除串口缓冲区 
        bool PurgeCommPort(DWORD dwFlags = PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
        //!
        bool SetupCommPort(DWORD dwInQueue = 1024,DWORD dwOutQueue = 1024);
        //! 监视串口中有无数据和发送缓冲区中的数据是否全部发送完毕。
        bool SetCommMaskPort(DWORD dwEvtMask  = EV_RXCHAR | EV_TXEMPTY);

        bool WaitCommEventPort(DWORD& dwEvent,LPOVERLAPPED pOverlapped = NULL);

        bool GetOverlappedResultPort(LPOVERLAPPED pOverlapped,DWORD& dwWrite,BOOL bWait);

        DWORD GetOverlappedResultPortEx(LPOVERLAPPED pOverlapped,DWORD& dwWrite,long millis);

        bool ClearCommErrorPort(DWORD &dwErrors,LPCOMSTAT lpStat);
    private:
        HANDLE mhFileHandle;
    };
}


