#include "StdAfx.h"
#include "CJGW_QcommDownloadSerialPort.h"
#include <TSE_FoundationLib/TSE_FoundationLib_Define.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "sw_sb3_download_define.h"
namespace JGW
{
    CCJGW_QcommDownloadSerialPort::CCJGW_QcommDownloadSerialPort(void) : mnTxTimeOutMSec(10 * 1000),mnRxTimeOutMSec(10 * 1000)
    {
        memset(&mov,0x00,sizeof(OVERLAPPED));
        mov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    }


    CCJGW_QcommDownloadSerialPort::~CCJGW_QcommDownloadSerialPort(void)
    {
    }

    void CCJGW_QcommDownloadSerialPort::SetTxTimeOutMSec(int timeout)
    {
        mnTxTimeOutMSec = timeout;
    }

    void CCJGW_QcommDownloadSerialPort::SetRxTimeOutMSec(int timeout)
    {
        mnRxTimeOutMSec = timeout;
    }

    bool CCJGW_QcommDownloadSerialPort::OpenSaharaSerialComPort(std::wstring strComName,int nbaud_rate /* = CBR_115200 */,DWORD dwFlagsAndAttributes /* = FILE_FLAG_OVERLAPPED */)
    {
        CloseQcommDownloadSerialPort();
        mdwFlagsAndAttributes = dwFlagsAndAttributes;

        if (!OpenPort(strComName.c_str()))
        {
            LogE_F(L"打开端口(%s)失败",strComName.c_str());
            return false;
        }

        if (!SetControlCommState(nbaud_rate))
        {
            LogE_F(L"SetCommState DCB 失败");
            return false;
        }

        COMMTIMEOUTS timeouts ={0};
        timeouts.ReadIntervalTimeout        = 10000;
        timeouts.ReadTotalTimeoutMultiplier = 10000;
        timeouts.ReadTotalTimeoutConstant   = 10000;
        timeouts.WriteTotalTimeoutMultiplier= 10000;
        timeouts.WriteTotalTimeoutConstant  = 10000;
        if (!mSerialPort.SetCommPortTimeout(&timeouts))
        {
            LogE_F(L"SetModemTimeOuts 失败");
            return false;
        }

        /*if (!mSerialPort.PurgeCommPort())
        {
        LogE_F(L"PurgeCommPort 失败");
        return false;
        }*/

        if (!mSerialPort.SetCommMaskPort())
        {
            LogE_F(L"SetCommMaskPort 失败");
            return false;
        }

        if (!mSerialPort.SetupCommPort(STREAM_MAX_BUF_SIZE,STREAM_MAX_BUF_SIZE))
        {
            LogE_F(L"SetupCommPort 失败");
            return false;
        }

        return true;
    }

    bool CCJGW_QcommDownloadSerialPort::WriteBufferToDownloadPort(const char* szBuf,size_t len,size_t& writen_len)
    {
        mov.Offset = 0;
        mov.OffsetHigh = 0;
        DWORD dwWritenLen = 0;
        bool bResult = mSerialPort.WritePort((LPVOID)szBuf,len,dwWritenLen,&mov);
        //! 如果当前不为异步模式并且写入失败
        bool bWrite = (ERROR_IO_PENDING == GetLastError());
        if (!bResult && !bWrite)
        {
            LogE(L"WriteAsyncATModem 失败");
            return false;
        }

        if (bWrite)
        {
            dwWritenLen = 0;
            if (WAIT_OBJECT_0 != mSerialPort.GetOverlappedResultPortEx(&mov,dwWritenLen,mnTxTimeOutMSec))
            {
                LogE_F(L"WriteAsyncATModem GetOverlappedResultPortEx 失败 ERROR:%s" ,JGW_GetSystemErrorString(GetLastError()).c_str());
                return false;
            }  
        }
        writen_len = dwWritenLen;
        return writen_len > 0 ;
    }

