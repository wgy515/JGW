/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/equipcommtl_tcp_ip.h#16 $
 * $DateTime: 2016/04/04 18:48:45 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2008-2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */

#pragma once
#include <string>
#include "IEquipCommTL.h"
#include "NetworkInstrumentProtocol.h"

#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"WS2_32.lib")

//!class CEquipCommTL_TCP_IP
class CEquipCommTL_TCP_IP : public IEquipCommTL
{
public:

   explicit CEquipCommTL_TCP_IP( int address, int secAddress = 0 )
   {
      address; // warning fix: unreferenced formal parameter
      secAddress; // warning fix: unreferenced formal parameter
      // this constructor should never be called for a TCP/IP controlled connection
   }
   //! inst_number default:0 GPIB1 1:GPIB2
   CEquipCommTL_TCP_IP( const std::string& ip_address , const int& inst_number,const std::string& strSourceAddress = "");
   CEquipCommTL_TCP_IP( const CEquipCommTL_TCP_IP& src);
   virtual ~CEquipCommTL_TCP_IP();
   CEquipCommTL_TCP_IP& operator=(const CEquipCommTL_TCP_IP& src);

   virtual bool write( const std::string& cmd );
   virtual bool write( void * buffer, long count );

   virtual bool read( std::string& retVal );
   virtual bool read( void * buffer, long bufferSize );
   virtual bool read( void * buffer, long bufferSize, long &bytesRead );

   virtual void query( const std::string& cmd, std::string& retVal );

   virtual bool query_ok( const std::string & cmd, std::string & retVal );

   virtual bool query(const char* buffer, float& fVal);

   bool CloseSession();

   const std::string m_forcedQueryPreIdendifier;
   const std::string m_forcedQueryPostIdendifier;
   int m_forcedQueryPreSize;
   int m_forcedQueryPostSize;

   virtual bool IsPresent();
   virtual bool GetErrorStatus();
   virtual bool ClearDevice();
   virtual bool SerialPoll( char& buffer );

   virtual bool SetTimeOut( double timeoutSec);
   virtual double GetTimeOut()
   {
      return m_timeout/1000.0;
   }

   virtual int GetDeviceAddress(void)
   {
      return 0;
   };

private:
   static const unsigned long maxBufferSize=0x100010;

protected:
   char* m_buffer/*[maxBufferSize]*/;
   bool m_isPresent;
   long m_lid;
   std::string m_dev_addr;
   CNetworkInstrumentProtocol*m_pInterface;
   unsigned long m_timeout;            // timeout in milliseconds
};
