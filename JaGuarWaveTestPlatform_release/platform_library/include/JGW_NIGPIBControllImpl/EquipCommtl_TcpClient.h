#pragma once
#include "IEquipCommTL.h"
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>

namespace JGW
{
    class CEquipCommtl_TcpClient : public IEquipCommTL
    {
    public:
        CEquipCommtl_TcpClient(const std::string& host,unsigned int socketPort,const std::string& strSourceAddr = "");
        ~CEquipCommtl_TcpClient(void);
    public:
        virtual bool IsPresent();

        virtual bool write( const std::string& cmd );

        virtual bool write( void * buffer, long count );

        virtual bool read( std::string& retVal );

        virtual bool read( void * buffer, long bufferSize );

        virtual bool read( void * buffer, long bufferSize, long &bytesRead );

        virtual void query( const std::string & cmd, std::string & retVal );

        virtual bool query_ok( const std::string & cmd, std::string & retVal );

        virtual bool query( const char* buffer, float& fVal );

        virtual bool GetErrorStatus();

        virtual bool ClearDevice();

        virtual bool CloseSession();

        virtual bool SerialPoll( char& buffer );

        virtual bool SetTimeOut( double timeoutSec );

        virtual double GetTimeOut();

        virtual int GetDeviceAddress();
   private:
         bool OpenEquipCommtlTcpClient(const std::string& host,unsigned int socketPort,const std::string& strSourceAddr = "");

         std::string ReplaceCommonEscapeSequences(std::string& str);

         std::string InsertCommonEscapeSequences(std::string& str);
    private:
        unsigned int mnSocketPort;
        std::string mstrHost;
        std::string mstrSourceAddr;

        std::string mstrWriteStringLineEnding;

        // Token: 0x0400001B RID: 27
        std::string mstrReadStringIgnoreList;

        // Token: 0x0400001C RID: 28
        std::string mstrReadStringLineEnding;




        CCJGW_AsyncSocket mAsyncSocket;
    };
}