    bool CCJGW_QcommDownloadSerialPort::ReadStreamingDownloadPort(char* buffer, size_t bytes_to_read, size_t& bytes_read)
    {
        jgw_timer tm;
        DWORD dwError = 0;
        DWORD dwRead = 0;
        DWORD dwBytesRead = 0;
        bytes_read = 0;
        bool bResult = true;
        bool bReader = true;
        COMSTAT comstat;
        while (tm.elapsed() * 1000.00 <= mnRxTimeOutMSec)
        {
            //! 检查缓冲区是否超过读取缓冲区大小
            if (dwRead >= bytes_to_read) break;
            //! 检查缓冲区是否有数据读取
            mSerialPort.ClearCommErrorPort(dwError,&comstat);
            if (comstat.cbInQue == 0) continue;
            //! 防止超出数据读取
            if (dwRead + comstat.cbInQue >= bytes_to_read)
            {
                comstat.cbInQue = bytes_to_read - dwRead;
            }
            //! if (len )
            //! 有数据则异步读取
            bResult =  mSerialPort.ReadPort(buffer + dwRead,comstat.cbInQue,dwBytesRead,&mov);
            //! 检查读写情况
            bReader = (ERROR_IO_PENDING == GetLastError());
            if (!bResult && !bReader)
            {
                LogE(L"ReadATAsyncModem 失败");
                return false;
            }
            //! 当ERROR_IO_PENDING的时候
            if (bReader)
            {
                if (WAIT_OBJECT_0 != mSerialPort.GetOverlappedResultPortEx(&mov,dwBytesRead, static_cast<long>(mnRxTimeOutMSec - tm.elapsed() * 1000.00)))
                {
                    LogE_F(L"read GetOverlappedResultPortEx 失败 ERROR:%s" ,JGW_GetSystemErrorString(GetLastError()).c_str());
                    return false;
                }
            }
            dwRead += dwBytesRead;
            if (FLAG_FRAME == buffer[0] && dwRead > 2 &&  FLAG_FRAME == buffer[dwRead - 1])
            {
                break;
            }
        }
        bytes_read = dwRead;

        return bytes_read > 0;
    }

    //bool CCJGW_QcommDownloadSerialPort::ReadSaharaDownloadPort(char* buffer, size_t bytes_to_read, size_t& bytes_read)
    //{
    //    jgw_timer tm;
    //    DWORD dwBytesRead = 0;
    //    DWORD dwRead = 0;
    //    bytes_read = 0;
    //    while (tm.elapsed() * 1000.00 <= mnRxTimeOutMSec)
    //    {
    //        bool bResult =  mSerialPort.ReadPort(buffer + dwRead,bytes_to_read - bytes_read,dwBytesRead,&mov);
    //        //! 检查读写情况
    //        bool bReader = (ERROR_IO_PENDING == GetLastError());
    //        if (!bResult && !bReader)
    //        {
    //            LogE(L"ReadATAsyncModem 失败");
    //            return false;
    //        }
    //        //! 当ERROR_IO_PENDING的时候
    //        if (bReader)
    //        {
    //            if (WAIT_OBJECT_0 != mSerialPort.GetOverlappedResultPortEx(&mov,dwBytesRead, static_cast<long>(mnRxTimeOutMSec - tm.elapsed() * 1000.00)))
    //            {
    //                LogE_F(L"read GetOverlappedResultPortEx 失败 ERROR:%s" ,JGW_GetSystemErrorString(GetLastError()).c_str());
    //                return false;
    //            }
    //        }
    //        bytes_read += dwBytesRead;
    //        if (bytes_to_read <= bytes_read)
    //        {
    //            return true;
    //        }
    //    }

    //    return false;
    //}

