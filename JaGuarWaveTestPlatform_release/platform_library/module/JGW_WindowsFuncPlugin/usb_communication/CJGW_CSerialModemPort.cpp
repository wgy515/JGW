#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_CSerialModemPort.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CCJGW_CSerialModemPort::CCJGW_CSerialModemPort(void)
    {
        memset(&mov,0x00,sizeof(OVERLAPPED));
        mov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        mTxBuf = (char*)calloc(0x01,TX_QUEUE_BUFFER_LEN);
        mRxBuf = (char*)calloc(0x01,RX_QUEUE_BUFFER_LEN);
    }


    CCJGW_CSerialModemPort::~CCJGW_CSerialModemPort(void)
    {
        free((void*)mTxBuf);
        free((void*)mRxBuf);
        CloseHandle(mov.hEvent);
        CloseModemPort();
    }

    bool CCJGW_CSerialModemPort::OpenModemPort(std::wstring strComName,int nbaud_rate /* = CBR_9600 */,DWORD dwFlagsAndAttributes /* = FILE_FLAG_OVERLAPPED */)
    {
        CloseModemPort();
        mdwFlagsAndAttributes = dwFlagsAndAttributes;

        if (!OpenPort(strComName.c_str()))
        {
            Log4WE_F(L"Open Port(%s) Fail",strComName.c_str());
            return false;
        }

        if (!SetControlCommState(nbaud_rate))
        {
            Log4WE(L"SetCommState DCB Fail");
            return false;
        }

        if (!SetModemTimeOuts())
        {
            Log4WE(L"SetModemTimeOuts Fail");
            return false;
        }

        if (!mSerialPort.PurgeCommPort())
        {
            Log4WE(L"PurgeCommPort Fail");
            return false;
        }

        if (!mSerialPort.SetCommMaskPort())
        {
            Log4WE(L"SetCommMaskPort Fail");
            return false;
        }

        if (!mSerialPort.SetupCommPort())
        {
            Log4WE(L"SetupCommPort Fail");
            return false;
        }
  
        return true;
    }
    
    bool CCJGW_CSerialModemPort::SendATCommandOK(const char* szFormat,const char* szValue,long millis /* = 4 * 1000 */)
    {
        std::string strReadValue;
        if (!ReadATCommandValue(szFormat,szValue,strReadValue))
        {
            return false;
        }
        return true;
    }

    bool CCJGW_CSerialModemPort::ReadATCommandValue(const char* szFormat,const char* szValue,std::string& strReturnValue,long millis /* = 4 * 1000 */)
    {
        if (!IsOpenModemPort()) return false;
        std::string strAtCommand(szFormat);
        if (szValue) JGW_FormatString(strAtCommand,szFormat,szValue);

        if (0 >= WriteAsyncATModem(strAtCommand.c_str(),strAtCommand.length(),millis))
        {
            Log4WE(_T("Error writing to AT command"));
            return false;
        }

        if (0 >= ReadATAsyncModem(strReturnValue,millis))
        {
            Log4WE(L"Error reading AT command");
            return false;
        }

        if (NULL == JGW_StrCaseStr(strReturnValue.c_str(),"OK"))
        {
            Log4WE(L"The AT command returns an error");
            return false;
        }

        return true;
    }



    int CCJGW_CSerialModemPort::ReadATAsyncModem(std::string& strRead,long millis /* = 4 * 1000 */,size_t max_len /* = RX_QUEUE_BUFFER_LEN */)
    {
        strRead.resize(max_len);
        return ReadATAsyncModem(const_cast<char*>(strRead.data()),max_len,millis);
    }

    int CCJGW_CSerialModemPort::ReadATAsyncModem(char* strRead,size_t len,long millis /* = 0 */)
    {
        if (!CheckIsAsyncRWATModem(strRead) || len <= 0) return -1;

        jgw_timer tm;
        DWORD dwError = 0;
        DWORD dwRead = 0;
        DWORD dwBytesRead = 0;
        bool bResult = true;
        bool bReader = true;
        double fMillis = millis / 1000.00;
        COMSTAT comstat;
        memset(strRead,0x00,len);

        while (tm.elapsed() <= fMillis)
        {
            //! 检查缓冲区是否超过读取缓冲区大小
            if (dwRead >= len) break;
            //! 检查缓冲区是否有数据读取
            mSerialPort.ClearCommErrorPort(dwError,&comstat);
            if (comstat.cbInQue == 0) continue;
            //! 有数据则异步读取
            bResult =  mSerialPort.ReadPort(strRead + dwRead,comstat.cbInQue,dwBytesRead,&mov);
            //! 检查读写情况
            bReader = (ERROR_IO_PENDING == GetLastError());
            if (!bResult && !bReader)
            {
                Log4WI(L"ReadATAsyncModem Fail");
                return -1;
            }
            //! 当ERROR_IO_PENDING的时候
            if (bReader)
            {
                if (WAIT_OBJECT_0 != mSerialPort.GetOverlappedResultPortEx(&mov,dwBytesRead,static_cast<long>((fMillis - tm.elapsed()) * 1000.00)))
                {
                   Log4WI_F(L"ReadATAsyncModem GetOverlappedResultPortEx ERROR:%s" ,JGW_GetSystemErrorString(GetLastError()).c_str());
                    return -1;
                }
                dwBytesRead = 0;
            }
            dwRead += dwBytesRead;
            //! 检查当前AT命令是否读取完全
            if (NULL != strstr(strRead,"OK\r") || strstr(strRead,"ERROR\r") || strstr(strRead,"+CME ERROR:"))  break;
        }
        Log4WI_F(L"ReadATAsyncModem:%s",JGW_A2W(strRead).c_str());

        return dwRead;
    }

    int CCJGW_CSerialModemPort::WriteAsyncATModem(const char* szATCommand,size_t len,long millis /* = 0 */)
    {
        if (!CheckIsAsyncRWATModem(szATCommand)) return -1;
        
        bool bResult = true;
        bool bWrite = false;
        DWORD dwWritenLen = 0;
        mov.Offset = 0;
        mov.OffsetHigh = 0;
        mSerialPort.PurgeCommPort();
        GetATCommandString(szATCommand,len);

        Log4WI_F(L"WriteAsyncATModem:%s",JGW_A2W_A(mTxBuf).c_str());
        bResult = mSerialPort.WritePort((LPVOID)mTxBuf,len,dwWritenLen,&mov);
        //! 如果当前不为异步模式并且写入失败
        bWrite = (ERROR_IO_PENDING == GetLastError());
        if (!bResult && !bWrite)
        {
            Log4WI(L"Write Serial Port Fail");
            return -1;
        }

        if (bWrite)
        {
            dwWritenLen = 0;
            if (WAIT_OBJECT_0 != mSerialPort.GetOverlappedResultPortEx(&mov,dwWritenLen,millis))
            {
                Log4WI_F(L"GetOverlappedResultPortEx Fail ERROR:%s" ,JGW_GetSystemErrorString(GetLastError()).c_str());
                return -1;
            }  
        }

        return dwWritenLen;
    }

    void CCJGW_CSerialModemPort::CloseModemPort()
    {
        if (IsOpenModemPort())
        {
            mSerialPort.ClosePort();
        }
    }

    bool CCJGW_CSerialModemPort::IsOpenModemPort()
    {
        return mSerialPort.IsOpenPort();
    }


    bool CCJGW_CSerialModemPort::CheckIsAsyncRWATModem(const char* szATCommand)
    {
        if (FILE_FLAG_OVERLAPPED != mdwFlagsAndAttributes || NULL == szATCommand )
        {
            Log4WI(L"The read/write cache cannot be empty or the current IO is not in asynchronous mode");
            return false;
        }
        return true;
    }

    void CCJGW_CSerialModemPort::GetATCommandString(const char* szATCommand,size_t &len)
    {
        memcpy((void*)mTxBuf,szATCommand,len);
        if (L'\r' != szATCommand[len - 1])
        {
            mTxBuf[len] = L'\r';
            len += 1;
            mTxBuf[len] = L'\0';
        }
    }

    bool CCJGW_CSerialModemPort::OpenPort(const wchar_t* szComName)
    {
        std::wstring strComName(_T("\\\\.\\"));
        strComName += szComName;
        Log4WI_F(L"CJGW_CSerialModemPort::OpenPort Port:%s",strComName.c_str());
        return mSerialPort.OpenPort(strComName,mdwFlagsAndAttributes);
    }

    bool CCJGW_CSerialModemPort::SetControlCommState(int nbaud_rate)
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
        dcb.fDtrControl =DTR_CONTROL_HANDSHAKE ;//MODEL通信必须
        dcb.fRtsControl =RTS_CONTROL_HANDSHAKE;//MODEL通信协议
        return mSerialPort.SetControlCommState(&dcb);
    }
    //! SetCommMask(hComm,EV_RXCHAR|EV_TXEMPTY);
    bool CCJGW_CSerialModemPort::SetModemTimeOuts()
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

//     const wchar_t* CCJGW_CSerialModemPort::GetModemLastErrorLog()
//     {
//         return JGW_GETLASTERRORINFO;
//     }
}

