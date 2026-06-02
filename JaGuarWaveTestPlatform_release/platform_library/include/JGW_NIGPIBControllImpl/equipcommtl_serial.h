/******************************************************************************
 * $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/equipcommtl_serial.h#9 $
 * $DateTime: 2016/04/04 19:04:53 $
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

//!class CEquipCommTL_Serial
class CEquipCommTL_Serial:public IEquipCommTL
{
public:
   explicit CEquipCommTL_Serial(int comPortNumber);
   virtual ~CEquipCommTL_Serial();

   virtual bool write(const std::string& cmd);
   virtual bool write(void *buffer,long count);

   virtual bool read(std::string& retVal);
   virtual bool read(void *buffer,long bufferSize);
   virtual bool read(void *buffer,long bufferSize, long &bytesRead);

   virtual void query(const std::string& cmd,std::string& retVal);

   virtual bool query_ok( const std::string & cmd, std::string & retVal );

   virtual bool query( const char* buffer, float& fVal );

   virtual bool CloseSession();

   const std::string m_forcedQueryPreIdentifier;
   const std::string m_forcedQueryPostIdentifier;
   int m_forcedQueryPreSize;
   int m_forcedQueryPostSize;

   virtual bool IsPresent()
   {
      return m_isPresent;
   }
   virtual bool GetErrorStatus()
   {
      return m_error==ERROR_SUCCESS;
   }
   // Irrelevant for serial comms.
   virtual bool ClearDevice()
   {
      return false;
   }
   virtual bool SerialPoll(char& buffer)
   {
      buffer; // warning fix: unreferenced formal parameter
      return false;
   }

   virtual bool SetTimeOut(double timeoutSec)
   {
      m_timeout=static_cast<DWORD>(timeoutSec*1000);
      return true;
   }
   virtual double GetTimeOut()
   {
      return m_timeout/1000.0;
   }

   virtual int GetDeviceAddress()
   {
      return 0;
   }

protected:
   bool m_isPresent;
   DWORD m_timeout;
   HANDLE m_handle;
   int m_comPortNumber;
   unsigned long m_error;
   std::string m_dev_addr;
};
