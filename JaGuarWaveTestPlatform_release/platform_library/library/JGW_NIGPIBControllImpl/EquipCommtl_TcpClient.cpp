#include "StdAfx.h"
#include <JGW_NIGPIBControllImpl/EquipCommtl_TcpClient.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>

#define SCPI_COMMAND_TimeoutMsec 50
namespace JGW
{
    CEquipCommtl_TcpClient::CEquipCommtl_TcpClient(const std::string& host,unsigned int socketPort,const std::string& strSourceAddr /* = "" */) : mstrHost(host),mnSocketPort(socketPort),mstrSourceAddr(strSourceAddr),mstrWriteStringLineEnding("\r\n"),mstrReadStringIgnoreList("\r"),mstrReadStringLineEnding("\n")
    {
        OpenEquipCommtlTcpClient(host,socketPort,strSourceAddr);
    }


    CEquipCommtl_TcpClient::~CEquipCommtl_TcpClient(void)
    {

    }

    bool CEquipCommtl_TcpClient::OpenEquipCommtlTcpClient(const std::string& host,unsigned int socketPort,const std::string& strSourceAddr /* = "" */)
    {
        std::string port;
        JGW_FormatString(port,"%d",socketPort);
        return mAsyncSocket.ConnectSocketTimeOut(host.c_str(),port.c_str(),10 * 1000,10 * 1000,strSourceAddr.empty()?NULL:strSourceAddr.c_str());
    }

    bool CEquipCommtl_TcpClient::IsPresent()
    {
        return mAsyncSocket.CheckSocketIsConnected();
    }

    bool CEquipCommtl_TcpClient::write( const std::string& cmd )
    {
        std::string buf(cmd + mstrWriteStringLineEnding);
        ReplaceCommonEscapeSequences(buf);
        Log4WD(JGW_A2W(buf).c_str());
        return mAsyncSocket.AsyncWriteSocket(buf);
    }

    bool CEquipCommtl_TcpClient::write( void * buffer, long count )
    {
        Log4WD(JGW_A2W((const char*)buffer).c_str());
        return mAsyncSocket.AsyncWriteSocket((const char*)buffer,count);
    }

    bool CEquipCommtl_TcpClient::read( std::string& retVal )
    {
        char buf[1024] = {0};
        jgw_timer jt;
        retVal = "";

        while (jt.elapsed() < SCPI_COMMAND_TimeoutMsec)
        {
            mAsyncSocket.AsyncReadSocket(buf,1023);
            retVal += buf;
            //! \n mstrReadStringLineEnding = '\n'
            if (retVal.size() > 0 && '\n' == retVal[retVal.size() - 1])
            {
                //InsertCommonEscapeSequences(retVal);
                Log4WD(JGW_A2W(retVal).c_str());
                return true;
            }
        }
        Log4WD(JGW_A2W(retVal).c_str());
        return false;
    }

    bool CEquipCommtl_TcpClient::read( void * buffer, long bufferSize )
    {
        bool result = mAsyncSocket.AsyncReadSocket((char*)buffer,bufferSize);
        Log4WD(JGW_A2W((const char*)buffer).c_str());
        return result;
    }

    bool CEquipCommtl_TcpClient::read( void * buffer, long bufferSize, long &bytesRead )
    {
        bytesRead = mAsyncSocket.RecvSocket((char*)buffer,bufferSize);
        //Log4AI((const char*)buffer);
        Log4WD(JGW_A2W((const char*)buffer).c_str());
        return bytesRead > 0;
    }

    void CEquipCommtl_TcpClient::query( const std::string & cmd, std::string & retVal )
    {
        if (write(cmd))
        {
            read(retVal);
        }
    }

    bool CEquipCommtl_TcpClient::query_ok( const std::string & cmd, std::string & retVal )
    {
        return ( write(cmd) && read(retVal) );
    }

    bool CEquipCommtl_TcpClient::query( const char* buffer, float& fVal )
    {
        std::string readVal;
        if (query_ok(buffer,readVal))
        {
            JGW_ReplaceStringA(readVal,"\\n","");
            JGW_ReplaceStringA(readVal,"\\r","");
            JGW_ReplaceStringA(readVal,"\n","");
            JGW_ReplaceStringA(readVal,"\r","");
            fVal = static_cast<float>(atof(readVal.c_str()));
            return true;
        }
        return false;
    }

    bool CEquipCommtl_TcpClient::GetErrorStatus()
    {
        return false;
    }

    bool CEquipCommtl_TcpClient::ClearDevice()
    {
        return false;
    }

    bool CEquipCommtl_TcpClient::CloseSession()
    {
        mAsyncSocket.CleanupSocket();
        return true;
    }

    bool CEquipCommtl_TcpClient::SerialPoll( char& buffer )
    {
        return false;
    }

    bool CEquipCommtl_TcpClient::SetTimeOut( double timeoutSec )
    {
        return false;
    }

    double CEquipCommtl_TcpClient::GetTimeOut()
    {
        return 0;
    }

    int CEquipCommtl_TcpClient::GetDeviceAddress()
    {
        return mnSocketPort;
    }

    std::string CEquipCommtl_TcpClient::ReplaceCommonEscapeSequences(std::string& str)
    {
        //str.Replace("\\n", "\n").Replace("\\r", "\r");
        JGW_ReplaceStringA(str,"\\n","\n");
        JGW_ReplaceStringA(str,"\\r","\r");
        return str;
    }

    std::string CEquipCommtl_TcpClient::InsertCommonEscapeSequences(std::string& str)
    {
        JGW_ReplaceStringA(str,"\n","\\n");
        JGW_ReplaceStringA(str,"\r","\\r");
        return str;
    }
}

