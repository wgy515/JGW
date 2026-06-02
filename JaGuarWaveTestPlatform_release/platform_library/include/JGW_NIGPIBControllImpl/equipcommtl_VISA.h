/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/equipcommtl_VISA.h#13 $
 * $DateTime: 2016/04/27 16:14:41 $
 *
 *
 ******************************************************************************
 *
 * Copyright (c) 2012-2016 Qualcomm Technologies, Inc.
 * All rights reserved.
 * Qualcomm Technologies, Inc. Confidential and Proprietary.
 *
 ******************************************************************************
 */
#pragma once

#include <string>
#include "IEquipCommTL.h"
#include "IVI Foundation\VISA\WinNT\include\visa.h"
#include "IVI Foundation\VISA\WinNT\include\visatype.h"
#pragma comment(lib,"visa32.lib")

typedef struct
{
   bool defaultRM_stat;
   bool instr_stat;
   bool read_stat;
   bool write_stat;

} VisaStatusStruct;

//!class CEquipCommTL_VISA
class CEquipCommTL_VISA : public IEquipCommTL
{
public:

   CEquipCommTL_VISA(const std::string& visaAddress);
   virtual ~CEquipCommTL_VISA();

   //------------- write and read operations ---------------//
   virtual bool write(const std::string& cmd);
   virtual bool write( void * buffer, long count );
   virtual bool read(std::string& retVal);
   virtual bool read(void *buffer, long bufferSize);
   virtual bool read(void *buffer, long bufferSize, long &bytesRead );
   virtual void query(const std::string& cmd,std::string& retVal);
   virtual bool query_ok( const std::string & cmd, std::string & retVal );

   virtual bool query( const char* buffer, float& fVal );

   virtual bool CloseSession();

   //------------- miscellaneous -----------//
   virtual bool IsPresent()
   {
      return m_isPresent;
   }

   virtual bool GetErrorStatus()
   {

      return (m_VisaStatusStr.defaultRM_stat || m_VisaStatusStr.instr_stat
              || m_VisaStatusStr.read_stat || m_VisaStatusStr.write_stat);
   }

   virtual bool ClearDevice();
   virtual bool SerialPoll( char& buffer );
   virtual bool SetTimeOut(double timeoutSec);
   // returns the timeout in seconds
   virtual double GetTimeOut()
   {
      return m_timeout/1000.0;
   }

   //------------- setting and getting of attributes ------------//


   virtual void GetMaxBuffSize(unsigned long& retBuffSize)
   {
      retBuffSize = maxBufferSize;
   }

   virtual int GetDeviceAddress() { return 0; };

protected:
   //------------ resource manager and session setup ------------//
   bool OpenResourceMgr();
   bool OpenInstrSession();
   bool SetReadTerminator(unsigned int terminator);
   bool SetWriteTerminator(unsigned int terminator);
   void CheckSessionStatus();
   short GetInterfaceType();
   std::string GetResourceName();

   // This must be at least 1024 (VXI 11 Rule B.6.3)
   static const unsigned long maxBufferSize = 4096;

   // false indicates failure, true indicates success
   void initVisaStatusStr()
   {
      m_VisaStatusStr.defaultRM_stat = false;
      m_VisaStatusStr.instr_stat     = false;
      m_VisaStatusStr.read_stat      = false;
      m_VisaStatusStr.write_stat     = false;
   }

   // VISA specific members
   ViSession m_defaultRM;               // default resource manager (manages all sessions connected to it)
   ViSession m_instrHandle;            // instrument handle (session)
   // other data members
   bool m_isPresent;
   unsigned long m_timeout;            // timeout in milliseconds
   std::string m_visaAddress;
   VisaStatusStruct m_VisaStatusStr;
   ViAttrState m_writeTerminator;
   ViAttrState m_readTerminator;
};