    bool CCJGW_QcommDownloadSerialPort::ReadDownloadPort(char* buffer, size_t bytes_to_read, size_t& bytes_read)
    {
        jgw_timer tm;
        DWORD dwError = 0;
        DWORD dwRead = 0;
        DWORD dwBytesRead = 0;
        bool bResult = true;
        bool bReader = true;
        bytes_read = 0;
        COMSTAT comstat;
        while (tm.elapsed() * 1000.00 <= mnRxTimeOutMSec)
        {
            //! 检查缓冲区是否超过读取缓冲区大小
            if (dwRead >= bytes_to_read) break;
            //! 检查缓冲区是否有数据读取
            mSerialPort.ClearCommErrorPort(dwError,&comstat);
            if (comstat.cbInQue == 0) continue;
            //! 防止超出数据读取
            if (dwRead + comstat.cbInQue >= bytes_to_read)
            {
                comstat.cbInQue = bytes_to_read - dwRead;
            }
            //! if (len )
            //! 有数据则异步读取
            bResult =  mSerialPort.ReadPort(buffer + dwRead,comstat.cbInQue,dwBytesRead,&mov);
            //! 检查读写情况
            bReader = (ERROR_IO_PENDING == GetLastError());
            if (!bResult && !bReader)
            {
                LogE(L"ReadATAsyncModem 失败");
                return false;
            }
            //! 当ERROR_IO_PENDING的时候
            if (bReader)
            {
                if (WAIT_OBJECT_0 != mSerialPort.GetOverlappedResultPortEx(&mov,dwBytesRead, static_cast<long>(mnRxTimeOutMSec - tm.elapsed() * 1000.00)))
                {
                    LogE_F(L"read GetOverlappedResultPortEx 失败 ERROR:%s" ,JGW_GetSystemErrorString(GetLastError()).c_str());
                    return false;
                }
            }
            dwRead += dwBytesRead;
            break;      
        }
        bytes_read = dwRead;

        return bytes_read > 0;
    }

    void CCJGW_QcommDownloadSerialPort::CloseQcommDownloadSerialPort()
    {
        if (IsOpenComPort())
        {
            mSerialPort.ClosePort();
        }
    }


    bool CCJGW_QcommDownloadSerialPort::IsOpenComPort()
    {
        return mSerialPort.IsOpenPort();
    }


    bool CCJGW_QcommDownloadSerialPort::OpenPort(const wchar_t* szComName)
    {
        std::wstring strComName(_T("\\\\.\\"));
        strComName += szComName;
        LogI_F(L"CJGW_CSerialModemPort::OpenPort Port:%s",strComName.c_str());
        return mSerialPort.OpenPort(strComName,mdwFlagsAndAttributes);
    }

    bool CCJGW_QcommDownloadSerialPort::SetControlCommState(int nbaud_rate)
    {
        DCB dcb;
        dcb.DCBlength =sizeof( DCB);
        mSerialPort.GetControlCommState(&dcb);
        dcb.fOutxCtsFlow = FALSE;
        dcb.fOutxDsrFlow = FALSE;
        dcb.fTXContinueOnXoff = TRUE;
        dcb.fDtrControl = DTR_CONTROL_ENABLE;
        dcb.fDsrSensitivity = FALSE;
        dcb.fOutX = FALSE;
        dcb.fInX = FALSE;
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
        dcb.fOutxCtsFlow = TRUE;
        dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;

        dcb.BaudRate = nbaud_rate;
        dcb.ByteSize = 8;
        dcb.StopBits = TWOSTOPBITS;
        dcb.fParity = FALSE;
        dcb.Parity = NOPARITY;
        //! dcb.fDtrControl =DTR_CONTROL_HANDSHAKE ;//MODEL通信必须
        //! dcb.fRtsControl =RTS_CONTROL_HANDSHAKE;//MODEL通信协议
        return mSerialPort.SetControlCommState(&dcb);
    }
    //! SetCommMask(hComm,EV_RXCHAR|EV_TXEMPTY);
    bool CCJGW_QcommDownloadSerialPort::SetModemTimeOuts()
    {
        COMMTIMEOUTS commTimeOuts ={0};
        commTimeOuts.ReadIntervalTimeout =500;//500 注释的时间比较稳定
        commTimeOuts.ReadTotalTimeoutMultiplier =10;//100
        commTimeOuts.ReadTotalTimeoutConstant =50;//500
        //设定写超时
        commTimeOuts.WriteTotalTimeoutMultiplier =100;
        commTimeOuts.WriteTotalTimeoutConstant =500;
        return mSerialPort.SetCommPortTimeout(&commTimeOuts);
    }
}
