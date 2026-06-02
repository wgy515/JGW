#include "StdAfx.h"
#include "CJGW_CSerialComPort.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CCJGW_CSerialComPort::CCJGW_CSerialComPort(const char* endStr):mszEndString(endStr)
    {
        memset(&mov,0x00,sizeof(OVERLAPPED));
        mov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        mTxBuf = (char*)calloc(0x01,TX_QUEUE_COMPORT_BUFFER_LEN);
        ///mRxBuf = (char*)calloc(0x01,RX_QUEUE_COMPORT_BUFFER_LEN);
    }


    CCJGW_CSerialComPort::~CCJGW_CSerialComPort(void)
    {
        free((void*)mTxBuf);
        ///free((void*)mRxBuf);
        CloseHandle(mov.hEvent);
        CloseSerialComPort();
    }

    void CCJGW_CSerialComPort::EraseCliString(std::string& strRead)
    {
        while (!strRead.empty() && L'\n' == strRead[0]) strRead.erase(0,1);
        if (strRead.empty()) return ;
        char temp = strRead.at(strRead.size() - 1);

        while (!strRead.empty() && (temp == '\n' || '>' == temp || '\r' == temp))
        {
            strRead.erase(strRead.size() - 1,1);
            if (strRead.empty()) return ;
            temp = strRead.at(strRead.size() - 1);
        }
        // while (!strRead.empty() && L'>' == strRead[strRead.size() - 1]) strRead.erase(strRead.size() - 2);
    }


    bool CCJGW_CSerialComPort::OpenSerialComPort(std::wstring strComName,int nbaud_rate /* = CBR_9600 */,DWORD dwFlagsAndAttributes /* = FILE_FLAG_OVERLAPPED */)
    {
        CloseSerialComPort();
        mdwFlagsAndAttributes = dwFlagsAndAttributes;

        if (!OpenPort(strComName.c_str()))
        {
            Log4WE_F(L"打开端口(%s)失败",strComName.c_str());
            return false;
        }

        if (!SetControlCommState(nbaud_rate))
        {
            Log4WE(L"SetCommState DCB 失败");
            return false;
        }

        if (!SetModemTimeOuts())
        {
            Log4WE(L"SetModemTimeOuts 失败");
            return false;
        }

        if (!mSerialPort.PurgeCommPort())
        {
            Log4WE(L"PurgeCommPort 失败");
            return false;
        }

        if (!mSerialPort.SetCommMaskPort())
        {
            Log4WE(L"SetCommMaskPort 失败");
            return false;
        }

        if (!mSerialPort.SetupCommPort())
        {
            Log4WE(L"SetupCommPort 失败");
            return false;
        }

        return true;
    }

    bool CCJGW_CSerialComPort::WriteBinaryFileBufferToComPort(const char* szBuf,size_t len,long millis /* = 4 * 1000 */)
    {
        if (!IsOpenComPort()) return false;

        if (0 >= WriteAsyncCommand(szBuf,len,millis))
        {
            Log4WE(_T("AT命令写入错误"));
            return false;
        }

        return true;
    }

    bool CCJGW_CSerialComPort::ReadResponseCommandToComPort(std::string& strRead,long millis /* = 4 * 1000 */)
    {
        if (!IsOpenComPort()) return false;

        DWORD dwRead = ReadAsyncCommand(strRead,millis);
        if (0 >= dwRead)
        {
            Log4WE(L"AT命令读取错误");
            return false;
        }
        strRead.resize(dwRead);

        return true;
    }

    bool CCJGW_CSerialComPort::WriteCommand(std::string strCommand,long millis /* = 4 * 1000 */)
    {
        return WriteBinaryFileBufferToComPort(strCommand.c_str(),strCommand.size(),millis);
    }

    //     bool CCJGW_CSerialComPort::WriteCommandToComPort(const char* szFormat/*,const char* szValue*/,long millis /* = 4 * 1000 */)
    //     {
    //         std::string strReadValue;
    //         if (!ReadValueToCommand(szFormat,strReadValue))
    //         {
    //             return false;
    //         }
    //         return true;
    //     }

    bool CCJGW_CSerialComPort::ReadRespCommandToWriteCommand(const char* szFormat/*,const char* szValue,*/,std::string& strReturnValue,long millis /* = 4 * 1000 */)
    {
        if (!IsOpenComPort()) return false;
        std::string strAtCommand(szFormat);
        DWORD dwRead = 0;
        /*if (szValue) JGW_FormatString(strAtCommand,szFormat,szValue);*/

        if (0 >= WriteAsyncCommand(strAtCommand.c_str(),strAtCommand.length(),millis))
        {
            Log4WE(_T("AT命令写入错误"));
            return false;
        }

        if (NULL == _strnicmp("reset",strAtCommand.c_str(),strlen("reset"))) 
        {
            return true;
        }

        dwRead = ReadAsyncCommand(strReturnValue,millis);
        if (0 >= dwRead)
        {
            Log4WE(L"AT命令读取错误");
            return false;
        }
        strReturnValue.resize(dwRead);

        return true;
    }



    int CCJGW_CSerialComPort::ReadAsyncCommand(std::string& strRead,long millis /* = 4 * 1000 */,size_t max_len /* = RX_QUEUE_BUFFER_LEN */)
    {
        strRead.resize(max_len);
        return ReadAsyncCommand(const_cast<char*>(strRead.data()),max_len,millis);
    }

    int CCJGW_CSerialComPort::ReadAsyncCommand(char* strRead,size_t len,long millis /* = 4 * 1000 */)
    {
        if (!CheckIsAsyncRWCoammnd(strRead) || len <= 0) return -1;

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
            if (dwRead >= len - 1) break;
            //! 检查缓冲区是否有数据读取
            mSerialPort.ClearCommErrorPort(dwError,&comstat);
            if (comstat.cbInQue == 0) continue;
            //! 防止超出数据读取
            if (dwRead + comstat.cbInQue >= len)
            {
                comstat.cbInQue = len - dwRead - 1;
            }
            //! if (len )
            //! 有数据则异步读取
            bResult =  mSerialPort.ReadPort(strRead + dwRead,comstat.cbInQue,dwBytesRead,&mov);
            //! 检查读写情况
            bReader = (ERROR_IO_PENDING == GetLastError());
            if (!bResult && !bReader)
            {
                Log4WI(L"ReadATAsyncModem 失败");
                return -1;
            }
            //! 当ERROR_IO_PENDING的时候
            if (bReader)
            {
                if (WAIT_OBJECT_0 != mSerialPort.GetOverlappedResultPortEx(&mov,dwBytesRead,static_cast<long>((fMillis - tm.elapsed()) * 1000.00)))
                {
                    Log4WI_F(L"ReadATAsyncModem GetOverlappedResultPortEx 失败 ERROR:%s" ,JGW_GetSystemErrorString(GetLastError()).c_str());
                    return -1;
                }
                //! dwBytesRead = 0;
            }
            dwRead += dwBytesRead;
            //! 检查当前AT命令是否读取完全
            if (NULL != strstr(strRead,mszEndString))  break;
        }
        Log4AI_F("ReadATAsyncModem:%s",strRead);

        return dwRead;
    }

    int CCJGW_CSerialComPort::WriteAsyncCommand(const char* szATCommand,size_t len,long millis /* = 0 */)
    {
        if (!CheckIsAsyncRWCoammnd(szATCommand)) return -1;

        bool bResult = true;
        bool bWrite = false;
        DWORD dwWritenLen = 0;
        mov.Offset = 0;
        mov.OffsetHigh = 0;
        mSerialPort.PurgeCommPort();
        GetATCommandString(szATCommand,len);

        bResult = mSerialPort.WritePort((LPVOID)mTxBuf,len,dwWritenLen,&mov);
        //! 如果当前不为异步模式并且写入失败
        bWrite = (ERROR_IO_PENDING == GetLastError());
        if (!bResult && !bWrite)
        {
            Log4WE(L"WriteAsyncATModem 失败");
            return -1;
        }

        if (bWrite)
        {
            dwWritenLen = 0;
            if (WAIT_OBJECT_0 != mSerialPort.GetOverlappedResultPortEx(&mov,dwWritenLen,millis))
            {
                Log4WE_F(L"WriteAsyncATModem GetOverlappedResultPortEx 失败 ERROR:%s" ,JGW_GetSystemErrorString(GetLastError()).c_str());
                return -1;
            }  
        }

        return dwWritenLen;
    }

    void CCJGW_CSerialComPort::CloseSerialComPort()
    {
        if (IsOpenComPort())
        {
            mSerialPort.ClosePort();
        }
    }

    bool CCJGW_CSerialComPort::IsOpenComPort()
    {
        return mSerialPort.IsOpenPort();
    }


    bool CCJGW_CSerialComPort::CheckIsAsyncRWCoammnd(const char* szATCommand)
    {
        if (FILE_FLAG_OVERLAPPED != mdwFlagsAndAttributes || NULL == szATCommand )
        {
            Log4WE(L"读写缓存不能为空或者当前IO不为异步模式");
            return false;
        }
        return true;
    }
    //! at\n
    void CCJGW_CSerialComPort::GetATCommandString(const char* szATCommand,size_t &len)
    {
        memcpy((void*)mTxBuf,szATCommand,len);
        char endChar = '\n';
        if (L'\n' == szATCommand[len - 1]) { mTxBuf[len - 1] = L'\0'; --len;}
        if (L'\r' == szATCommand[len - 1]) { endChar = '\r';mTxBuf[len - 1] = L'\0'; --len;}
        mTxBuf[len] = L'\0';
        Log4AI_F("SerialComPort Send : %s",(char*)mTxBuf);
        mTxBuf[len] = endChar;
        mTxBuf[++len] = L'\0';
    }

    bool CCJGW_CSerialComPort::OpenPort(const wchar_t* szComName)
    {
        std::wstring strComName(_T("\\\\.\\"));
        strComName += szComName;
        Log4WI_F(L"CJGW_CSerialModemPort::OpenPort Port:%s",strComName.c_str());
        return mSerialPort.OpenPort(strComName,mdwFlagsAndAttributes);
    }

    bool CCJGW_CSerialComPort::SetControlCommState(int nbaud_rate)
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
    bool CCJGW_CSerialComPort::SetModemTimeOuts()
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

    //     const wchar_t* CCJGW_CSerialComPort::GetComPortLastErrorLog()
    //     {
    //         return JGW_GETLASTERRORINFO;
    //     }
}

